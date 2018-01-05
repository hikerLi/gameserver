#ifndef LOBBYPAYLOADHANDLER_H
#define LOBBYPAYLOADHANDLER_H

#include <Common/payloadhandler.h>
class LobbyPayloadHandler : public PayloadHandler
{
public:
    LobbyPayloadHandler();

    virtual void ProcessPayload(const std::string & payload);
};

#endif // LOBBYPAYLOADHANDLER_H
