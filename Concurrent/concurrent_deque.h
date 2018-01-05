#ifndef CONCURRENT_DEQUE_H
#define CONCURRENT_DEQUE_H
#include <deque>
#include <mutex>
#include <Common/spinlock.h>

namespace Concurrent{
    template<class ItemType>
    class concurrent_deque
    {
    public:
        concurrent_deque() = default;
        ~concurrent_deque() = default;

    public:

        void push_back(ItemType item);
        void push_front(ItemType item);
        bool pop_back(ItemType &item);
        bool pop_front(ItemType &item);

        bool find(const ItemType &item);

        void remove(const ItemType &item);

        bool empty();

        std::deque<ItemType> copy();
    private:
        std::deque<ItemType> deque;
        SpinLock splock;
    };

    template<class ItemType>
    bool concurrent_deque<ItemType>::find(const ItemType &item)
    {
        std::unique_lock<decltype(splock)> unlock(splock);
        for (auto it = deque.begin(); it != deque.end(); ++it) {
            if (*it == item) {
                return true;
            }
        }
        return false;
    }

    template<class ItemType>
    bool concurrent_deque<ItemType>::empty()
    {
        std::unique_lock<decltype(splock)> unlock(splock);
        return deque.empty();
    }

    template<class ItemType>
    void concurrent_deque<ItemType>::push_back(ItemType item)
    {
        std::unique_lock<decltype(splock)> unlock(splock);
        deque.push_back(item);
    }

    template<class ItemType>
    void concurrent_deque<ItemType>::push_front(ItemType item)
    {
        std::unique_lock<decltype(splock)> unlock(splock);
        deque.push_front(item);
    }

    template<class ItemType>
    bool concurrent_deque<ItemType>::pop_back(ItemType &item)
    {
        std::unique_lock<decltype(splock)> unlock(splock);
        if (deque.empty()) {
            return false;
        }
        item = deque.back();
        deque.pop_back();
        return true;
    }

    template<class ItemType>
    bool concurrent_deque<ItemType>::pop_front(ItemType &item)
    {
        std::unique_lock<decltype(splock)> unlock(splock);
        if (deque.empty()) {
            return false;
        }
        item = deque.front();
        deque.pop_front();
        return true;
    }

    template<class ItemType>
    void concurrent_deque<ItemType>::remove(const ItemType &item)
    {
        std::unique_lock<decltype(splock)> unlock(splock);
        for (auto it = deque.begin(); it != deque.end(); ++it) {
            if (*it == item) {
                deque.erase(it);
                return;
            }
        }
    }

    template<class ItemType>
    std::deque<ItemType> concurrent_deque<ItemType>::copy()
    {
        std::unique_lock<decltype(splock)> unlock(splock);
        return deque;
    }
}
#endif // CONCURRENT_DEQUE_H
