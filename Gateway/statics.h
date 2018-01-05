#ifndef STATICS_H
#define STATICS_H

#include <Common/inireader.h>
#include <Common/csvreader.h>
#include <Common/timer.h>
#include <Gateway/payloadparser.h>
class Statics{

public:
    static void Init();

    static uint32_t GenerateGroupId();
public:
    static IniReader SInireader;
    static CsvReaderManager SCsvReader;
    static Timer STimer;
};

#endif // STATICS_H


