#include "udpwatcher.h"
#include <Common/memorypool.h>
#include <Gateway/statics.h>


UdpWatcher::UdpWatcher(UdpWatcher &&watcher)
{
    this->mConnManager = watcher.mConnManager;
    watcher.mConnManager = nullptr;

    this->mIdConvMap = watcher.mIdConvMap;
}

UdpWatcher &UdpWatcher::operator =(const UdpWatcher &watcher)
{
    this->mConnManager = watcher.mConnManager;

    this->mIdConvMap = watcher.mIdConvMap;
}

//UdpWatcher::UdpWatcher(const UdpWatcher &watcher)
//{
//    this->mConnManager = watcher.mConnManager;

//    this->mIdConvMap = watcher.mIdConvMap;
//}

void UdpWatcher::HandleInput(HANDLE_T connfd)
{
    std::string rData, payload;
    Endport ePort;
    kcp_conv_t conv;
    if(UdpRecvMsg(rData, ePort, conv, payload)){
        Connector * conn = MemPoolIns.newElement<Connector>(payload, connfd, conv);
        mConnectorsQue.push(conn);
    }
}

void UdpWatcher::HandleWrite()
{

}

void UdpWatcher::HandleNewConnection()
{

}

int UdpWatcher::MaxEvents()
{

}

int UdpWatcher::WaitTimeout()
{

}

HANDLE_T UdpWatcher::Getfd()
{
    return mConnManager->Getfd();
}

uint32_t UdpWatcher::UdpSendMsg(uint32_t id, const std::string &data)
{
    if(mIdConvMap.findkey(id)){
        return mConnManager->PushToKcpSendQue(mIdConvMap[id], data);
    }
    return 0;
}

uint32_t UdpWatcher::UdpRecvMsg(std::string &data, Endport &ePort, kcp_conv_t &conv, std::string &payload)
{
    if(mConnManager->UdpRecvPackage(data, ePort) > 0){
        if(mConnManager->HandleKcpPackage(data, ePort, conv, payload)){
            return data.length();
        }
    }
    return 0;
}

void UdpWatcher::Start(uint16_t port, decltype(ConnectTimeoutCB) timeoutcb, decltype(NewConnectorCB) newconncb)
{
    ConnectTimeoutCB = timeoutcb;

    NewConnectorCB = newconncb;

    mConnManager = MemPoolIns.newElement<ConnectionManager>([&](kcp_conv_t conv, const std::string &payload){
        this->RegisterConnector(conv, payload);
    });

    mConnManager->Init(port);

    EpollWatcher::Start();
}

void UdpWatcher::RegisterConnector(kcp_conv_t conv, const std::string &payload)
{
    uint32_t id = PayloadParser::GetInstance().GetId(payload);
    uint64_t mTimerHandler = mConnTimer.AddOnce(UDPTIMEOUT, [&](){
        mIdConvMap.erase(id);
        ConnectTimeoutCB(id);
    });

    mIdConvMap.insert(id, conv);
    NewConnectorCB(id, mTimerHandler);
}
