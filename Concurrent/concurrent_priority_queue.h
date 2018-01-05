#ifndef CONCURRENT_PRIORITY_QUEUE_H
#define CONCURRENT_PRIORITY_QUEUE_H
#include <queue>
#include <mutex>
#include <Common/spinlock.h>

namespace Concurrent{
    template<class ItemType, typename _Compare  = std::less<int>>
    class concurrent_priority_queue
    {
    public:
        concurrent_priority_queue() = default;
        ~concurrent_priority_queue() = default;

        concurrent_priority_queue(const _Compare& comp){
            std::priority_queue<ItemType> pri_queue(comp);
            priority_queue = std::move(pri_queue);
        }

        void push(const ItemType& item);
        void push(ItemType&& item);

        bool try_pop(ItemType &item);
        const ItemType& top();
        void pop();
        size_t size();
        bool empty();
    private:
        std::priority_queue<ItemType> priority_queue;
        SpinLock splock;
    };

    template<class ItemType, typename _Compare>
    void concurrent_priority_queue<ItemType, _Compare>::push(const ItemType &item)
    {
        std::unique_lock<decltype(splock)> ulock(splock);
        priority_queue.push(item);
    }

    template<class ItemType, typename _Compare>
    void concurrent_priority_queue<ItemType, _Compare>::push(ItemType &&item)
    {
        std::unique_lock<decltype(splock)> ulock(splock);
        priority_queue.push(std::forward<ItemType>(item));
    }

    template<class ItemType, typename _Compare>
    bool concurrent_priority_queue<ItemType, _Compare>::try_pop(ItemType& item)
    {
        std::unique_lock<decltype(splock)> ulock(splock);
        if(priority_queue.empty()){
            return false;
        }else{
            item = priority_queue.top();
            priority_queue.pop();
            return true;
        }
    }

    template<class ItemType, typename _Compare>
    const ItemType &concurrent_priority_queue<ItemType, _Compare>::top()
    {
        std::unique_lock<decltype(splock)> ulock(splock);
        return priority_queue.top();
    }

    template<class ItemType, typename _Compare>
    void concurrent_priority_queue<ItemType, _Compare>::pop()
    {
        std::unique_lock<decltype(splock)> ulock(splock);
        priority_queue.pop();
    }

    template<class ItemType, typename _Compare>
    size_t concurrent_priority_queue<ItemType, _Compare>::size()
    {
        std::unique_lock<decltype(splock)> ulock(splock);
        return priority_queue.size();
    }

    template<class ItemType, typename _Compare>
    bool concurrent_priority_queue<ItemType, _Compare>::empty()
    {
        std::unique_lock<decltype(splock)> ulock(splock);
        return priority_queue.empty();
    }
}
#endif // CONCURRENT_PRIORITY_QUEUE_H
