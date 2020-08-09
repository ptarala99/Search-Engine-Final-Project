TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

LIBS +=  -lstdc++fs
SOURCES += \
    avltreeindex.cpp \
    doc.cpp \
    docparser.cpp \
    hashindex.cpp \
    indexinterface.cpp \
    main.cpp \
    porter2_stemmer.cpp \
    searcher.cpp \
    word.cpp

HEADERS += \
    AVLTree.h \
    avltreeindex.h \
    doc.h \
    docparser.h \
    hashindex.h \
    hashtable.h \
    indexinterface.h \
    json.hpp \
    porter2_stemmer.h \
    searcher.h \
    word.h

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../Downloads/myhtml-master/lib/release/ -lmyhtml
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../Downloads/myhtml-master/lib/debug/ -lmyhtml
else:unix: LIBS += -L$$PWD/../../../../Downloads/myhtml-master/lib/ -lmyhtml

INCLUDEPATH += $$PWD/../../../../Downloads/myhtml-master/include
DEPENDPATH += $$PWD/../../../../Downloads/myhtml-master/include

DISTFILES += \
    filePaths.txt \
    stemStops.txt

