/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the Qt Messaging Framework.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the either Technology Preview License Agreement or the
** Beta Release License Agreement.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef EMAILCLIENT_H
#define EMAILCLIENT_H

#include "messagelistview.h"
#include <qmailmessageserver.h>
#include <qmailserviceaction.h>
#include <qmailviewer.h>
#include <QList>
#include <QMainWindow>
#include <QStack>
#include <QTime>
#include <QTimer>
#include <QProcess>

class MessageFolder;
class MessageStore;
class EmailFolderModel;
class EmailFolderView;
class SearchView;
class UILocation;
class ReadMail;
class WriteMail;
class SearchProgressDialog;
class QAction;
class QMailAccount;
class QMailMessageSet;
class QMailRetrievalAction;
class QMailSearchAction;
class QMailTransmitAction;
class QMailStorageAction;
class QStackedWidget;
class QStringList;
class QToolBar;

class MessageUiBase : public QMainWindow
{
    Q_OBJECT

public:
    MessageUiBase(QWidget *parent = 0, Qt::WindowFlags f = 0);
    virtual ~MessageUiBase() {}

signals:
    void statusVisible(bool);
    void updateStatus(const QString&);
    void updateProgress(uint, uint);
    void clearStatus();

protected:
    void viewSearchResults(const QMailMessageKey& filter, const QString& title = QString());
    void viewComposer();

protected:
    WriteMail* writeMailWidget() const;
    ReadMail* readMailWidget() const;
    EmailFolderView* folderView() const;
    MessageListView* messageListView() const;
    MessageStore* messageStore() const;
    EmailFolderModel* emailFolderModel() const;

    void suspendMailCounts();
    void resumeMailCounts();
    virtual void contextStatusUpdate();
    virtual void showFolderStatus(QMailMessageSet* item);
    virtual void setMarkingMode(bool set);
    virtual void clearStatusText();

    virtual WriteMail* createWriteMailWidget();
    virtual ReadMail* createReadMailWidget();
    virtual EmailFolderView* createFolderView();
    virtual MessageListView* createMessageListView();
    virtual MessageStore* createMessageStore();
    virtual EmailFolderModel* createEmailFolderModel();

protected slots:
    virtual void messageSelectionChanged();
    virtual void presentMessage(const QMailMessageId &, QMailViewerFactory::PresentationType);

    // Slots to be implemented by subclass
    virtual void folderSelected(QMailMessageSet*) = 0;
    virtual void composeActivated() = 0;
    virtual void emptyTrashFolder() = 0;
    virtual void messageActivated() = 0;
    virtual void allWindowsClosed() = 0;

protected:
    QString appTitle;
    bool suspendMailCount;
    bool markingMode;
    QMailMessageId selectedMessageId;
    int selectionCount;
    bool emailCountSuspended;
};


class EmailClient : public MessageUiBase
{
    Q_OBJECT

public:
    EmailClient(QWidget *parent = 0, Qt::WindowFlags f = 0);
    ~EmailClient();

    bool cleanExit(bool force);
    bool closeImmediately();

    void setVisible(bool visible);

public slots:
    void sendMessageTo(const QMailAddress &address, QMailMessage::MessageType type);
    void quit();
    void closeEvent(QCloseEvent *e);

protected:
    void setupUi();
    void showEvent(QShowEvent* e);

protected slots:
    void cancelOperation();

    void noSendAccount(QMailMessage::MessageType);
    void enqueueMail(const QMailMessage&);
    void saveAsDraft(const QMailMessage&);
    void discardMail();

    void sendAllQueuedMail(bool userRequest = false);
    void sendSingleMail(QMailMessageMetaData& message);

    void getSingleMail(const QMailMessageMetaData& message);
    void retrieveMessagePortion(const QMailMessageMetaData& message, uint bytes);
    void retrieveMessagePart(const QMailMessagePart::Location& partLocation);
    void retrieveMessagePartPortion(const QMailMessagePart::Location& partLocation, uint bytes);
    void messageActivated();
    void emptyTrashFolder();

    void moveSelectedMessages();
    void copySelectedMessages();
    void restoreSelectedMessages();
    void deleteSelectedMessages();

    void moveSelectedMessagesTo(MessageFolder* destination);
    void copySelectedMessagesTo(MessageFolder* destination);

    void selectAll();

    void detachThread();

    void connectivityChanged(QMailServiceAction::Connectivity connectivity);
    void activityChanged(QMailServiceAction::Activity activity);
    void statusChanged(const QMailServiceAction::Status &status);
    void progressChanged(uint progress, uint total);

    void flagRetrievalActivityChanged(QMailServiceAction::Activity activity);

    void transmitCompleted();
    void retrievalCompleted();
    void storageActionCompleted();
    void searchCompleted();

    void folderSelected(QMailMessageSet*);

    void composeActivated();

    void getAllNewMail();
    void getAccountMail();
    void getNewMail();

    void synchronizeFolder();

    void updateAccounts();

