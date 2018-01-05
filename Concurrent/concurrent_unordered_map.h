#ifndef CONCURRENT_UNORDERED_MAP_H
#define CONCURRENT_UNORDERED_MAP_H
#include <unordered_map>
#include <Common/spinlock.h>
#include <utility>
namespace Concurrent{
    template<class KeyType, class ValueType>
    class concurrent_unordered_map
    {
    public:
        concurrent_unordered_map() = default;

        concurrent_unordered_map(concurrent_unordered_map&& conumap){
            this->unmap = std::move(conumap.unmap);
        }

        concurrent_unordered_map(const concurrent_unordered_map& conumap){
            this->unmap = conumap.unmap;
        }

        concurrent_unordered_map & operator =(const concurrent_unordered_map& conumap){
            this->unmap = conumap.unmap;
        }

        void insert(KeyType key, ValueType&& value){
            std::unique_lock<decltype(splock)> lck(splock);
            unmap.insert(std::make_pair(key, std::forward<ValueType>(value)));
        }

        void insert(KeyType key, const ValueType& value){
            std::unique_lock<decltype(splock)> lck(splock);
            unmap.insert(std::make_pair(key, value));
        }

        ValueType& operator [](const KeyType &key){
            std::unique_lock<decltype(splock)> lck(splock);
            return unmap[key];
        }

        bool findkey(const KeyType& key){
            std::unique_lock<decltype(splock)> lck(splock);
            if(unmap.find(key) != unmap.end()){
                return true;
            }
            return false;
        }

        void erase(const KeyType& key){
            std::unique_lock<decltype(splock)> lck(splock);
            unmap.erase(key);
        }

        bool empty(){
            std::unique_lock<decltype(splock)> lck(splock);
            return unmap.empty();
        }

        void clear(){
            std::unique_lock<decltype(splock)> lck(splock);
            unmap.clear();
        }

        const std::unordered_map<KeyType, ValueType> foreach(std::function<void(KeyType, ValueType)> call){
            std::unique_lock<decltype(splock)> lck(splock);
            for(auto item : unmap){
                call(item.first, item.second);
            }
        }
    private:
        std::unordered_map<KeyType, ValueType> unmap;
        SpinLock splock;
    };
}
#endif // CONCURRENT_UNORDERED_MAP_H
