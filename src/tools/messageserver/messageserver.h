/****************************************************************************
**
** This file is part of the $PACKAGE_NAME$.
**
** Copyright (C) $THISYEAR$ $COMPANY_NAME$.
**
** $QT_EXTENDED_DUAL_LICENSE$
**
****************************************************************************/

#ifndef MESSAGESERVER_H
#define MESSAGESERVER_H

#ifdef QMAIL_QTOPIA
#include <QUniqueId>
#endif
#include <qmailmessageserver.h>
#include <QObject>
#include <QSet>

class ServiceHandler;
class MailMessageClient;
class QDSData;
class QMailMessageMetaData;
class QNetworkState;
class NewCountNotifier;

class MessageServer : public QObject
{
    Q_OBJECT

public:
    MessageServer(QObject *parent = 0);
    ~MessageServer();

signals:
    void messageCountUpdated();

private slots:
    void retrievalCompleted(quint64 action);

    void transmissionCompleted(quint64 action);
    void messagesTransmitted(quint64 action, const QMailMessageIdList &);

    void response(bool handled);
    void error(const QString &message);

    void messagesAdded(const QMailMessageIdList &ids);
    void messagesUpdated(const QMailMessageIdList &ids);
    void messagesRemoved(const QMailMessageIdList &ids);
    void reportNewCounts();
    void acknowledgeNewMessages(const QMailMessageTypeList&);

private:
    int newMessageCount(QMailMessage::MessageType type) const;

    void updateNewMessageCounts();

    ServiceHandler *handler;
    MailMessageClient *client;
    QMailMessageCountMap messageCounts;

#ifdef QMAIL_QTOPIA
    QtopiaIpcAdaptor messageCountUpdate;
#else
    QCopAdaptor messageCountUpdate;
#endif
    QMap<NewCountNotifier*, QMailMessage::MessageType> actionType;
    int newMessageTotal;

    QSet<QMailMessageId> completionList;
    bool completionAttempted;
};

#endif