    void transferFailure(const QMailAccountId& accountId, const QString&, int);
    void storageActionFailure(const QMailAccountId& accountId, const QString&);

    void setStatusText(QString &);

    void search();
    void searchRequested();

    void automaticFetch();

    void externalEdit(const QString &);

    void respond(const QMailMessage& message, QMailMessage::ResponseType type);
    void respond(const QMailMessagePart::Location& partLocation, QMailMessage::ResponseType type);
    void modify(const QMailMessage& message);

    void retrieveMoreMessages();
    void retrieveVisibleMessagesFlags();

    bool removeMessage(const QMailMessageId& id, bool userRequest);

    void readReplyRequested(const QMailMessageMetaData&);

    void settings();

    void accountsAdded(const QMailAccountIdList& ids);
    void accountsRemoved(const QMailAccountIdList& ids);
    void accountsUpdated(const QMailAccountIdList& ids);

    void messagesUpdated(const QMailMessageIdList& ids);

    void messageSelectionChanged();

    void allWindowsClosed();

private slots:
    void delayedInit();
    void openFiles();
    void initActions();
    void updateActions();
    void markMessages();
    void resumeInterruptedComposition();
    bool startMessageServer();
    bool waitForMessageServer();
    void messageServerProcessError(QProcess::ProcessError);

private:
    bool isMessageServerRunning() const;
    virtual EmailFolderView* createFolderView();
    virtual MessageListView* createMessageListView();

    void init();
    void userInvocation();

    void mailResponded();

    void closeAfterTransmissionsFinished();
    bool isTransmitting();
    bool isSending();
    bool isRetrieving();

    bool checkMailConflict(const QString& msg1, const QString& msg2);
    void setNewMessageCount(QMailMessage::MessageType type, uint);

    void readSettings();
    bool saveSettings();

    void displayCachedMail();

    void accessError(const MessageFolder &box);
    void copyError(const MessageFolder &dest);
    void moveError(const MessageFolder &dest);

    void getNextNewMail();
    bool verifyAccount(const QMailAccountId &accountId, bool outgoing);

    void setSendingInProgress(bool y);
    void setRetrievalInProgress(bool y);

    void transferStatusUpdate(int status);
    void setSuspendPermitted(bool y);
    void clearOutboxFolder();

    void updateGetMailButton();
    void updateGetAccountButton();

    QString mailType(QMailMessage::MessageType type);

    void setActionVisible(QAction*, bool);

    bool restoreMessages(const QMailMessageIdList& ids, MessageFolder*);
    bool deleteMessages(const QMailMessageIdList& ids, MessageFolder*);

    void contextStatusUpdate();

    void setMarkingMode(bool set);

    MessageFolder* containingFolder(const QMailMessageId& id);

    bool applyToSelectedFolder(void (EmailClient::*function)(MessageFolder*));

    void sendFailure(const QMailAccountId &accountId);
    void receiveFailure(const QMailAccountId &accountId);

    void closeApplication();

    QMailAccountIdList emailAccounts() const;

    SearchProgressDialog* searchProgressDialog() const;

    void clearNewMessageStatus(const QMailMessageKey& key);

private:
    // Whether the initial action for the app was to view incoming messages 
    enum InitialAction { None = 0, IncomingMessages, NewMessages, View, Compose, Cleanup };
    enum SyncState { ExportUpdates = 0, RetrieveFolders, RetrieveMessages };

    bool filesRead;
    QMailMessageId cachedDisplayMailId;

    int transferStatus;
    int primaryActivity;

    uint totalSize;

    QMailAccountId mailAccountId;

    QAction *getMailButton;
    QAction *getAccountButton;
    QAction *composeButton;
    QAction *searchButton;
    QAction *cancelButton;
    QAction *synchronizeAction;
    QAction *settingsAction;
    QAction *emptyTrashAction;
    QAction *deleteMailAction;
    QAction *detachThreadAction;
    QAction *markAction;
    bool enableMessageActions;

    QAction *moveAction;
    QAction *copyAction;
    QAction *restoreAction;
    QAction *selectAllAction;
    bool closeAfterTransmissions;
    bool closeAfterWrite;

    QTimer fetchTimer;
    bool autoGetMail;

    QMailMessageId repliedFromMailId;
    quint64 repliedFlags;

    QList<int> queuedAccountIds;

    InitialAction initialAction;

    QMap<QAction*, bool> actionVisibility;

    SearchView *searchView;
    int preSearchWidgetId;

    QSet<QMailFolderId> locationSet;

    QMailAccountIdList transmitAccountIds;
    QMailAccountIdList retrievalAccountIds;

    QMailMessageId lastDraftId;

    QMailTransmitAction *transmitAction;
    QMailRetrievalAction *retrievalAction;
    QMailStorageAction *storageAction;
    QMailSearchAction *searchAction;

    QProcess* m_messageServerProcess;

    SyncState syncState;

    QMailRetrievalAction *flagRetrievalAction;
    QSet<QMailMessageId> flagMessageIds;
    QMenu* m_contextMenu;
    QToolBar* m_toolBar;
};

#endif
