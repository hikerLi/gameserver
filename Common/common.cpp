
#include <Common/common.h>

//获取当前时间毫秒数
uint64_t GetCurrentTimeMsec()
{
#ifdef _WIN32
        struct timeval tv;
        time_t clock;
        struct tm tm;
        SYSTEMTIME wtm;

        GetLocalTime(&wtm);
        tm.tm_year = wtm.wYear - 1900;
        tm.tm_mon = wtm.wMonth - 1;
        tm.tm_mday = wtm.wDay;
        tm.tm_hour = wtm.wHour;
        tm.tm_min = wtm.wMinute;
        tm.tm_sec = wtm.wSecond;
        tm.tm_isdst = -1;
        clock = mktime(&tm);
        tv.tv_sec = clock;
        tv.tv_usec = wtm.wMilliseconds * 1000;
        return ((unsigned long long)tv.tv_sec * 1000 + (unsigned long long)tv.tv_usec / 1000);
#else
        struct timeval tv;
        gettimeofday(&tv,NULL);
        return ((unsigned long long)tv.tv_sec * 1000 + (unsigned long long)tv.tv_usec / 1000);
#endif
}

uint32_t GetFileSize(const std::string &filepath)
{
#ifdef _WIN32
    struct _stat info;
    _stat(filepath.c_str(), &info);
    return info.st_size;
#else
    struct stat info;
    stat(filepath.c_str(), &info);
    return info.st_size;
#endif
}

bool FileExist(const std::string &filepath)
{
    if(access(filepath.c_str(), 0) != -1){
        return true;
    }
    return false;
}
