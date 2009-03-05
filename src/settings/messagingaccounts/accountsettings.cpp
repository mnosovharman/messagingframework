/****************************************************************************
**
** This file is part of the $PACKAGE_NAME$.
**
** Copyright (C) $THISYEAR$ $COMPANY_NAME$.
**
** $QT_EXTENDED_DUAL_LICENSE$
**
****************************************************************************/

#include "accountsettings.h"
#include "editaccount.h"
#include "statusdisplay.h"
#include <qmaillog.h>
#include <QAction>
#include <QLayout>
#include <QMouseEvent>
#include <qmailaccount.h>
#include <qmailaccountlistmodel.h>
#include <qmailstore.h>
#include <QMenu>
#include <QMessageBox>
#include <QPainter>
#include <QPushButton>
#include <QTextDocument>
#include <QTimer>
#include <qmailipc.h>
#include <QItemDelegate>
#include <QListView>
#include <QMenuBar>
#include <QApplication>

AccountSettings::AccountSettings(QWidget *parent, Qt::WFlags flags)
    : QDialog(parent, flags),
      preExisting(false),
      deleteBatchSize(0),
      deleteProgress(0)
{
    setWindowTitle(tr("Messaging Accounts"));
    QVBoxLayout *vb = new QVBoxLayout(this);
    vb->setContentsMargins(0, 0, 0, 0);

    QMenuBar* menu = new QMenuBar(this);
    context = menu->addMenu("File");
    vb->addWidget(menu);

    accountModel = new QMailAccountListModel();
    accountModel->setKey(QMailAccountKey::status(QMailAccount::UserEditable, QMailDataComparator::Includes));
    accountModel->setSortKey(QMailAccountSortKey::id(Qt::AscendingOrder));
    accountView = new QListView(this);

    accountView->setModel(accountModel);

    if (accountModel->rowCount())
        accountView->setCurrentIndex(accountModel->index(0, 0));
    vb->addWidget(accountView);

    addAccountAction = new QAction( QIcon(":icon/new"), tr("Add account..."), this );
    connect(addAccountAction, SIGNAL(triggered()), this, SLOT(addAccount()));
    context->addAction( addAccountAction );
    removeAccountAction = new QAction( QIcon(":icon/trash"), tr("Remove account..."), this );
    connect(removeAccountAction, SIGNAL(triggered()), this, SLOT(removeAccount()));
    context->addAction(removeAccountAction);

    QAction* exitAction = new QAction(tr("Quit"), this );
    connect(exitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    context->addAction(exitAction);

    statusDisplay = new StatusDisplay(this);
    statusDisplay->setVisible(false);
    vb->addWidget(statusDisplay);
    connect(accountView, SIGNAL(activated(QModelIndex)),
	    this, SLOT(accountSelected(QModelIndex)) );
    connect(context, SIGNAL(aboutToShow()), 
	    this, SLOT(updateActions()) );

    retrievalAction = new QMailRetrievalAction(this);
    connect(retrievalAction, SIGNAL(activityChanged(QMailServiceAction::Activity)), 
            this, SLOT(activityChanged(QMailServiceAction::Activity)));
    connect(retrievalAction, SIGNAL(progressChanged(uint, uint)), 
            this, SLOT(displayProgress(uint, uint)));
}

void AccountSettings::addAccount()
{
    QMailAccount newAccount;
    editAccount(&newAccount);
}

void AccountSettings::showEvent(QShowEvent* e)
{
    accountModel->setSynchronizeEnabled(true);
    QDialog::showEvent(e);
}

void AccountSettings::hideEvent(QHideEvent* e)
{
    accountModel->setSynchronizeEnabled(false);
    QDialog::hideEvent(e);
}

void AccountSettings::removeAccount()
{
    QModelIndex index = accountView->currentIndex();
    if (!index.isValid())
      return;

    QMailAccount account(accountModel->idFromIndex(index));

    QString message = tr("<qt>Delete account:\n%1</qt>").arg(Qt::escape(account.name()));
    if (QMessageBox::warning( this, tr("Email"), message, tr("Yes"), tr("No"), 0, 0, 1 ) == 0) {
        // We could simply delete the account since QMailStore::deleteAccount
        // will remove all folders and messages, but for now we will remove the
        // messages manually so we can give progress indication (eventually, we
        // might add progress notification to QMailStore)

        // Number of messages required before we use a progress indicator
        static const int MinimumForProgressIndicator = 20;

        // Maximum messages processed per batch operation
        static const int MaxBatchSize = 50;
        static const int BatchMinimumForProgressIndicator = 2 * MaxBatchSize + 1;

        // Remove the messages and folders from this account (not just in the Inbox)
        QMailMessageKey messageKey(QMailMessageKey::parentAccountId(account.id()));
        const int count = QMailStore::instance()->countMessages(messageKey);
        if (count >= MinimumForProgressIndicator) {
            deleteMessageIds = QMailStore::instance()->queryMessages(messageKey);
            deleteProgress = 0;

            deleteBatchSize = 1;
            if (count >= BatchMinimumForProgressIndicator) {
                // Process this list in batches of roughly equivalent size
                int batchCount = (count / MaxBatchSize) + (count % MaxBatchSize ? 1 : 0);
                deleteBatchSize = ((count / batchCount) + (count % batchCount ? 1 : 0));
            }

            statusDisplay->displayProgress(0, count);
            statusDisplay->displayStatus(tr("Deleting messages"));
        } else {
            // No progress indication is required - allow the messages to be removed in account deletion
            deleteMessageIds = QMailMessageIdList();
        }

        deleteAccountId = account.id();
        QTimer::singleShot(0, this, SLOT(deleteMessages()));
    }
}

void AccountSettings::deleteMessages()
{
    if (!deleteMessageIds.isEmpty()) {
        // Process the next batch
        QMailMessageIdList batch(deleteMessageIds.mid(0, deleteBatchSize));
        deleteMessageIds = deleteMessageIds.mid(deleteBatchSize);

        QMailStore::instance()->removeMessages(QMailMessageKey::id(batch), QMailStore::CreateRemovalRecord);
        deleteProgress += batch.count();
        
        statusDisplay->displayProgress(deleteProgress, deleteProgress + deleteMessageIds.count());
        QTimer::singleShot(0, this, SLOT(deleteMessages()));
    } else {
        // Remove the account now
        QMailStore::instance()->removeAccount(deleteAccountId);

        statusDisplay->setVisible(false);
    }
}

void AccountSettings::accountSelected(QModelIndex index)
{
    if (!index.isValid())
      return;

    QMailAccount account(accountModel->idFromIndex(index));

    if (account.messageType() != QMailMessage::Sms)
        editAccount(&account);
}

void AccountSettings::updateActions()
{
    QModelIndex index = accountView->currentIndex();
    if (!index.isValid())
        return;

    QMailAccount account(accountModel->idFromIndex(index));
    removeAccountAction->setVisible(account.status() & QMailAccount::UserRemovable);
}

void AccountSettings::editAccount(QMailAccount *account)
{
    QMailAccountConfiguration config;
    if (account->id().isValid()) {
        config = QMailAccountConfiguration(account->id());
    } else {
        account->setStatus(QMailAccount::UserEditable, true);
        account->setStatus(QMailAccount::UserRemovable, true);
    }

    QDialog *editAccountView;
    bool wasPreferred(account->status() & QMailAccount::PreferredSender);

    EditAccount *e = new EditAccount(this, "EditAccount");
    e->setAccount(account, &config);
    editAccountView = e;

    editAccountView->setMinimumSize(QSize(400,400));
    int ret = editAccountView->exec();

    delete editAccountView;

    if (ret == QDialog::Accepted) {
        QMailAccountId previousPreferredId;
        if ((account->status() & QMailAccount::PreferredSender) && !wasPreferred) {
            // This account is now preferred - see if there is a predecessor that must be deselected
            QMailAccountKey preferredKey(QMailAccountKey::status(QMailAccount::PreferredSender, QMailDataComparator::Includes));
            QMailAccountKey typeKey(QMailAccountKey::messageType(account->messageType()));

            QMailAccountIdList previousIds = QMailStore::instance()->queryAccounts(preferredKey & typeKey);
            if (!previousIds.isEmpty())
                previousPreferredId = previousIds.first();
        }

        preExisting = account->id().isValid();
        if (preExisting) {
            QMailStore::instance()->updateAccount(account, &config);
        } else {
            QMailStore::instance()->addAccount(account, &config);
            accountView->setCurrentIndex(accountModel->index(accountModel->rowCount() - 1, 0));
        }

        if ((account->status() & QMailAccount::PreferredSender) && !wasPreferred) {
            if (previousPreferredId.isValid()) {
                QMailAccount previousAccount(previousPreferredId);
                previousAccount.setStatus(QMailAccount::PreferredSender, false);
                QMailStore::instance()->updateAccount(&previousAccount);

                QMessageBox::warning(qApp->activeWindow(),
                                     tr("New default account"),
                                     tr("<qt>Your previous default mail account has been unchecked</qt>"),
                                     QMessageBox::Ok);
            }
        }

        if (config.services().contains("imap4", Qt::CaseInsensitive)) {
            QTimer::singleShot(0, this, SLOT(retrieveFolders()));
        }
    }
}

void AccountSettings::retrieveFolders()
{
    QModelIndex index(accountView->currentIndex());
    if (index.isValid()) {
        QMailAccountId id(accountModel->idFromIndex(index));
        
        // See if the user wants to retrieve the folders for this account
        if (QMessageBox::question(qApp->activeWindow(),
                                  preExisting ? tr("Account Modified") : tr("Account Added"),
                                  tr("Do you wish to retrieve the folder structure for this account?"),
                                  QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
            statusDisplay->setVisible(true);
            statusDisplay->displayStatus(tr("Retrieving folders..."));

            retrievalAction->retrieveFolderList(id, QMailFolderId(), true);
        }
    }
}

void AccountSettings::activityChanged(QMailServiceAction::Activity activity)
{
    if (activity == QMailServiceAction::Successful) {
        statusDisplay->displayStatus(tr("Folders retrieved"));
    } else if (activity == QMailServiceAction::Failed) {
        QString caption(tr("Retrieve Failure"));
        QString action(tr("%1 - Error retrieving folders: %2", "%1: account name, %2: error text"));

        const QMailServiceAction::Status status(retrievalAction->status());
        QMailAccount account(status.accountId);
        action = action.arg(account.name()).arg(status.text);

        qMailLog(Messaging) << "retrieveFolders failed:" << action;
        statusDisplay->setVisible(false);
        QMessageBox::warning(0, caption, action, QMessageBox::Ok);
    }
}

void AccountSettings::displayProgress(uint value, uint range)
{
    if (statusDisplay->isVisible()) {
        statusDisplay->displayProgress(value, range);
    }
}

