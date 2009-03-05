/****************************************************************************
**
** This file is part of the $PACKAGE_NAME$.
**
** Copyright (C) $THISYEAR$ $COMPANY_NAME$.
**
** $QT_EXTENDED_DUAL_LICENSE$
**
****************************************************************************/

#include "messagenavigator.h"
#include <qtopiaapplication.h>

// Comment out this line to use a manual main() function.
// Ensure you also remove CONFIG+=qtopia_main from qbuild.pro if you do this.
#define USE_THE_MAIN_MACROS



#ifdef USE_THE_MAIN_MACROS

QTOPIA_ADD_APPLICATION(QTOPIA_TARGET, MessageNavigator)
QTOPIA_MAIN

#else

#ifdef SINGLE_EXEC
QTOPIA_ADD_APPLICATION(QTOPIA_TARGET, MessageNavigator)
#define MAIN_FUNC main_MessageNavigator
#else
#define MAIN_FUNC main
#endif

// This is the storage for the SXE key that uniquely identified this applicaiton.
// make will fail without this!
QSXE_APP_KEY

int MAIN_FUNC( int argc, char **argv )
{
    // This is required to load the SXE key into memory
    QSXE_SET_APP_KEY(argv[0]);

    QtopiaApplication a( argc, argv );

    // Set the preferred document system connection type
    QTOPIA_SET_DOCUMENT_SYSTEM_CONNECTION();

    MessageNavigator *mw = new MessageNavigator();
    a.setMainWidget(mw);
    if ( mw->metaObject()->indexOfSlot("setDocument(QString)") != -1 ) {
        a.showMainDocumentWidget();
    } else {
        a.showMainWidget();
    }
    int rv = a.exec();
    delete mw;
    return rv;
}

#endif

