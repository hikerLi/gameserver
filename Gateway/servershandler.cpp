#include "servershandler.h"

#include <Common/epollwatcher.h>
#include <msgtype.h>
ServersHandler::ServersHandler()
{
    TasksManager::GetInstance().Add(MSG::MSG_S2S_REGISTER, this, &ServersHandler::OnRegister);
}

void ServersHandler::ProcessPackage()
{
    Connector *conn;
    if(watcher.TryPop(conn)){
        TasksManager::GetInstance().Execute(conn->payload);
    }
}

void ServersHandler::ConnectTimeoutCB(HANDLE_T connfd)
{

}

void ServersHandler::NewConnectCB(HANDLE_T connfd, uint64_t timeouthandle)
{

}

void ServersHandler::Send2LobbyServer()
{

}

void ServersHandler::Send2BattleServer()
{

}

void ServersHandler::Send2ChatServer()
{

}

void ServersHandler::Send2LogServer()
{

}

void ServersHandler::Send2MatchServer()
{

}

void ServersHandler::OnRegister(Task *task)
{

}
