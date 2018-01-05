#ifndef EPOLLWATCHER_H
#define EPOLLWATCHER_H
#include <sys/epoll.h>
#include <Common/common.h>
#include <thread>
#include <atomic>
#include <Common/timer.h>
#include <Concurrent/concurrent_queue.h>

struct Connector{
    //[(userid|groupid) | msgtype | headerlen | msgheader | protobufferlen | protobuffers]
    std::string payload;

    union HANDLE{
        HANDLE_T connfd;
        struct UDPHANDLE{
            uint32_t id;
            uint32_t conv;
        }udphandle;
    }handle;
//    uint64_t mTimerHandler;
    Connector(/*uint64_t tHandler,*/const std::string& payload, HANDLE_T fd, uint32_t conv = 0) :
/*        mTimerHandler(tHandler)*/payload(payload){
        handle.udphandle.id = fd;
        handle.udphandle.conv = conv;
    }

    ~Connector(){
    }
};

class EpollWatcher
{
public:
    EpollWatcher();

    EpollWatcher(EpollWatcher&&) = default;

    ~EpollWatcher();

    virtual void Start();

    virtual bool TryPop(Connector* conn);
protected:
    virtual void HandleInput(HANDLE_T connfd = 0) = 0;
    virtual void HandleWrite() = 0;
    virtual void HandleNewConnection() = 0;

    virtual int MaxEvents();

    virtual int WaitTimeout();

    void EventAdd(int op, HANDLE_T fd);

    virtual inline HANDLE_T Getfd();

private:
    void Loop();

    void ProcessTimeout();

    void WatcherLoop();

protected:
    HANDLE_T serverfd;
    Concurrent::concurrent_queue<Connector*> mConnectorsQue;
    Timer mConnTimer;
    std::function<void(HANDLE_T connfd)> ConnectTimeoutCB;
    std::function<void(HANDLE_T connfd, uint64_t timerhandle)> NewConnectorCB;
private:
    HANDLE_T epfd;
    epoll_event * events;

    std::thread mThreadHandle;
    std::atomic_bool mRunning;
};

#endif // EPOLLWATCHER_H
