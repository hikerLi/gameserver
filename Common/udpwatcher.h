#ifndef UDPWATCHER_H
#define UDPWATCHER_H

#include <Common/epollwatcher.h>
#include <Common/common.h>
#include <KCP/connectionmanager.h>
#include <Concurrent/concurrent_unordered_map.h>

//[kcp_conv | (userid|groupid) | msgtype | msgheader | protobufferlen | protobuffers]

class UdpWatcher : public EpollWatcher
{
public:
    UdpWatcher() = default;

    UdpWatcher(UdpWatcher&& watcher);

    UdpWatcher & operator =(const UdpWatcher& watcher);

    UdpWatcher(const UdpWatcher& watcher) = delete;

    void Start(uint16_t port, decltype(ConnectTimeoutCB) timeoutcb, decltype(NewConnectorCB) newconncb);

    void RegisterConnector(kcp_conv_t conv, const std::string& payload);

    uint32_t UdpSendMsg(uint32_t id, const std::string &data);
protected:

    virtual void HandleInput(HANDLE_T id = 0);
    virtual void HandleWrite();
    virtual void HandleNewConnection();

    virtual int MaxEvents();

    virtual int WaitTimeout();

    virtual inline HANDLE_T Getfd();
private:
    uint32_t UdpRecvMsg(std::string &data, Endport &ePort, kcp_conv_t &conv, std::string& payload);
protected:
    ConnectionManager* mConnManager;
private:
    Concurrent::concurrent_unordered_map<uint32_t/*id*/, kcp_conv_t/*conv*/> mIdConvMap;
};

#endif // UDPWATCHER_H
