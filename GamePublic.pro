TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt



QMAKE_CXXFLAGS += -std=c++14

QMAKE_LINK += -pthread



LIBS += -L/usr/lib -levent  -lprotobuf -ljemalloc -L/usr/local/lib -lbsoncxx -lmongocxx -lm -lssl -lcrypto -levent_openssl

INCLUDEPATH += /usr/include:/usr/local/include

HEADERS += \
    Common/common.h \
    Common/commonmacros.h \
    Common/csvreader.h \
    Common/inireader.h \
    Common/memorypool.h \
    Common/singleton.h \
    Common/spinlock.h \
    Common/threadpool.h \
    Common/timer.h \
    Common/logmanager.h \
    Concurrent/concurrent_deque.h \
    Concurrent/concurrent_queue.h \
    Concurrent/concurrent_unordered_map.h \
    Thirdparty/hiredis/async.h \
    Thirdparty/hiredis/dict.h \
    Thirdparty/hiredis/fmacros.h \
    Thirdparty/hiredis/hiredis.h \
    Thirdparty/hiredis/net.h \
    Thirdparty/hiredis/read.h \
    Thirdparty/hiredis/sds.h \
    Thirdparty/hiredis/sdsalloc.h \
    Thirdparty/hiredis/win32.h \
    Common/redisconnector.h \
    Common/epollwatcher.h \
    Concurrent/concurrent_priority_queue.h \
    Common/eventhandler.h \
    Common/udpwatcher.h \
    KCP/ThirdParty/ikcp.h \
    KCP/connection.h \
    KCP/connectionmanager.h \
    KCP/connnectioncontainer.h \
    KCP/kcptype.h \
    Common/statics.h \
    Common/payloadhandler.h

SOURCES += \
    Common/csvreader.cpp \
    Common/inireader.cpp \
    Common/threadpool.cpp \
    Common/timer.cpp \
    Common/logmanager.cpp \
    Thirdparty/hiredis/async.c \
    Thirdparty/hiredis/dict.c \
    Thirdparty/hiredis/hiredis.c \
    Thirdparty/hiredis/net.c \
    Thirdparty/hiredis/read.c \
    Thirdparty/hiredis/sds.c \
    Common/redisconnector.cpp \
    Common/epollwatcher.cpp \
    Common/common.cpp \
    Common/udpwatcher.cpp \
    KCP/connection.cpp \
    KCP/connectionmanager.cpp \
    KCP/connnectioncontainer.cpp \
    KCP/kcptype.cpp \
    KCP/ThirdParty/ikcp.c \
    PublicTestMain.cpp


