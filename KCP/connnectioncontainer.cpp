#include "connnectioncontainer.h"

#include <KCP/connection.h>
#include <KCP/connectionmanager.h>

ConnnectionContainer::ConnnectionContainer(ConnectionManager * pManager)
{
    RegisterManager(pManager);
}

void ConnnectionContainer::UpdateAllKcp(uint32_t clock)
{
    for(auto connpair : connections){
        Connection * conn = (Connection * )connpair.second;
        if(nullptr != conn){
            conn->UpdateIkcp(clock);
        }
    }
}

Connection *ConnnectionContainer::AddConn(kcp_conv_t conv, const Endport &ePort)
{
    Connection * conn = Connection::Create(conv, ePort, ConnMgr);
    if(conn){
        connections[conv] = conn;
    }
    return conn;
}

Connection *ConnnectionContainer::FindConnByConv(kcp_conv_t conv)
{
    return connections[conv];
}


void ConnnectionContainer::StopAll()
{

}

kcp_conv_t ConnnectionContainer::GetNewConv()
{
    // todo using rand to get a conv. privent the attack from guess conv.
    // and must bigger than 1000

    // increase from 1001, must bigger than 1000
    static uint32_t static_cur_conv = 1000;
    static_cur_conv++;
    return static_cur_conv;
}
