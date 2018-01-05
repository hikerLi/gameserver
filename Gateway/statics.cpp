#include <Gateway/statics.h>

IniReader Statics::SInireader;
CsvReaderManager Statics::SCsvReader;
Timer Statics::STimer;
void Statics::Init()
{
    Statics::SInireader.ParseIniFile("gateway.ini");
}

uint32_t Statics::GenerateGroupId()
{
    static uint32_t mGroupId = 0;
    return ++mGroupId;
}
