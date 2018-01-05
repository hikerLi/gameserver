#ifndef PAYLOADPARSER_H
#define PAYLOADPARSER_H
#include <Common/common.h>
#include <msgtype.h>

class Task{
public:
    Task() = default;
    Task(uint32_t id, uint32_t msgid, MSG::MsgHeader *header, const std::string& protobuf)
        : id(id), msgid(msgid), header(header), protobuffer(protobuf){}

    uint32_t id;
    uint32_t msgid;
    MSG::MsgHeader *header;
    std::string protobuffer;
};

//[(userid|groupid) | msgtype | headerlen | msgheader | protobufferlen | protobuffers]
struct PackagePayload{

    uint32_t id;
    uint32_t msgtype;
    uint32_t headerlen;
    MSG::MsgHeader *header;
    uint32_t bufferslen;
    std::string buffers;
};

class PayloadParser : public Singleton<PayloadParser>
{
public:
    PackagePayload Parser(const std::string& payload);

    uint32_t GetId(const std::string& payload);
};

#endif // PAYLOADPARSER_H
