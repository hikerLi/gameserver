#ifndef CONNNECTIONCONTAINER_H
#define CONNNECTIONCONTAINER_H

#include <unordered_map>
#include <KCP/kcptype.h>
#include <KCP/connection.h>

class Connection;

class ConnnectionContainer
{
public:
    ConnnectionContainer(ConnectionManager * pManager);

    /**
     * @brief 更新所有kcp时间戳
     * @param clock : 当前时间戳
     */
    void UpdateAllKcp(uint32_t clock);

    /**
     * @brief 添加一个连接
     * @param conv : 连接ID
     * @param ePort : IP 端口
     * @return 返回创建的连接对象 ,失败返回nullptr
     */
    Connection * AddConn(kcp_conv_t conv, const Endport &ePort);

    /**
     * @brief 查找一个连接
     * @param conv : 要查找的连接ID
     * @return 没找到返回nullptr
     */
    Connection * FindConnByConv(kcp_conv_t conv);

    /**
     * @brief 停止所有连接
     */
    void StopAll();

    /**
     * @brief 获取一个新的连接ID
     * @return 新的连接ID
     */
    kcp_conv_t GetNewConv();

private:
    void RegisterManager(ConnectionManager * pManager){
        ConnMgr = pManager;
    }
private:
    std::unordered_map<kcp_conv_t, Connection*> connections;

    ConnectionManager * ConnMgr;
};

#endif // CONNNECTIONCONTAINER_H
