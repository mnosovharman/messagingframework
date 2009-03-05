/****************************************************************************
**
** This file is part of the $PACKAGE_NAME$.
**
** Copyright (C) $THISYEAR$ $COMPANY_NAME$.
**
** $QT_EXTENDED_DUAL_LICENSE$
**
****************************************************************************/

#include "messageserver.h"
#include "servicehandler.h"
#include "mailmessageclient.h"
#include <qmailfolder.h>
#include <qmailmessage.h>
#include <qmailstore.h>
#include <QDataStream>
#ifdef QMAIL_QTOPIA
#include <QMimeType>
#include <QtopiaApplication>
#include <QtopiaServiceRequest>
#include <qnetworkstate.h>
#endif
#include <QTimer>
#include <qmaillog.h>
#include <qmailipc.h>
#include <newcountnotifier.h>

MessageServer::MessageServer(QObject *parent)
    : QObject(parent),
      handler(0),
      client(new MailMessageClient(this)),
      messageCountUpdate("QPE/Messages/MessageCountUpdated"),
      newMessageTotal(0),
      completionAttempted(false)
{
    qMailLog(Messaging) << "MessageServer ctor begin";
#ifdef QMAIL_QTOPIA
    QtopiaApplication::loadTranslations("libqtopiamail");
#else
    new QCopServer(this);
#endif

    QMailMessageCountMap::iterator it = messageCounts.begin(), end = messageCounts.end();
    for ( ; it != end; ++it)
        it.value() = 0;

    QMailStore *store = QMailStore::instance();
    if (!store->initialized()) {
#ifdef QMAIL_QTOPIA
        // We can't do anything without the mail store...
        QtopiaServiceRequest req("SystemMessages", "showDialog(QString,QString)");
        req << tr("Messaging DB Invalid");
        req << tr("Messaging cannot operate due to database incompatibilty!");
        req.send();
#else
        qFatal("Messaging DB Invalid: Messaging cannot operate due to database incompatibilty!");
#endif
        // Do not close, however, or QPE will start another instance.
    } else {
        handler = new ServiceHandler(this);

        connect(store, SIGNAL(messagesAdded(QMailMessageIdList)),
                this, SLOT(messagesAdded(QMailMessageIdList)));
        connect(store, SIGNAL(messagesUpdated(QMailMessageIdList)),
                this, SLOT(messagesUpdated(QMailMessageIdList)));
        connect(store, SIGNAL(messagesRemoved(QMailMessageIdList)),
                this, SLOT(messagesRemoved(QMailMessageIdList)));

        // Propagate email handler signals to the client
        connect(handler, SIGNAL(activityChanged(quint64, QMailServiceAction::Activity)),
                client, SIGNAL(activityChanged(quint64, QMailServiceAction::Activity)));
        connect(handler, SIGNAL(connectivityChanged(quint64, QMailServiceAction::Connectivity)),
                client, SIGNAL(connectivityChanged(quint64, QMailServiceAction::Connectivity)));
        connect(handler, SIGNAL(statusChanged(quint64, const QMailServiceAction::Status)),
                client, SIGNAL(statusChanged(quint64, const QMailServiceAction::Status)));
        connect(handler, SIGNAL(progressChanged(quint64, uint, uint)),
                client, SIGNAL(progressChanged(quint64, uint, uint)));
        connect(handler, SIGNAL(messagesDeleted(quint64, QMailMessageIdList)), 
                client, SIGNAL(messagesDeleted(quint64, QMailMessageIdList)));
        connect(handler, SIGNAL(messagesCopied(quint64, QMailMessageIdList)), 
                client, SIGNAL(messagesCopied(quint64, QMailMessageIdList)));
        connect(handler, SIGNAL(messagesMoved(quint64, QMailMessageIdList)), 
                client, SIGNAL(messagesMoved(quint64, QMailMessageIdList)));
        connect(handler, SIGNAL(storageActionCompleted(quint64)),
                client, SIGNAL(storageActionCompleted(quint64)));
        connect(handler, SIGNAL(matchingMessageIds(quint64, QMailMessageIdList)), 
                client, SIGNAL(matchingMessageIds(quint64, QMailMessageIdList)));
        connect(handler, SIGNAL(searchCompleted(quint64)),
                client, SIGNAL(searchCompleted(quint64)));
        connect(handler, SIGNAL(protocolResponse(quint64, QString, QVariant)), 
                client, SIGNAL(protocolResponse(quint64, QString, QVariant)));
        connect(handler, SIGNAL(protocolRequestCompleted(quint64)),
                client, SIGNAL(protocolRequestCompleted(quint64)));

        connect(handler, SIGNAL(transmissionCompleted(quint64)), 
                this, SLOT(transmissionCompleted(quint64)));
        connect(handler, SIGNAL(messagesTransmitted(quint64, QMailMessageIdList)), 
                this, SLOT(messagesTransmitted(quint64, QMailMessageIdList)));
        connect(handler, SIGNAL(retrievalCompleted(quint64)),
                this, SLOT(retrievalCompleted(quint64)));

        // The email handler should handle the email client signals
        connect(client, SIGNAL(transmitMessages(quint64, QMailAccountId)),
                handler, SLOT(transmitMessages(quint64, QMailAccountId)));
        connect(client, SIGNAL(retrieveFolderList(quint64, QMailAccountId, QMailFolderId, bool)),
                handler, SLOT(retrieveFolderList(quint64, QMailAccountId, QMailFolderId, bool)));
        connect(client, SIGNAL(retrieveMessageList(quint64, QMailAccountId, QMailFolderId, uint, QMailMessageSortKey)),
                handler, SLOT(retrieveMessageList(quint64, QMailAccountId, QMailFolderId, uint, QMailMessageSortKey)));
        connect(client, SIGNAL(retrieveMessages(quint64, QMailMessageIdList, QMailRetrievalAction::RetrievalSpecification)),
                handler, SLOT(retrieveMessages(quint64, QMailMessageIdList, QMailRetrievalAction::RetrievalSpecification)));
        connect(client, SIGNAL(retrieveMessagePart(quint64, QMailMessagePart::Location)),
                handler, SLOT(retrieveMessagePart(quint64, QMailMessagePart::Location)));
        connect(client, SIGNAL(retrieveMessageRange(quint64, QMailMessageId, uint)),
                handler, SLOT(retrieveMessageRange(quint64, QMailMessageId, uint)));
        connect(client, SIGNAL(retrieveMessagePartRange(quint64, QMailMessagePart::Location, uint)),
                handler, SLOT(retrieveMessagePartRange(quint64, QMailMessagePart::Location, uint)));
        connect(client, SIGNAL(retrieveAll(quint64, QMailAccountId)),
                handler, SLOT(retrieveAll(quint64, QMailAccountId)));
        connect(client, SIGNAL(exportUpdates(quint64, QMailAccountId)),
                handler, SLOT(exportUpdates(quint64, QMailAccountId)));
        connect(client, SIGNAL(synchronize(quint64, QMailAccountId)),
                handler, SLOT(synchronize(quint64, QMailAccountId)));
        connect(client, SIGNAL(deleteMessages(quint64, QMailMessageIdList, QMailStore::MessageRemovalOption)),
                handler, SLOT(deleteMessages(quint64, QMailMessageIdList, QMailStore::MessageRemovalOption)));
        connect(client, SIGNAL(copyMessages(quint64, QMailMessageIdList, QMailFolderId)),
                handler, SLOT(copyMessages(quint64, QMailMessageIdList, QMailFolderId)));
        connect(client, SIGNAL(moveMessages(quint64, QMailMessageIdList, QMailFolderId)),
                handler, SLOT(moveMessages(quint64, QMailMessageIdList, QMailFolderId)));
        connect(client, SIGNAL(cancelTransfer(quint64)),
                handler, SLOT(cancelTransfer(quint64)));
        connect(client, SIGNAL(protocolRequest(quint64, QMailAccountId, QString, QVariant)),
                handler, SLOT(protocolRequest(quint64, QMailAccountId, QString, QVariant)));
        connect(client, SIGNAL(searchMessages(quint64, QMailMessageKey, QString, QMailSearchAction::SearchSpecification, QMailMessageSortKey)),
                handler, SLOT(searchMessages(quint64, QMailMessageKey, QString, QMailSearchAction::SearchSpecification, QMailMessageSortKey)));
        connect(client, SIGNAL(cancelSearch(quint64)),
                handler, SLOT(cancelSearch(quint64)));
        connect(client, SIGNAL(shutdown()),
                handler, SLOT(shutdown()));
       connect(handler, SIGNAL(newMessagesAvailable()),
                this, SLOT(reportNewCounts()));
        connect(client, SIGNAL(acknowledgeNewMessages(QMailMessageTypeList)),
                this, SLOT(acknowledgeNewMessages(QMailMessageTypeList)));

#ifdef QMAIL_QTOPIA
        QtopiaIpcAdaptor::connect
#else
        QCopAdaptor::connect
#endif
            (this, SIGNAL(messageCountUpdated()),
             &messageCountUpdate, MESSAGE(changeValue()));
    }
}

