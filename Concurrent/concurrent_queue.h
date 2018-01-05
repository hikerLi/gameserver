#ifndef CONCURRENT_QUEUE_H
#define CONCURRENT_QUEUE_H

#include <queue>
#include <Common/spinlock.h>
namespace Concurrent{
    template <class ItemType>
    class concurrent_queue
    {
    public:
        concurrent_queue() = default;

        concurrent_queue(concurrent_queue&& conque){
            this->que = std::move(conque);
        }

        void push(const ItemType& item){
            std::unique_lock<decltype(splock)> lck(splock);
            que.push(item);
        }

        void push(ItemType&& item){
            std::unique_lock<decltype(splock)> lck(splock);
            que.push(std::move(item));
        }

        bool try_pop(ItemType &item){
            std::unique_lock<decltype(splock)> lck(splock);
            if(que.empty()){
                return false;
            }else{
                item = que.front();
                que.pop();
                return true;
            }
        }

        bool empty(){
            std::unique_lock<decltype(splock)> lck(splock);
            return que.empty();
        }

    private:
        std::queue<ItemType> que;
        SpinLock splock;
    };
}

#endif // CONCURRENT_QUEUE_H
