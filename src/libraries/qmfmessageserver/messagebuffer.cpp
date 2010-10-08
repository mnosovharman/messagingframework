#include "messagebuffer.h"
#include "qmailstore.h"

#include <QSettings>
#include <QDebug>
#include <QTimer>
#include <QFile>

class BufferItem
{
public:
    BufferItem(bool _add, MessageBufferFlushCallback *_callback, QMailMessage *_message)
        : add(_add)
        , callback(_callback)
        , message(_message)
    {
    }

    bool add;
    MessageBufferFlushCallback *callback;
    QMailMessage *message;
};

Q_GLOBAL_STATIC(MessageBuffer, messageBuffer)

MessageBuffer::MessageBuffer(QObject *parent)
    : QObject(parent)
{
    m_messageTimer = new QTimer(this);
    m_messageTimer->setSingleShot(true);
    connect(m_messageTimer, SIGNAL(timeout()), this, SLOT(messageTimeout()));

    m_lastFlushTimePerMessage = 0;

    readConfig();
}

MessageBuffer::~MessageBuffer()
{
}

MessageBuffer *MessageBuffer::instance()
{
    return messageBuffer();
}

bool MessageBuffer::addMessage(QMailMessage *message)
{
    m_waitingForCallback.append(new BufferItem(true, 0, message));
    return true;
}

bool MessageBuffer::updateMessage(QMailMessage *message)
{
    m_waitingForCallback.append(new BufferItem(false, 0, message));
    return true;
}

BufferItem *MessageBuffer::get_item(QMailMessage *message)
{
    foreach (BufferItem *item, m_waitingForCallback) {
        if (item->message == message) {
            m_waitingForCallback.removeOne(item);
            return item;
        }
    }

    return 0;
}

// We "own" the callback instance (makes the error case simpler for the client to handle)
bool MessageBuffer::setCallback(QMailMessage *message, MessageBufferFlushCallback *callback)
{
    BufferItem *item = get_item(message);
    if (!message) {
        // This message was not scheduled for adding or updating
        delete callback;
        return false;
    }

    item->callback = callback;
    item->message = new QMailMessage(*message);
    m_waitingForFlush.append(item);

    if (isFull() || !m_messageTimer->isActive()) {
        // If the buffer is full we flush.
        // If the timer isn't running we flush.
        messageFlush();
    }

    return true;
}

bool MessageBuffer::removeMessages(const QMailMessageKey &key)
{
    foreach (BufferItem *item, m_waitingForCallback) {
        QMailMessageKey itemKey(QMailMessageKey::serverUid(item->message->serverUid()) & QMailMessageKey::parentAccountId(item->message->parentAccountId()));
        if (key == itemKey) {
            m_waitingForCallback.removeOne(item);
            delete item->message;
            delete item->callback;
            delete item;
        }
    }
    foreach (BufferItem *item, m_waitingForFlush) {
        QMailMessageKey itemKey(QMailMessageKey::serverUid(item->message->serverUid()) & QMailMessageKey::parentAccountId(item->message->parentAccountId()));
        if (key == itemKey) {
            m_waitingForCallback.removeOne(item);
            delete item->message;
            delete item->callback;
            delete item;
        }
    }
    return QMailStore::instance()->removeMessages(key);
}

int MessageBuffer::countMessages(const QMailMessageKey &key)
{
    int count = 0;
    foreach (BufferItem *item, m_waitingForCallback) {
        QMailMessageKey check = QMailMessageKey::serverUid(item->message->serverUid()) & QMailMessageKey::parentAccountId(item->message->parentAccountId());
        if (check == key)
            ++count;
    }
    foreach (BufferItem *item, m_waitingForFlush) {
        QMailMessageKey check = QMailMessageKey::serverUid(item->message->serverUid()) & QMailMessageKey::parentAccountId(item->message->parentAccountId());
        if (check == key)
            ++count;
    }
    count += QMailStore::instance()->countMessages(key);
    return count;
}

void MessageBuffer::messageTimeout()
{
    if (messagePending()) {
        messageFlush();
    } else {
        m_lastFlushTimePerMessage = 0;
        m_messageTimer->setInterval(m_idleTimeout);
    }
}

void MessageBuffer::messageFlush()
{
    QMailStore *store = QMailStore::instance();
    QList<QMailMessage*> work;
    QTime commitTimer;
    int processed = messagePending();
    commitTimer.start();

    // Start by processing all the new messages
    foreach (BufferItem *item, m_waitingForFlush) {
        if (item->add)
            work.append(item->message);
    }
    if (work.count())
        store->addMessages(work);
    foreach (BufferItem *item, m_waitingForFlush) {
        if (item->add)
            item->callback->messageFlushed(item->message);
    }

    // Now we process all tne updated messages
    work.clear();
    foreach (BufferItem *item, m_waitingForFlush) {
        if (!item->add)
            work.append(item->message);
    }
    if (work.count())
        store->updateMessages(work);
    foreach (BufferItem *item, m_waitingForFlush) {
        if (!item->add)
            item->callback->messageFlushed(item->message);
    }

    // Delete all the temporarily memory
    foreach (BufferItem *item, m_waitingForFlush) {
        delete item->message;
        delete item->callback;
        delete item;
    }
    m_waitingForFlush.clear();

    int timePerMessage = commitTimer.elapsed() / processed;
    if (timePerMessage > m_lastFlushTimePerMessage && m_messageTimer->interval() < m_maxTimeout) {
        // increase the timeout
        int interval = m_messageTimer->interval() * m_timeoutScale;
        int actual = (interval > m_maxTimeout)?m_maxTimeout:interval;
        m_messageTimer->setInterval(actual);
    }
    m_lastFlushTimePerMessage = timePerMessage;

    m_messageTimer->start();

    if (processed)
        emit flushed();
}

void MessageBuffer::flush()
{
    if (messagePending())
        messageFlush();
}

void MessageBuffer::readConfig()
{
    QSettings settings("Nokia", "QMF");
    settings.beginGroup("MessageBuffer");

    m_maxPending = settings.value("maxPending", 1000).toInt();
    m_idleTimeout = settings.value("idleTimeout", 1000).toInt();
    m_maxTimeout = settings.value("maxTimeout", 8000).toInt();
    m_timeoutScale = settings.value("timeoutScale", 2.0f).value<qreal>();

    m_messageTimer->setInterval(m_idleTimeout);
}

void MessageBuffer::removeCallback(MessageBufferFlushCallback *callback)
{
    foreach (BufferItem *item, m_waitingForFlush) {
        if (item->callback == callback) {
            m_waitingForCallback.removeOne(item);
            delete item->message;
            delete item->callback;
            delete item;
        }
    }
}
