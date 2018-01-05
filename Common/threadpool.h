#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <Common/singleton.h>
#include <Concurrent/concurrent_queue.h>
#include <memory>
#include <thread>
#include <future>
#include <type_traits>
class FunctionWrapper
{
    struct ImplBase
    {
        virtual void Call() = 0;
    };
    std::unique_ptr<ImplBase> imp;

    template <class FunctionType>
    struct ImplType : public ImplBase
    {
        FunctionType func;
        ImplType(FunctionType&& f) : func(std::move(f)){}
        virtual void Call(){func();}
    };

public:

    void operator ()(){
        imp->Call();
    }

    FunctionWrapper() = default;

    template<typename F>
    FunctionWrapper(F&& f) :
        imp(new ImplType<F>(std::move(f))){}

    FunctionWrapper(FunctionWrapper&& other) : imp(std::move(other.imp)){}

    FunctionWrapper& operator=(FunctionWrapper&& other){
        imp = std::move(other.imp);
        return *this;
    }

    FunctionWrapper& operator=(FunctionWrapper& other){
        imp = std::move(other.imp);
        return *this;
    }
};

/**
 * @brief epoll single thread, other threadpool
 */

class ThreadPool : public Singleton<ThreadPool>
{
public:
    ThreadPool();

    void Start();

    template <class FunctionType>
    std::future<typename std::result_of<FunctionType()>::type> Submit(FunctionType&& func){
        typedef typename std::result_of<FunctionType()>::type ResultType;
        std::packaged_task<ResultType()> task(std::move(func));
        std::future<ResultType> result = task.get_future();
        taskque.push(std::move(task));
        return result;
    }

private:
    void WorkThread();

    void RunPendingTask();
private:

    std::atomic_bool mRunning;
    Concurrent::concurrent_queue<FunctionWrapper> taskque;
    std::vector<std::thread> threads;
};

#define ThreadPoolIns ThreadPool::GetInstance()
#endif // THREADPOOL_H
