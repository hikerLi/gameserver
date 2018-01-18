#include <iostream>

#include <Gateway/statics.h>
#include <Common/threadpool.h>
using namespace std;

int main()
{
    Statics::mInireader.ParseIniFile("");

    Statics::mCsvReader.AddCsvFile("");

    int32_t cltport = Statics::mInireader.GetInt("client", "port");
    cout << "start client !" << endl;
    Statics::mCltHandler.Start(cltport);

    int32_t svrport = Statics::mInireader.GetInt("server", "port");

    cout << "start server !" << endl;
    Statics::mSvrHandler.Start(svrport);

    ThreadPoolIns.Start();

    while(1){
        Statics::mTimer.Process();

        Statics::mSvrHandler.ProcessPackage();

        Statics::mCltHandler.ProcessPackage();
    }
    return 0;
}
