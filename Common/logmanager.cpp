#include "Common/logmanager.h"
#include <cstring>
#include <cstdarg>
#include <thread>
#include <iostream>
#include <Common/inireader.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <Common/threadpool.h>

using namespace std;

LogManager::LogManager() :
    maxFileSize(10000), logFilePath("./")
{
}

LogManager::~LogManager()
{
    logOStream.close();
}

void LogManager::Start(const string &logfilename, const string &suffix)
{
    std::string logfullpath = logFilePath + logfilename + suffix;

    logOStream.open(logfullpath, std::ios::out|std::ios::app);
    if(logOStream.fail())
    {
        std::cout << "open log file failed." << logfullpath << std::endl;
        exit(1);
    }

    logOStream.tellp();
}

uint32_t LogManager::GetFileSize(ofstream &logStream)
{
    if(logStream.is_open()){
        return logStream.tellp();
    }
    return 0;
}

void LogManager::WriteLog(const char *fmt, ...)
{
    char * logBuff = new char[MAXLOGSIZE];
    memset(logBuff, 0, sizeof(char) * MAXLOGSIZE);

    va_list	ap;
    va_start(ap, fmt);
    vsnprintf(logBuff, MAXLOGSIZE, fmt,ap);
    va_end(ap);

    ThreadPoolIns.Submit([&](){
        std::unique_lock<std::mutex> ulock(wMutex);
        logOStream << logBuff << std::endl << std::flush;
    });
}

