#ifndef SINGLETON_H
#define SINGLETON_H

#include <mutex>
#include <Common/spinlock.h>
#include <jemalloc/jemalloc.h>

template<class Type>
class Singleton
{
public:

    template <typename... Args>
    static Type &GetInstance(Args&&... args)
    {
        if(nullptr == ClassInstance){
            std::lock_guard<decltype(splock)> lock(splock);
            if(nullptr == ClassInstance){
                ClassInstance = (Type*)malloc(sizeof(Type));
                new (ClassInstance) Type(std::forward<Args>(args)...);
            }
        }
        return *ClassInstance;
    }

protected:
    Singleton(void) = default;

    Singleton(Singleton& single) = delete;

    Singleton(Singleton&& single) = delete;

    ~Singleton(void){
        ClassInstance->~Type();
    }
private:
    static SpinLock splock;
    static Type* ClassInstance;
};

template <class Type>
Type* Singleton<Type>::ClassInstance = nullptr;
template <class Type>
SpinLock Singleton<Type>::splock;
#endif // SINGLETON_H
