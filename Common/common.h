#ifndef COMMON_H
#define COMMON_H

#include <Common/commonmacros.h>
#include <string>
#include  <stdlib.h>
#include <Common/logmanager.h>
#include <stdint.h>
#ifdef _WIN32
    #include <winsock2.h>
    #include <time.h>
    #include <Winsock2.h>
#else
    #include <sys/time.h>
    #include <sys/stat.h>
    #include <unistd.h>
    #include <sys/socket.h>
    #include <sys/unistd.h>
    #include <netinet/in.h>
    #include <sys/types.h>
    #include <fcntl.h>
    #include <arpa/inet.h>
#endif

//获取当前时间毫秒数
uint64_t GetCurrentTimeMsec();

uint32_t GetFileSize(const std::string &filepath);

bool FileExist(const std::string &filepath);

typedef uint32_t HANDLE_T;

struct Endport
{
    std::string ip;
    short port;

    bool Empty() const{
        return ip.empty() || 0 == port;
    }
};

#define MAXPAYLOAD 65535

#define TCPTIMEOUT 16
#define UDPTIMEOUT 16
#endif // COMMON_H
