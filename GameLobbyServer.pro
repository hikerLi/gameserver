TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    Common/common.cpp \
    Common/csvreader.cpp \
    Common/epollwatcher.cpp \
    Common/inireader.cpp \
    Common/logmanager.cpp \
    Common/redisconnector.cpp \
    Common/threadpool.cpp \
    Common/timer.cpp \
    KCP/connection.cpp \
    KCP/connectionmanager.cpp \
    KCP/connnectioncontainer.cpp \
    KCP/kcptype.cpp \
    KCP/ThirdParty/ikcp.c \
    Thirdparty/hiredis/async.c \
    Thirdparty/hiredis/dict.c \
    Thirdparty/hiredis/hiredis.c \
    Thirdparty/hiredis/net.c \
    Thirdparty/hiredis/read.c \
    Thirdparty/hiredis/sds.c \
    Thirdparty/hiredis/test.c \
    LobbyMain.cpp \
    Lobby/lobbypayloadhandler.cpp \
    Lobby/lobbywatcher.cpp

HEADERS += \
    Common/common.h \
    Common/commonmacros.h \
    Common/csvreader.h \
    Common/epollwatcher.h \
    Common/eventhandler.h \
    Common/inireader.h \
    Common/logmanager.h \
    Common/memorypool.h \
    Common/payloadhandler.h \
    Common/redisconnector.h \
    Common/singleton.h \
    Common/spinlock.h \
    Common/threadpool.h \
    Common/timer.h \
    Concurrent/concurrent_deque.h \
    Concurrent/concurrent_priority_queue.h \
    Concurrent/concurrent_queue.h \
    Concurrent/concurrent_unordered_map.h \
    KCP/ThirdParty/ikcp.h \
    KCP/connection.h \
    KCP/connectionmanager.h \
    KCP/connnectioncontainer.h \
    KCP/kcptype.h \
    Thirdparty/hiredis/adapters/ae.h \
    Thirdparty/hiredis/adapters/glib.h \
    Thirdparty/hiredis/adapters/ivykis.h \
    Thirdparty/hiredis/adapters/libev.h \
    Thirdparty/hiredis/adapters/libevent.h \
    Thirdparty/hiredis/adapters/libuv.h \
    Thirdparty/hiredis/adapters/macosx.h \
    Thirdparty/hiredis/adapters/qt.h \
    Thirdparty/hiredis/async.h \
    Thirdparty/hiredis/dict.h \
    Thirdparty/hiredis/fmacros.h \
    Thirdparty/hiredis/hiredis.h \
    Thirdparty/hiredis/net.h \
    Thirdparty/hiredis/read.h \
    Thirdparty/hiredis/sds.h \
    Thirdparty/hiredis/sdsalloc.h \
    Thirdparty/hiredis/win32.h \
    msgtype.h \
    Lobby/lobbypayloadhandler.h \
    Lobby/lobbywatcher.h \
    Lobby/statics.h
