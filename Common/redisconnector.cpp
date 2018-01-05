#include "redisconnector.h"
#include "inireader.h"
RedisConnector::RedisConnector() : port(0), timeout(0)
{

}

RedisConnector::~RedisConnector()
{
    redisFree(context);
}

bool RedisConnector::Connect(const std::string &hostname, uint32_t port, uint32_t timeout)
{
    if(hostname.empty() || 0 == port || 0 == timeout){
        return false;
    }

    timeval tv{timeout,0};
    context = redisConnectWithTimeout(hostname.c_str(), port, tv);
    if(!context){
        return false;
    }

    this->hostname = hostname;
    this->port = port;
    this->timeout = timeout;

    return true;
}

bool RedisConnector::IsConnect()
{
    if(!context){
        return false;
    }

    std::string TestConn;
    if(RedisQuery(TestConn, "PING")){
        if(0 == TestConn.compare("PONG")){
            return true;
        }
    }
    return false;
}

void RedisConnector::CheckConnect()
{
    if(!IsConnect()){
        Connect(hostname, port, timeout);
    }
}