MessageServer::~MessageServer()
{
}

void MessageServer::retrievalCompleted(quint64 action)
{
    // Ensure the client receives any resulting events before a notification
    QMailStore::instance()->flushIpcNotifications();

    if (!completionList.isEmpty()) {
        if (!completionAttempted) {
            // Complete the messages that we selected for immediate completion
            completionAttempted = true;
            handler->retrieveMessages(action, completionList.toList(), QMailRetrievalAction::Content);
            return;
        } else {
            completionList.clear();
        }
    }

    completionAttempted = false;
    emit client->retrievalCompleted(action);
}

QMap<QMailMessage::MessageType, QString> typeSignatureInit()
{
    QMap<QMailMessage::MessageType, QString> map;

    map.insert(QMailMessage::Sms, "newSmsCount(int)");
    map.insert(QMailMessage::Mms, "newMmsCount(int)");
    map.insert(QMailMessage::Email, "newEmailCount(int)");
    map.insert(QMailMessage::Instant, "newInstantCount(int)");
    map.insert(QMailMessage::System, "newSystemCount(int)");

    return map;
}

void MessageServer::acknowledgeNewMessages(const QMailMessageTypeList& types)
{
    foreach (QMailMessage::MessageType type, types) {
        // No messages of this type are new any longer
        QMailMessageKey newMessages(QMailMessageKey::messageType(type));
        newMessages &= QMailMessageKey(QMailMessageKey::status(QMailMessage::New, QMailDataComparator::Includes));
        QMailStore::instance()->updateMessagesMetaData(newMessages, QMailMessage::New, false);

        if (messageCounts[type] != 0) {
            newMessageTotal -= messageCounts[type];

            messageCounts[type] = 0;
            NewCountNotifier::notify(type, 0);
        }
    }
}

