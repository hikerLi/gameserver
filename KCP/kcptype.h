#ifndef KCPTYPE_H
#define KCPTYPE_H

#include <string>
#include <memory.h>

struct IKCPCB;
typedef struct IKCPCB ikcpcb;

typedef unsigned int      uint32_t;
//typedef unsigned long long uint64_t;

// indicate a converse between a client and connection_obj between server.
typedef uint32_t kcp_conv_t;

namespace kcp_svr
{

    enum eEventType
    {
        eConnect,
        eDisconnect,
        eRcvMsg,
        eLagNotify,

        eCountOfEventType
    };


    //typedef void(event_callback_t)(kcp_conv_t /*conv*/, eEventType /*event_type*/, std::shared_ptr<std::string> /*msg*/);
}

#define KCP_CONNECT_PACK "KCP_CONNECT_PACK"

#define KCP_CONNECT_ACK "KCP_CONNECT_ACK"


bool IsConnectPack(const char * data, int len);

//bool IsConnectAck(const char * data, int len);

std::string GetConnectAckPack(kcp_conv_t conv);

#endif // KCPTYPE_H
