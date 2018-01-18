#include "clientshandler.h"

ClientsHandler::ClientsHandler()
{
    HandlerManager::GetInstance().Add(MSG::MSG_C2S_PING, this, &ClientsHandler::OnPing);
}

ClientsHandler::ClientsHandler(ClientsHandler &&ch)
{
    this->watcher = std::move(ch.watcher);
    this->mClientsMap = std::move(ch.mClientsMap);
}

ClientsHandler::ClientsHandler(const ClientsHandler &ch)
{
    this->watcher = ch.watcher;
    this->mClientsMap = ch.mClientsMap;
}


void ClientsHandler::Start(uint16_t port)
{
    watcher.Start(port, std::bind(&ClientsHandler::ConnectTimeoutCB, *this, std::placeholders::_1), std::bind(&ClientsHandler::NewConnectCB, *this, std::placeholders::_1, std::placeholders::_2));
}

void ClientsHandler::ProcessPackage()
{
    Connector *conn = nullptr;
    while(watcher.TryPop(conn)){
        if(conn){
            HandlerManager::GetInstance().Execute(conn->payload);
        }
    }
}

void ClientsHandler::ConnectTimeoutCB(HANDLE_T userid)
{
    if(mClientsMap.findkey(userid)){
        MemPoolIns.delElement(mClientsMap[userid]);
        mClientsMap.erase(userid);
    }
}

void ClientsHandler::NewConnectCB(HANDLE_T userid, uint64_t timeouthandle)
{
    if(!mClientsMap.findkey(userid)){
        Client * clt = MemPoolIns.newElement<Client>(userid, timeouthandle);
        mClientsMap.insert(userid, clt);
    }
}

void ClientsHandler::OnPing(Task *task)
{

}
