#ifndef STATICS_H
#define STATICS_H

#include <Common/inireader.h>
#include <Common/csvreader.h>
#include <Common/timer.h>
#include <Gateway/payloadparser.h>
#include <Gateway/clientshandler.h>
#include <Gateway/servershandler.h>
class Statics{

public:
    static void Init();

    static uint32_t GenerateGroupId();
public:
    static IniReader mInireader;
    static CsvReaderManager mCsvReader;
    static Timer mTimer;
    static ClientsHandler mCltHandler;
    static ServersHandler mSvrHandler;
};

#endif // STATICS_H


