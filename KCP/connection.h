#ifndef CONNECTION_H
#define CONNECTION_H

#include <KCP/ThirdParty/ikcp.h>
#include <string>
#include <Common/common.h>
class ConnectionManager;

class Connection
{
public:
    Connection();


    /*
     * 描述：创建一个连接对象，在有新连接时进行
     * @param conv : 连接ID
     * @param ePort : 连接ip和端口
     * @param connMgr : 管理连接的对象
     */
    static Connection *Create(const kcp_conv_t &conv, const Endport &ePort, ConnectionManager *connMgr);

    /**
     * @brief 发送数据的回调函数，不需要主动调用
     * @param buf : 数据字符串
     * @param len : 数据长度
     * @param kcp : 所发送目标的kcp对象
     * @param user :附加参数
     * @return 发送成功返回 0
     */
    static int UdpSendOutput(const char *buf, int len, struct IKCPCB *kcp, void *user);

    /**
     * @brief kcp日志回调函数
     * @param log : 日志内容
     * @param kcp : kcp对象
     * @param user : 附加参数
     */
    static void WriteLog(const char *log, struct IKCPCB *kcp, void *user);

    /**
     * @brief kcp持续更新，处理各种包的丢失重发
     * @param curentTimestamp : 当前时间戳
     */
    void UpdateIkcp(uint32_t curentTimestamp);

    /**
     * @brief 加入到kcp发送包队列
     * @param msg : kcp消息头加入前，要发送的消息
     * @return 成功返回0
     */
    int PushToSender(const std::string &msg);

    /**
     * @brief 将接受到的数据加入到kcp消息队列待读取
     * @param data : 带有kcp消息头的数据
     * @param len : 数据长度
     */
    bool SetToInput(const char * data, size_t len);

    /**
     * @brief 从kcp数据队列获取一条数据
     * @param msg : 获取到的数据
     * @return 成功返回true
     */
    bool PopByRecv(std::string &msg);

private:
    void Init(const kcp_conv_t &conv);

    int SendUdpPackage(const char * buffer, int len);

private:
    ikcpcb * pKcp;

    Endport ipPort;

    ConnectionManager * connMgr;
};

#endif // CONNECTION_H
