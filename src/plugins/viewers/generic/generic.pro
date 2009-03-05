TEMPLATE = lib 

TARGET = genericviewer 
target.path += $$QMF_INSTALL_ROOT/plugins/viewers
INSTALLS += target

DEPENDPATH += .

INCLUDEPATH += . ../../../libraries/qmfutil \
               ../../../libraries/qtopiamail \
               ../../../libraries/qtopiamail/support

LIBS += -L../../../libraries/qtopiamail -lqtopiamail \
        -L../../../libraries/qmfutil -lqmfutil

HEADERS += browser_qt.h genericviewer_qt.h

SOURCES += browser_qt.cpp genericviewer_qt.cpp

TRANSLATIONS += libgenericviewer-ar.ts \
                libgenericviewer-de.ts \
                libgenericviewer-en_GB.ts \
                libgenericviewer-en_SU.ts \
                libgenericviewer-en_US.ts \
                libgenericviewer-es.ts \
                libgenericviewer-fr.ts \
                libgenericviewer-it.ts \
                libgenericviewer-ja.ts \
                libgenericviewer-ko.ts \
                libgenericviewer-pt_BR.ts \
                libgenericviewer-zh_CN.ts \
                libgenericviewer-zh_TW.ts
