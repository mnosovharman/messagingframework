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

HEADERS += attachmentoptions.h browser.h genericviewer.h

SOURCES += attachmentoptions.cpp browser.cpp genericviewer.cpp

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
