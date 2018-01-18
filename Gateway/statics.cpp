#include <Gateway/statics.h>

IniReader Statics::mInireader;
CsvReaderManager Statics::mCsvReader;
Timer Statics::mTimer;
ClientsHandler Statics::mCltHandler;
ServersHandler Statics::mSvrHandler;
void Statics::Init()
{
    Statics::mInireader.ParseIniFile("gateway.ini");
}

uint32_t Statics::GenerateGroupId()
{
    static uint32_t mGroupId = 0;
    return ++mGroupId;
}
