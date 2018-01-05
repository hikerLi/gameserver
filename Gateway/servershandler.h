#ifndef SERVERSHANDLER_H
#define SERVERSHANDLER_H

#include <Common/tcpwatcher.h>
#include <Concurrent/concurrent_vector.h>
#include <Common/tasksmanager.h>
// [SERVERMSG | msgtype | headerlen | msgheader | protobufferlen | protobuffers]
class ServersHandler
{
public:
    ServersHandler();

    void ProcessPackage();

    void ConnectTimeoutCB(HANDLE_T connfd);

    void NewConnectCB(HANDLE_T connfd, uint64_t timeouthandle);

    void Send2LobbyServer();

    void Send2BattleServer();

    void Send2ChatServer();

    void Send2LogServer();

    void Send2MatchServer();

    void OnRegister(Task *task);
private:
    TcpWatcher watcher;

    Concurrent::Concurrent_vector<uint32_t/*HANDLE*/> mLobbysVec;
    Concurrent::Concurrent_vector<uint32_t/*HANDLE*/> mBattlesVec;
    Concurrent::Concurrent_vector<uint32_t/*HANDLE*/> mChatsVec;
    Concurrent::Concurrent_vector<uint32_t/*HANDLE*/> mMatchsVec;
    Concurrent::Concurrent_vector<uint32_t/*HANDLE*/> mLogsVec;
};

#endif // SERVERSHANDLER_H
