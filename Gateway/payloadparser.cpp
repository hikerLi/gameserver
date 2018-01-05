#include "payloadparser.h"


PackagePayload PayloadParser::Parser(const std::string &payload)
{
    PackagePayload package;
    void * p = (void *)payload.data();
    package.id = GetId(payload);
    p += sizeof(uint32_t);
    package.msgtype = *((uint32_t*)p);
    p += sizeof(uint32_t);
    package.headerlen = *((uint32_t*)p);
    p += sizeof(uint32_t);
    package.header = (MSG::MsgHeader*)p;
    p += sizeof(MSG::MsgHeader);
    package.bufferslen = *((uint32_t*)p);
    p += sizeof(uint32_t);
    package.buffers = std::string((char *)p, package.bufferslen);
    return package;
}

uint32_t PayloadParser::GetId(const std::string &payload)
{
    return *((uint32_t*)payload.data());
}
