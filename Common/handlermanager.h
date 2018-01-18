#ifndef TASKSMANAGER_H
#define TASKSMANAGER_H

#include <msgtype.h>
#include <Common/common.h>
#include <Concurrent/concurrent_unordered_map.h>
#include <Common/payloadparser.h>

class HandlerManager : public Singleton<HandlerManager>{
public:
    template<class TargetType, class FuncType>
    void Add(uint32_t msgid, TargetType *obj, FuncType&& func){
        mTaskHandlerMap[msgid][obj] = std::bind(std::forward<FuncType>(func), obj, std::placeholders::_1);
    }

    void Remove(uint32_t msgid){
        if(mTaskHandlerMap.findkey(msgid)){
            mTaskHandlerMap.erase(msgid);
        }
    }

    void Execute(const std::string & payload){
        PackagePayload pkgPayload = PayloadParser::GetInstance().Parser(payload);
        if(mTaskHandlerMap.findkey(pkgPayload.msgtype)){
            Task task(pkgPayload.id, pkgPayload.msgtype, pkgPayload.header, pkgPayload.buffers);
            mTaskHandlerMap[pkgPayload.msgtype].foreach([&](void *, std::function<void(Task*)> func){
                func(&task);
            });
        }
    }
private:
    Concurrent::concurrent_unordered_map<uint32_t, Concurrent::concurrent_unordered_map<void *, std::function<void(Task*)>>> mTaskHandlerMap;
};

#endif // TASKSMANAGER_H
