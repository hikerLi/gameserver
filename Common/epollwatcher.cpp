#include "epollwatcher.h"
#include <Common/memorypool.h>


EpollWatcher::EpollWatcher()
{

}

EpollWatcher::~EpollWatcher()
{

}

void EpollWatcher::Start()
{
    epfd = ::epoll_create(1);

    events = MemPoolIns.newArray<epoll_event>(MaxEvents());

    EventAdd(EPOLLIN | EPOLLOUT | EPOLLONESHOT, Getfd());

    mRunning.store(true);
    mThreadHandle = std::thread([&](){
        this->Loop();
    });
}

bool EpollWatcher::TryPop(Connector *conn)
{
    return mConnectorsQue.try_pop(conn);
}

int EpollWatcher::MaxEvents()
{
    return 65535;
}

int EpollWatcher::WaitTimeout()
{
    return 1;
}

void EpollWatcher::Loop()
{
    while(mRunning){
        ProcessTimeout();
        WatcherLoop();
    }
}

void EpollWatcher::ProcessTimeout()
{
    mConnTimer.Process();
}

void EpollWatcher::WatcherLoop()
{
    int nfds = ::epoll_wait(epfd, events, MaxEvents(), WaitTimeout());
    if(-1 == nfds){
        LOG_ERR("epoll wait error no : <%d>", errno);
        return;
    }

    for(uint32_t index = 0; index < nfds; index++){
        if(events[index].data.fd == epfd){
            HandleNewConnection();
        }
        else if(events[index].events & EPOLLIN){
            HandleInput(events[index].data.fd);
        }
        else if(events[index].events & EPOLLOUT){
            HandleWrite();
        }
    }
}

void EpollWatcher::EventAdd(int op, HANDLE_T fd)
{
    struct epoll_event ev;
    if(op & EPOLLIN){
        ev.events |= EPOLLIN;
        if(op & EPOLLONESHOT){
            ev.events |= EPOLLONESHOT;
        }
    }
    if(op & EPOLLOUT){
        ev.events |= EPOLLOUT;
        if(op & EPOLLONESHOT){
            ev.events |= EPOLLONESHOT;
        }
    }

    if(ev.events){
        ev.events |= EPOLLET;
    }

    ev.data.fd = fd;

    if(-1 == ::epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev)){
        LOG_ERR("epoll add failed.");
    }
}

HANDLE_T EpollWatcher::Getfd()
{
    return serverfd;
}
