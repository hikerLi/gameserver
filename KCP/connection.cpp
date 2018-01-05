#include "connection.h"

#include <iostream>
#include <cstring>
#include <KCP/connectionmanager.h>
#include <Common/logmanager.h>
#include <Common/memorypool.h>
#include <Common/common.h>

Connection::Connection()
{

}

Connection *Connection::Create(const kcp_conv_t &conv, const Endport &ePort, ConnectionManager *connMgr)
{
    if(nullptr == connMgr){
        return nullptr;
    }

    Connection * conn = MemPoolIns.newElement<Connection>();
    if(conn){
        conn->connMgr = connMgr;
        conn->Init(conv);
        conn->ipPort = ePort;
    }
    return conn;
}

int Connection::UdpSendOutput(const char *buf, int len, IKCPCB *kcp, void *user)
{
    return ((Connection *)user)->SendUdpPackage(buf, len);
}

void Connection::WriteLog(const char *log, IKCPCB *kcp, void *user)
{
    LOG_DEBUG(" ikcp write log : %s" , log);
}

void Connection::UpdateIkcp(uint32_t curentTimestamp)
{
    ikcp_update(pKcp, curentTimestamp);
}

int Connection::PushToSender(const std::string &msg)
{
    return ikcp_send(pKcp, msg.c_str(), msg.length());
}

bool Connection::SetToInput(const char *data, size_t len)
{
    if(nullptr == data || len < 0){
        LOG_ERR(" recv input null or len < 0.");
        return false;
    }

    if(int ret = ikcp_input(pKcp, data, len) != 0){
        LOG_ERR(" kcp input err no : <%d>", ret);
        return false;
    }
    return true;
}

bool Connection::PopByRecv(std::string &msg)
{
    char buff[MAXPAYLOAD];
    memset(buff, 0, MAXPAYLOAD);
    int ret = ikcp_recv(pKcp,buff,MAXPAYLOAD);
    if(ret <= 0){
        LOG_ERR("kcp recv < 0 len: %d", ret);
        return false;
    }

    msg = buff;

    return true;
}

void Connection::Init(const kcp_conv_t &conv)
{
    pKcp = ikcp_create(conv, (void *)this);
    if(nullptr != pKcp){
        pKcp->output = &Connection::UdpSendOutput;
        pKcp->writelog = &Connection::WriteLog;

        ikcp_nodelay(pKcp, 1, 5, 1, 1);
    }
}

int Connection::SendUdpPackage(const char *buffer, int len)
{
    return connMgr->SendUdpPackage(std::string(buffer,len), ipPort);
}
