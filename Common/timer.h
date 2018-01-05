#ifndef TIMER_H
#define TIMER_H

#include <functional>
#include <Common/singleton.h>
#include <thread>
#include <Common/common.h>
#include <Concurrent/concurrent_unordered_map.h>
#include <Concurrent/concurrent_priority_queue.h>
#include <Common/spinlock.h>
#include <mutex>
#include <condition_variable>
#include <future>
#include <type_traits>
#include <Common/memorypool.h>
struct TimerTask
{
    struct FuncBase{
        virtual void Call() = 0;
    };

    std::unique_ptr<FuncBase> storage;
    template <class FunctionType>
    struct FunctionImp : public FuncBase{
        FunctionType func;
        FunctionImp(FunctionType&& func): func(std::forward<FunctionType>(func)){}
        virtual void Call(){
            func();
        }
    };
public:
    TimerTask() = default;

    template<class FunctionType>
    TimerTask(uint64_t mTimerHandle, uint64_t loopTime, uint32_t times, FunctionType&& func):
        mTimerHandle(mTimerHandle), loopTime(loopTime), times(times) {
        storage = std::unique_ptr<FunctionImp<FunctionType>>(new FunctionImp<FunctionType>(std::forward<FunctionType>(func)));
        nextTriggerTime = GetCurrentTimeMsec() + loopTime;
    }

    void Call(){
        storage->Call();
        if(1 == times){
            MemPoolIns.delElement(this);
        }else if(times > 1){
            times--;
        }
    }

    uint64_t mTimerHandle;
    uint32_t times; //0 always loop
    uint64_t loopTime;
    uint64_t nextTriggerTime;
};

class TaskCompare
{
  public:
    bool operator()(const TimerTask& taskleft, const TimerTask& taskright)
    {
        return taskleft.nextTriggerTime > taskright.nextTriggerTime;
    }
};

class Timer //: public Singleton<Timer>
{
public:
    Timer();

    ~Timer();

    template <class FunctionType>
    uint64_t Add(uint64_t milliseconds, FunctionType&& func);

    template <class FunctionType>
    uint64_t AddOnce(uint64_t milliseconds, FunctionType&& func);

    template <class FunctionType>
    uint64_t AddTimes(uint64_t milliseconds, int times, FunctionType&& func);

    void CancelTimer(uint64_t mTimerHandle);

    void ResetTimer(uint64_t mTimerHandle);

    void Process();
private:
    void ProcessTimerTask(TimerTask * task);

    uint64_t GenerateTimerId();
private:

    uint64_t mLastMilliSeconds;
    uint64_t mCurrentMilliSeconds;

//    //lock
    SpinLock splock;
    std::mutex mutex;
//    std::condition_variable mCondition;

    Concurrent::concurrent_unordered_map<uint64_t/*timerid*/, TimerTask*> mTimerMap;
    Concurrent::concurrent_priority_queue<TimerTask*, TaskCompare> mTimerPrioQue;
    const uint64_t interval; //milliseconds
    std::atomic_ullong TimerId;
};

//typedef Singleton<Timer> TimerSingleton;
//#define TimerIns TimerSingleton::GetInstance()

template<class FunctionType>
uint64_t Timer::Add(uint64_t milliseconds, FunctionType&& func)
{
    return AddTimes(milliseconds, 0, std::forward<FunctionType>(func));
}

template<class FunctionType>
uint64_t Timer::AddOnce(uint64_t milliseconds, FunctionType&& func)
{
    return AddTimes(milliseconds, 1, std::forward<FunctionType>(func));
}

template<class FunctionType>
uint64_t Timer::AddTimes(uint64_t milliseconds, int32_t times, FunctionType&& func)
{
    std::unique_lock<decltype(splock)> unlock(splock);
    uint64_t timerId = GenerateTimerId();
    TimerTask * task = MemPoolIns.newElement<TimerTask>(timerId, milliseconds, times, std::forward<FunctionType>(func));
    mTimerMap.insert(timerId, task);
    mTimerPrioQue.push(task);
//    mCondition.notify_one();
    return timerId;
}
#endif // TIMER_H
