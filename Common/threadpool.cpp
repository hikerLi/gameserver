#include "threadpool.h"
ThreadPool::ThreadPool() :
    mRunning(false)
{

}

void ThreadPool::Start()
{
    mRunning.store(true);
    uint32_t mThreadCount=std::thread::hardware_concurrency();
    for(uint32_t i = 0; i < mThreadCount; i++){
        std::thread taskthread(&ThreadPool::WorkThread, this);
        taskthread.detach();
        threads.push_back(std::move(taskthread));
    }
}

void ThreadPool::WorkThread()
{
    while(mRunning){
        RunPendingTask();
    }
}

void ThreadPool::RunPendingTask()
{
    FunctionWrapper task;
    if(taskque.try_pop(task)){
        task();
    }else{
        std::this_thread::yield();
    }
}