static QMap<QMailMessage::MessageType, QString> typeServiceInit()
{
    QMap<QMailMessage::MessageType, QString> map;

    map.insert(QMailMessage::Sms, "NewSmsArrival");
    map.insert(QMailMessage::Mms, "NewMmsArrival");
    map.insert(QMailMessage::Email, "NewEmailArrival");
    map.insert(QMailMessage::Instant, "NewInstantMessageArrival");
    map.insert(QMailMessage::System, "NewSystemMessageArrival");

    return map;
}

QString serviceForType(QMailMessage::MessageType type)
{
    static QMap<QMailMessage::MessageType, QString> typeService(typeServiceInit());
    return typeService[type];
}

int MessageServer::newMessageCount(QMailMessage::MessageType type) const
{
    QMailMessageKey newMessageKey(QMailMessageKey::status(QMailMessage::New, QMailDataComparator::Includes));
    if (type != QMailMessage::AnyType) {
        newMessageKey &= QMailMessageKey::messageType(type);
    }

    return QMailStore::instance()->countMessages(newMessageKey);
}

void MessageServer::reportNewCounts()
{
    static QMap<QMailMessage::MessageType, QString> typeSignature(typeSignatureInit());

    QMailMessageCountMap newCounts;
    foreach (QMailMessage::MessageType type, typeSignature.keys()) {
        newCounts[type] = newMessageCount(type);
    }

    newMessageTotal = newMessageCount(QMailMessage::AnyType);

    if (newMessageTotal) {
        // Inform QPE of changes to the new message counts
        foreach (QMailMessage::MessageType type, typeSignature.keys()) {
            if ((newCounts[type] > 0) && (newCounts[type] != messageCounts[type]))
               NewCountNotifier::notify(type, newCounts[type]);
        }

        // Request handling of the new message events
        QMailMessageCountMap::const_iterator it = newCounts.begin(), end = newCounts.end();
        for ( ; it != end; ++it) {
            QMailMessage::MessageType type(it.key());
            if (it.value() != messageCounts[type]) {
                // This type's count has changed since last reported

                if( NewCountNotifier* action = new NewCountNotifier(type,it.value())) {
                    actionType[action] = type;

                    connect(action, SIGNAL(response(bool)), this, SLOT(response(bool)));
                    connect(action, SIGNAL(error(QString)), this, SLOT(error(QString)));

                    // Ensure the client receives any generated events before the arrival notification
                    QMailStore::instance()->flushIpcNotifications();
                    if (!action->notify())
                        qWarning() << "Unable to invoke service:" << serviceForType(type);
                }
            }
        }
    }

    messageCounts = newCounts;
}

