/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt Messaging Framework.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QMFDATASESSION_H
#define QMFDATASESSION_H

#include <e32base.h>

// Constants
// Number of message slots to reserve for this client server session.
// In this example we can have one asynchronous request outstanding
// and one synchronous request in progress.
const TUint KDefaultMessageSlots = 2;

const TUid KServerUid3 = { 0x2003A67A }; // Server UID

_LIT(KQMFDataServerFilename, "QMFDataServer");

#ifdef __WINS__
static const TUint KServerMinHeapSize =  0x1000;  //  4K
static const TUint KServerMaxHeapSize = 0x10000;  // 64K
#endif

class RFile;

class RQMFDataSession : public RSessionBase
{
public:
    RQMFDataSession();
    TInt Connect();
    TVersion Version() const;
    TBool CreateDatabase(const TDesC& aDatabaseName);
    TBool OpenFile(RFile& aFile, const TDesC& aFilePath);
    TBool OpenOrCreateFile(RFile& aFile, const TDesC& aFilePath);
    TBool FileExists(const TDesC& aFilePath);
    TBool RemoveFile(const TDesC& aFilePath);
    TBool RenameFile(const TDesC& aOldFilePath, const TDesC& aNewFilePath);
    TBool DirectoryExists(const TDesC& aPath);
    TBool MakeDirectory(const TDesC& aPath);
    TBool RemoveDirectory(const TDesC& aPath);
    TBool RemovePath(const TDesC& aPath);
    TBool DirectoryListing(const TDesC& aPath, RBuf& aDirectoryListing);

private: // Data
    TPtr8 iTimeBuffer;
};

#endif // QMFDATASESSION

// End of File
