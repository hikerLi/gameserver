#ifndef CONCURRENT_VECTOR_H
#define CONCURRENT_VECTOR_H

#include <vector>
#include <Common/spinlock.h>
#include <mutex>
namespace Concurrent{
    template <class ItemType>
    class Concurrent_vector
    {
    public:
        Concurrent_vector() = default;

        void push_back (const ItemType& val);

        void push_back (ItemType&& val);

        template<class FunctionType>
        void for_each(FunctionType&& func){
            std::unique_lock<decltype(splock)> ulock(splock);
            for(auto item : vector){
                func(item);
            }
        }

        size_t size() const;

        bool empty();

        void clear();

        void erase(const ItemType & item);

        ItemType & operator [](size_t n);
        const ItemType &operator [](size_t n) const;

    private:
        std::vector<ItemType> vector;
        SpinLock splock;
    };

//    template<class ItemType>
//    template<class FunctionType>
//    void Concurrent_vector</*ItemType,*/ FunctionType>::for_each(FunctionType&& func)
//    {
//        std::unique_lock<decltype(splock)> ulock(splock);
//        for(auto item : vector){
//            func(item);
//        }
//    }

    template<class ItemType>
    void Concurrent_vector<ItemType>::push_back(const ItemType &val)
    {
        std::unique_lock<decltype(splock)> ulock(splock);
        vector.push_back(val);
    }

    template<class ItemType>
    void Concurrent_vector<ItemType>::push_back(ItemType &&val)
    {
        std::unique_lock<decltype(splock)> ulock(splock);
        vector.push_back(std::forward<ItemType>(val));
    }

    template<class ItemType>
    size_t Concurrent_vector<ItemType>::size() const
    {
        std::unique_lock<decltype(splock)> ulock(splock);
        return vector.size();
    }

    template<class ItemType>
    bool Concurrent_vector<ItemType>::empty()
    {
        std::unique_lock<decltype(splock)> ulock(splock);
        return vector.empty();
    }

    template<class ItemType>
    void Concurrent_vector<ItemType>::clear()
    {
        std::unique_lock<decltype(splock)> ulock(splock);
        return vector.clear();
    }

    template<class ItemType>
    void Concurrent_vector<ItemType>::erase(const ItemType &item)
    {
        std::unique_lock<decltype(splock)> ulock(splock);
        for(auto iter = vector.begin(); iter != vector.end(); iter++){
            if(*iter == item){
                vector.erase(iter);
                break;
            }
        }
    }

    template<class ItemType>
    ItemType &Concurrent_vector<ItemType>::operator [](size_t n)
    {
        std::unique_lock<decltype(splock)> ulock(splock);
        return vector[n];
    }

    template<class ItemType>
    const ItemType &Concurrent_vector<ItemType>::operator [](size_t n) const
    {
        std::unique_lock<decltype(splock)> ulock(splock);
        return vector[n];
    }
}
#endif // CONCURRENT_VECTOR_H
