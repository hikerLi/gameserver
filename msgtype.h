#ifndef MSGTYPE_H
#define MSGTYPE_H
#include <Common/common.h>
namespace MSG{

    enum MSGTYPE{
        MSG_MIN = 0,

        MSG_S2S_REGISTER,
        MSG_S2S_PING,

        MSG_C2S_PING,

        MSG_MAX = 65535
    };

    struct MsgHeader{
        uint32_t conv;
        uint32_t id; /*userid | serverno*/
        uint32_t flag; //0x00000001 Single 0x00000100 Group
        uint32_t groupid;

        MsgHeader() = default;
        MsgHeader(uint32_t conv, uint32_t id, uint32_t flag, uint32_t groupid)
            : conv(conv), id(id), flag(flag), groupid(groupid){

        }
    };

    enum SERVERMSG{
        AGENTSERVER = 1,
        LOBBYSERVER,
        GAMESERVER,
        MATCHSERVER,
        CHATSERVER,
        LOGSERVER
    };
}
#endif // MSGTYPE_H
