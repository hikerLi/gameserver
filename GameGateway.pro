TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++14

QMAKE_LINK += -pthread

LIBS += -L/usr/lib -lprotobuf -ljemalloc -L/usr/local/lib -lbsoncxx -lmongocxx -lmysqlpp

SOURCES += \
    GatewayMain.cpp \
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
    Gateway/statics.cpp \
    Common/tcpwatcher.cpp \
    Common/udpwatcher.cpp \
    Common/payloadparser.cpp \
    Gateway/clientshandler.cpp \
    Gateway/servershandler.cpp \
    Common/mysqlconnector.cpp

HEADERS += \
    Common/common.h \
    Common/commonmacros.h \
    Common/csvreader.h \
    Common/epollwatcher.h \
    Common/inireader.h \
    Common/logmanager.h \
    Common/memorypool.h \
    Common/redisconnector.h \
    Common/singleton.h \
    Common/spinlock.h \
    Common/statics.h \
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
    Gateway/statics.h \
    Common/tcpwatcher.h \
    Common/udpwatcher.h \
    Common/payloadparser.h \
    Gateway/clientshandler.h \
    Gateway/servershandler.h \
    Concurrent/concurrent_vector.h \
    Common/mysqlconnector.h \
    Common/tasksmanager.h
