#include "tcpwatcher.h"

TcpWatcher::TcpWatcher()
{

}

void TcpWatcher::Start(uint16_t port, decltype(ConnectTimeoutCB) timeoutcb, decltype(NewConnectorCB) newconncb)
{
    Init(port);

    ConnectTimeoutCB = timeoutcb;

    NewConnectorCB = newconncb;

    EpollWatcher::Start();
}


void TcpWatcher::HandleInput(HANDLE_T connfd)
{
    std::string payload = Recv(connfd);
    Connector * conn = MemPoolIns.newElement<Connector>(std::move(payload), connfd);
    mConnectorsQue.push(conn);
}

void TcpWatcher::HandleWrite()
{

}

void TcpWatcher::HandleNewConnection()
{
    sockaddr_in addr;
    uint32_t addrlen = sizeof(addr);
    HANDLE_T connfd = ::accept(serverfd, (sockaddr*)&addr, &addrlen);
    if(-1 != connfd){
        RegisterConnector(connfd);
    }
}

void TcpWatcher::Send(HANDLE_T fd, const std::string &data)
{
    ::send(fd, data.c_str(), data.length(), 0);
}

int TcpWatcher::MaxEvents()
{

}

int TcpWatcher::WaitTimeout()
{

}

void TcpWatcher::Init(uint16_t port)
{
    serverfd = ::socket(PF_INET, SOCK_STREAM, 0);
    if(-1 == serverfd){
        LOG_ERR("create socket error, errno:<%d>", errno);
        return;
    }

    int status = ::fcntl(serverfd, F_GETFL, 0);
    ::fcntl(serverfd, F_SETFL, status | O_NONBLOCK);

    struct sockaddr_in sAddr;

    sAddr.sin_addr.s_addr = INADDR_ANY;
    sAddr.sin_family = AF_INET;
    sAddr.sin_port = htons(port);

    if(-1 == ::bind(serverfd, (struct sockaddr *)&sAddr, sizeof(sAddr))){
        LOG_ERR("bind socket error, fd <%d> errno <%d>", serverfd, errno);
        return;
    }

    if(-1 == ::listen(serverfd, SOMAXCONN)){
        LOG_ERR("listen error, fd <%d> errno <%d>", serverfd, errno);
        return;
    }
}

void TcpWatcher::RegisterConnector(HANDLE_T connfd)
{
    EventAdd(EPOLLIN | EPOLLOUT | EPOLLONESHOT, connfd);

    uint64_t mTimerHandler = mConnTimer.AddOnce(TCPTIMEOUT, [&](){
        ConnectTimeoutCB(connfd);
    });

    NewConnectorCB(connfd, mTimerHandler);
}

std::string TcpWatcher::Recv(HANDLE_T connfd)
{
    char tbuff[MAXPAYLOAD] = {0};
     uint32_t ret = 0;
    do{
        ret = ::recv(connfd, tbuff, MAXPAYLOAD, 0);
        if(0 == ret){
            LOG_ERR("disconnect. ");
        }
        if(errno == EAGAIN){
            break;
        }
    }while(ret > 0);
    return std::string(tbuff);
}
