#include "timer.h"
#include <thread>
#include <Common/common.h>
#include <functional>
#include <Common/threadpool.h>

Timer::Timer():
    interval(1),
    TimerId(1)
{
    mLastMilliSeconds = mCurrentMilliSeconds = GetCurrentTimeMsec();
}

Timer::~Timer()
{

}

void Timer::CancelTimer(uint64_t mTimerHandle)
{
    std::lock_guard<std::mutex> lckguard(mutex);
    mTimerMap.erase(mTimerHandle);
}

void Timer::ResetTimer(uint64_t mTimerHandle)
{
    std::lock_guard<std::mutex> lckguard(mutex);
    if(mTimerMap.findkey(mTimerHandle)){
        mTimerMap[mTimerHandle]->nextTriggerTime = GetCurrentTimeMsec() + mTimerMap[mTimerHandle]->loopTime;
    }
}

void Timer::Process()
{
    std::lock_guard<std::mutex> lckguard(mutex);
    if(!mTimerMap.empty()){
        TimerTask * task = nullptr;
        while(nullptr != (task = mTimerPrioQue.top()) && task->nextTriggerTime <= mCurrentMilliSeconds){
            if(mTimerMap.findkey(task->mTimerHandle)){
                ProcessTimerTask(task);
            }else{
                mTimerPrioQue.pop();
                MemPoolIns.delElement(task);
            }
        }
    }
}

void Timer::ProcessTimerTask(TimerTask *task)
{
    check(task);
    uint64_t currenttime = GetCurrentTimeMsec();
    if(task->nextTriggerTime <= currenttime){
        if(task->times == 1){
            mTimerPrioQue.pop();
            mTimerMap.erase(task->mTimerHandle);
        }else{
            task->nextTriggerTime = currenttime + task->loopTime;
        }

        ThreadPoolIns.Submit(std::bind(&TimerTask::Call, task));
    }
}

uint64_t Timer::GenerateTimerId()
{
    return TimerId++;
}

