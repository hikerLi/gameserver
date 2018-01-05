#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include <list>
#include <fstream>
#include "Common/singleton.h"
#define MAXLOGSIZE 1024

//日志管理
class LogManager : public Singleton<LogManager>
{
public:
    LogManager();
    ~LogManager();

    void Start(const std::string &logfilename, const std::string &suffix = ".txt");

    void WriteLog(const char * msg, ...);

private:
    uint32_t GetFileSize(std::ofstream &logStream);
private:
    std::string logFilePath;
    std::ofstream logOStream;
    uint32_t maxFileSize;
    std::mutex wMutex;
};

#define LogMgr LogManager::GetInstance()

#define LOG_DEBUG(FMT,...) do{\
    LogMgr.WriteLog("%s:%s:%d %s: "#FMT, "LOG_DEBUG", __FILE__, __LINE__, __func__, ##__VA_ARGS__);\
    }while(0)

#define LOG_MSG(FMT,...) do{\
    LogMgr.WriteLog("%s:%s:%d %s: "#FMT, "LOG_MSG", __FILE__, __LINE__, __func__, ##__VA_ARGS__);\
    }while(0)

#define LOG_WARN(FMT,...) do{\
    LogMgr.WriteLog("%s:%s:%d %s: "#FMT, "LOG_WARN", __FILE__, __LINE__, __func__, ##__VA_ARGS__);\
    }while(0)

#define LOG_ERR(FMT,...) do{\
    LogMgr.WriteLog("%s:%s:%d %s: "#FMT, "LOG_ERR", __FILE__, __LINE__, __func__, ##__VA_ARGS__);\
    }while(0)


#endif // LOGMANAGER_H