void MessageServer::response(bool handled)
{
    if (NewCountNotifier* action = static_cast<NewCountNotifier*>(sender())) {
        if (handled) {
            acknowledgeNewMessages(QMailMessageTypeList() << actionType[action]);
        }
        actionType.remove(action);
        action->deleteLater();
        if (actionType.isEmpty()) {
            // All outstanding handler events have been processed
            emit messageCountUpdated();
        }
    }
}

void MessageServer::error(const QString &message)
{
    if (NewCountNotifier* action = static_cast<NewCountNotifier*>(sender())) {
        qWarning() << "Unable to complete service:" << serviceForType(actionType[action]) << "-" << message;
        actionType.remove(action);
        action->deleteLater();
    }

    if (actionType.isEmpty()) {
        // No outstanding handler events remain
        emit messageCountUpdated();
    }
}

void MessageServer::transmissionCompleted(quint64 action)
{
    // Ensure the client receives any resulting events before the completion notification
    QMailStore::instance()->flushIpcNotifications();

    emit client->transmissionCompleted(action);
}

void MessageServer::messagesTransmitted(quint64 action, const QMailMessageIdList &ids)
{
    // Mark these messages as having been sent
    QMailStore::instance()->updateMessagesMetaData(QMailMessageKey::id(ids), QMailMessage::Sent, true);

    emit client->messagesTransmitted(action, ids);
}

void MessageServer::messagesAdded(const QMailMessageIdList &ids)
{
    if (!QMailStore::instance()->asynchronousEmission()) {
        // Added in our process - from retrieval
        foreach (const QMailMessageId &id, ids) {
            QMailMessageMetaData message(id);

            bool complete(false);
            if (!(message.status() & QMailMessage::Downloaded)) {
                // Automatically download voicemail messages
                if (message.content() == QMailMessage::VoicemailContent ||
                    message.content() == QMailMessage::VideomailContent) {
                    complete = true;
                }
            }

            if (complete)
                completionList.insert(message.id());
        }
    }
}

void MessageServer::messagesUpdated(const QMailMessageIdList &ids)
{
    if (QMailStore::instance()->asynchronousEmission()) {
        // Only need to check message counts if the update occurred in another process
        updateNewMessageCounts();
    } else {
        // If we're updating, check whether the messages have been marked as Removed
        foreach (const QMailMessageId &id, ids) {
            if (completionList.contains(id)) {
                QMailMessageMetaData message(id);
                if ((message.status() & QMailMessage::Downloaded) || (message.status() & QMailMessage::Removed)) {
                    // This message has been completed (or removed)
                    completionList.remove(id);
                }
            }
        }
    }
}

void MessageServer::messagesRemoved(const QMailMessageIdList &ids)
{
    foreach (const QMailMessageId &id, ids) {
        // No need to complete deleted messages
        completionList.remove(id);
    }

    updateNewMessageCounts();
}

void MessageServer::updateNewMessageCounts()
{
    int newTotal = newMessageCount(QMailMessage::AnyType);
    if (newTotal != newMessageTotal) {
        // The number of messages marked as new has changed, but not via a message arrival event
        static QMap<QMailMessage::MessageType, QString> typeSignature(typeSignatureInit());

        // Update the individual counts
        foreach (QMailMessage::MessageType type, typeSignature.keys()) {
            int count(newMessageCount(type));
            if (count != messageCounts[type]) {
                messageCounts[type] = count;
                NewCountNotifier::notify(type, count);
            }
        }

        emit messageCountUpdated();
    }
}

