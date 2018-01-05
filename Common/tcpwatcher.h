#ifndef TCPWATCHER_H
#define TCPWATCHER_H

#include<Common/common.h>
#include<Common/epollwatcher.h>

class TcpWatcher : public EpollWatcher
{
public:
    TcpWatcher();

    void Start(uint16_t port, decltype(ConnectTimeoutCB) timeoutcb, decltype(NewConnectorCB) newconncb);

    //bool TryPop(Connector* conn);
protected:
    virtual void HandleInput(HANDLE_T connfd = 0);
    virtual void HandleWrite();
    virtual void HandleNewConnection();

    void Send(HANDLE_T fd, const std::string&  data);

    virtual int MaxEvents();

    virtual int WaitTimeout();

    void Init(uint16_t port);

private:
    void RegisterConnector(HANDLE_T connfd);

    std::string Recv(HANDLE_T connfd);

//    void ProcConnectTimeout(HANDLE_T connfd);
private:

//    Concurrent::concurrent_unordered_map<uint32_t/*connectfd*/, TcpConnector*> mConnectersMap;

};

#endif // TCPWATCHER_H
