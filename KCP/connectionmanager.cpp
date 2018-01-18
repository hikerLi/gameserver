#include "connectionmanager.h"

#include <KCP/ThirdParty/ikcp.h>
#include <string>
#include <KCP/connection.h>
#include <iostream>
#include <KCP/kcptype.h>
#include <event2/event.h>
#include <Common/common.h>

using namespace std;

bool ConnectionManager::HandleKcpPackage(const string &data, const Endport &ipPort, kcp_conv_t &conv, std::string& payload)
{
    //处理连接包
    if(IsConnectPack(data.c_str(), data.length())){
        if(CreateConnection(ipPort) > 0){
            LOG_DEBUG("create connection.");
            return false;
        }
    }

    int ret = ikcp_get_conv(data.c_str(), data.length(), &conv);
    if(0 == ret){
        LOG_DEBUG(" kcp get conv faield. len : %d ",data.length());
        return false;
    }

    bool bNewConnection = false;
    Connection * conn = ConnContainer->FindConnByConv(conv);
    if(nullptr == conn){
        conn = ConnContainer->AddConn(conv, ipPort);
        bNewConnection = true;
        assert(conn);
    }

    if(conn){
        if(conn->SetToInput(data.c_str(), data.length())){
            if(conn->PopByRecv(payload)){
                if(bNewConnection){
                    RegisterConnector(conv, payload);
                }
                return true;
            }
        }
    }

    LOG_DEBUG("find conn failed conv : %d ", conv);

    return false;
}

int ConnectionManager::SendUdpPackage(const string &data, const Endport &ipPort)
{
    return UdpSendMsg(data, ipPort);
}

int ConnectionManager::PushToKcpSendQue(kcp_conv_t conv, const string &msg)
{
    Connection * conn = ConnContainer->FindConnByConv(conv);
    if(nullptr != conn){
        return conn->PushToSender(msg);
    }
    return -1;
}

int ConnectionManager::CreateConnection(const Endport &ipPort)
{
    kcp_conv_t conv = ConnContainer->GetNewConv();
    LOG_DEBUG("create connection: endport : %s, %d. conv:<%d>", ipPort.ip.c_str(), ipPort.port, conv);
    return SendUdpPackage(GetConnectAckPack(conv), ipPort);
}

void ConnectionManager::UpdateAllKcpTimer()
{
    ConnContainer->UpdateAllKcp(GetCurrentTimeMsec());
}

void ConnectionManager::InitUdp(uint16_t port)
{
    serverfd = ::socket(PF_INET, SOCK_DGRAM, 0);
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
}

uint32_t ConnectionManager::UdpSendMsg(const std::string &data, const Endport &ePort)
{
    if(data.empty() || ePort.Empty()){
        LOG_ERR("Param error.");
        return -1;
    }

    struct sockaddr_in sock_addr;
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = htons(ePort.port);
    sock_addr.sin_addr.s_addr = inet_addr(ePort.ip.c_str());
    return ::sendto(serverfd, data.c_str(), data.length(), 0, (sockaddr*)&sock_addr, sizeof(sockaddr));
}

uint32_t ConnectionManager::UdpRecvPackage(std::string &data, Endport &ePort)
{
    struct sockaddr_in client_addr;
    socklen_t socklen;
    char buff[MAXPAYLOAD];
    memset(buff, 0, MAXPAYLOAD);
    int ret = ::recvfrom(serverfd, buff, MAXPAYLOAD, 0, (struct sockaddr *)&client_addr, &socklen);
    if(ret > 0){
        ePort.ip = inet_ntoa(client_addr.sin_addr);
        ePort.port = ntohs(client_addr.sin_port);
        data = std::string(buff, ret);
    }
    return ret;
}

void ConnectionManager::Init(uint16_t port)
{
    ConnContainer = MemPoolIns.newElement<ConnnectionContainer>(this);

    InitUdp(port);
}
