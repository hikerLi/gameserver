
#include <KCP/kcptype.h>

bool IsConnectPack(const char * data, int len)
{
    return (len >= strlen(KCP_CONNECT_PACK) &&
            memcmp(data, KCP_CONNECT_PACK, sizeof(KCP_CONNECT_PACK) - 1) == 0);
}

//bool IsConnectAck(const char * data, int len)
//{
//    return (len > sizeof(KCP_CONNECT_ACK) &&
//            memcmp(data, KCP_CONNECT_ACK, sizeof(KCP_CONNECT_ACK) - 1) == 0);
//}

std::string GetConnectAckPack(kcp_conv_t conv)
{
    char str_send_back_conv[256] = {};
    size_t n = snprintf(str_send_back_conv, sizeof(str_send_back_conv), "%s %u", KCP_CONNECT_ACK, conv);
    return std::string(str_send_back_conv, n);
}
