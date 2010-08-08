/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Messaging Framework.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef SMTPCONFIGURATION_H
#define SMTPCONFIGURATION_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt Extended API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <qmailserviceconfiguration.h>

class PLUGIN_EXPORT SmtpConfiguration : public QMailServiceConfiguration
{
public:
    enum AuthType {
        Auth_NONE = 0,
#ifndef QT_NO_OPENSSL
        Auth_LOGIN = 1,
        Auth_PLAIN = 2,
#endif
        Auth_CRAMMD5 = 3,
        Auth_INCOMING = 4
    };

    explicit SmtpConfiguration(const QMailAccountConfiguration &config);
    explicit SmtpConfiguration(const QMailAccountConfiguration::ServiceConfiguration &svcCfg);

    QString userName() const;
    QString emailAddress() const;
    QString smtpServer() const;
    int smtpPort() const;
#ifndef QT_NO_OPENSSL
    QString smtpUsername() const;
    QString smtpPassword() const;
#endif
    int smtpAuthentication() const;
    int smtpEncryption() const;
};

class PLUGIN_EXPORT SmtpConfigurationEditor : public SmtpConfiguration
{
public:
    explicit SmtpConfigurationEditor(QMailAccountConfiguration *config);

    void setUserName(const QString &str);
    void setEmailAddress(const QString &str);
    void setSmtpServer(const QString &str);
    void setSmtpPort(int i);
#ifndef QT_NO_OPENSSL
    void setSmtpUsername(const QString& username);
    void setSmtpPassword(const QString& password);
#endif
#ifndef QT_NO_OPENSSL
    void setSmtpAuthentication(int t);
#endif
#ifndef QT_NO_OPENSSL
    void setSmtpEncryption(int t);
#endif
};

#endif
