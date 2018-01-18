#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <KCP/connnectioncontainer.h>
#include <KCP/kcptype.h>
#include <string>
#include <KCP/connection.h>
#include <Common/singleton.h>
//#include <Timer/timer.h>
#include <functional>
#include <Common/common.h>
#include <Common/memorypool.h>

typedef kcp_conv_t handle_t;

class ConnectionManager /*: Singleton<ConnectionManager>*/
{
public:
    template<class RegisterFunction>
    ConnectionManager(RegisterFunction&& RFunc);

    /***
     * 描述：处理kcp网络包，将解包后的原始字符串数据放到任务队列。
     * @param msg : 原始网络字符串接收数据
     * @param ipPort : ip-port
     * return value :
     */
    bool HandleKcpPackage(const std::string& data, const Endport& ipPort, kcp_conv_t &conv, std::string& payload);

    /***
     * 描述：发送udp数据，用于回掉，不要主动调用。
     * @param data : 原始网络字符串发送数据
     * @param ipPort : ip-port
     * return value :
     */
    int SendUdpPackage(const std::string &data, const Endport &ipPort);

    /***
     * 描述：将需要发送的消息推到发送队列。
     * @param handle : 连接句柄
     * @param msg : 发送数据
     * return value :TimerIns
     */
    int PushToKcpSendQue(kcp_conv_t conv, const std::string &msg);

    inline HANDLE_T Getfd(){
        return serverfd;
    }

    uint32_t UdpRecvPackage(std::string &data, Endport &ePort);

    void Init(uint16_t port);
private:
    int CreateConnection(const Endport &ipPort);

    void RegisterHandle(handle_t handle);

    void UpdateAllKcpTimer();

    void InitUdp(uint16_t port);

    uint32_t UdpSendMsg(const std::string &data, const Endport &ePort);


private:

    ConnnectionContainer * ConnContainer;

    std::function<void(kcp_conv_t conv, const std::string& payload)> RegisterConnector;

    HANDLE_T serverfd;
};

template<class RegisterFunction>
ConnectionManager::ConnectionManager(RegisterFunction &&RFunc)
{
    RegisterConnector = std::move(RFunc);
}

//#define ConnMgr ConnectionManager::GetInstance()

#endif // CONNECTIONMANAGER_H
