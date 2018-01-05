#ifndef REDISCONNECTOR_H
#define REDISCONNECTOR_H

#include <Thirdparty/hiredis/hiredis.h>
#include <Common/singleton.h>
#include <memory>
#include <vector>
class RedisConnector : public Singleton<RedisConnector>
{
public:
    RedisConnector();
    ~RedisConnector();

    template<class... Args>
    bool RedisExecute(const std::string &cmd, Args... args);

    template<class ResultType, class... Args>
    bool RedisQuery(ResultType& result, const std::string &cmd, Args... args);

    bool Connect(const std::string& hostname, uint32_t port = 6379, uint32_t timeout = 604800);

private:

    bool IsConnect();

    void CheckConnect();

    template<class ResultType>
    bool GetArrayResult(redisReply * reply, std::vector<ResultType> &result);

    template<class ResultType>
    bool GetResult(redisReply * reply, ResultType &result);
private:
    redisContext * context;

    std::string hostname;
    uint32_t port;
    uint32_t timeout;
};

template<class ResultType>
bool RedisConnector::GetArrayResult(redisReply *reply, std::vector<ResultType> &result)
{
    if(!reply)
        return false;
    switch(reply->type){
    case REDIS_REPLY_ARRAY:
        for (int i = 0; i < reply->elements; i++) {
            ResultType item;
            if(GetResult(reply->element[i], item)){
                result.push_back(item);
            }
        }
        break;
    }
}

template<class ResultType>
bool RedisConnector::GetResult(redisReply *reply, ResultType &result)
{
    if(!reply)
        return false;
    switch(reply->type){
    case REDIS_REPLY_STRING:
        result.assign(reply->str, reply->len);
        break;
    case REDIS_REPLY_INTEGER:
        result = std::atoi(reply->str);
        break;
    case REDIS_REPLY_STATUS:    //返回执行结果为状态的命令。比如set命令的返回值的类型是REDIS_REPLY_STATUS，然后只有当返回信息是"OK"时，才表示该命令执行成功。
        result.assign(reply->str, reply->len);
        break;
    case REDIS_REPLY_NIL:   //返回nil对象，说明不存在要访问的数据。
        return false;
    case REDIS_REPLY_ERROR:
        return false;
    case REDIS_REPLY_ARRAY:
        return false;
    }
    return true;
}

template<class ResultType, class... Args>
bool RedisConnector::RedisQuery(ResultType &result, const std::string &cmd, Args... args)
{
    CheckConnect();

    redisReply * reply = (redisReply *)redisCommand(context, cmd.c_str(), std::forward<Args>(args)...);
    std::shared_ptr<redisReply> shared(reply, freeReplyObject);
    if(!reply){
        if(GetResult(reply, result)){
            return true;
        }
    }
    return false;
}

template<class... Args>
bool RedisConnector::RedisExecute(const std::string &cmd, Args... args)
{
    CheckConnect();

    redisReply * reply = (redisReply *)redisCommand(context, cmd.c_str(), std::forward<Args>(args)...);
    std::shared_ptr<redisReply> shared(reply, freeReplyObject);
    if(!reply){
        std::string result;
        if(GetResult(reply, result) && 0 == result.compare("OK")){
            return true;
        }
    }
    return false;
}

#endif // REDISCONNECTOR_H
