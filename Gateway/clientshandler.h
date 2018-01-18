#ifndef GATEWAYHANDLER_H
#define GATEWAYHANDLER_H

#include <Common/common.h>
#include <Common/udpwatcher.h>
#include <Concurrent/concurrent_vector.h>
#include <msgtype.h>
#include <Common/payloadparser.h>
#include <Common/handlermanager.h>

struct Client{
    uint32_t userid;
    uint64_t mTimerHandler;
    Concurrent::Concurrent_vector<uint32_t/*groupid*/> mCltGroupsMap;

    Client(uint32_t userid, uint64_t timerhandle): userid(userid), mTimerHandler(timerhandle){}

    void AddToGroup(uint32_t groupid){
        mCltGroupsMap.push_back(groupid);
    }
};

typedef Concurrent::concurrent_unordered_map<uint32_t/*userid*/, Client*> ClientsMap;

class GroupManager : public Singleton<GroupManager>
{
public:
    void AddToGroup(uint32_t groupid, Client* clt){
        if(!clt){
            return;
        }
        clt->AddToGroup(groupid);
        if(!mGroupsMap.findkey(groupid)){
            ClientsMap mCltsMap;
            mCltsMap.insert(clt->userid, clt);
            mGroupsMap.insert(groupid, std::move(mCltsMap));
        }
        else{
            ClientsMap &cltsMap = mGroupsMap[groupid];
            if(!cltsMap.findkey(clt->userid)){
                cltsMap.insert(clt->userid, clt);
            }
        }
    }

    void QuitGroup(uint32_t groupid, uint32_t userid){
        if(mGroupsMap.findkey(groupid)){
            if(mGroupsMap[groupid].findkey(userid)){
                mGroupsMap[groupid].erase(userid);
            }
        }
    }

    void ClearGroup(uint32_t groupid){
        if(mGroupsMap.findkey(groupid)){
            mGroupsMap.erase(groupid);
        }
    }

    void ClearClient(Client* clt){
        if(!clt){
            clt->mCltGroupsMap.for_each([&](uint32_t groupid){
                QuitGroup(groupid, clt->userid);
            });
            clt->mCltGroupsMap.clear();
        }
    }

    void ExecuteGroup(uint32_t groupid, std::function<void(uint32_t)> func){
        if(mGroupsMap.findkey(groupid)){
            mGroupsMap[groupid].foreach([&](uint32_t userid, Client* clt){
                func(userid);
            });
        }
    }
private:
    Concurrent::concurrent_unordered_map<uint32_t/*groupid*/, ClientsMap> mGroupsMap;
};

class ClientsHandler
{
public:
    ClientsHandler();

    ClientsHandler(ClientsHandler&& ch);

    ClientsHandler(const ClientsHandler& ch);

    void Start(uint16_t port);

    void ProcessPackage();

    void ConnectTimeoutCB(HANDLE_T userid);

    void NewConnectCB(HANDLE_T userid, uint64_t timeouthandle);

    void Send2Player(uint32_t userid, const std::string& msg);

    void Send2Group(uint32_t groupid, const std::string& msg);

    void OnPing(Task * task);
private:
    UdpWatcher watcher;

    ClientsMap mClientsMap;

};

#endif // GATEWAYHANDLER_H
