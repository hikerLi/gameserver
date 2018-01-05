#ifndef MEMORYPOOL_H
#define MEMORYPOOL_H

#include <Common/spinlock.h>
#include <mutex>
#include <Common/singleton.h>
#include <jemalloc/jemalloc.h>
namespace MemoryPool{

    class MemoryPoolObj final: public Singleton<MemoryPoolObj>
    {

#define ElementType EType
#define Pointer ElementType*
    public:
        template<class ElementType, class... Args>
        Pointer newElement(Args&& ...args){
            std::unique_lock<decltype(splock)> lck(splock);
            Pointer p = (Pointer)malloc(sizeof(ElementType));
            construct(p, std::forward<Args>(args)...);
            return p;
        }

        template<class ElementType>
        void delElement(Pointer p){
            std::unique_lock<decltype(splock)> lck(splock);
            destroy(p);
        }

        template<class ElementType>
        Pointer newArray(int number){
            std::unique_lock<decltype(splock)> lck(splock);
            Pointer p = (Pointer)malloc(sizeof(ElementType) * number);
            return p;
        }

        void delArray(void *p){
            std::unique_lock<decltype(splock)> lck(splock);
            free(p);
        }
    private:
        template<class ElementType, class... Args>
        void construct(Pointer p, Args&& ...args){
            new (p) ElementType(std::forward<Args>(args)...);
        }

        template<class ElementType>
        void destroy(Pointer p){
            p->~ElementType();
            free(p);
        }
    private:
        SpinLock splock;
    };

}

#define MemPoolIns MemoryPool::MemoryPoolObj::GetInstance()
#endif // MEMORYPOOL_H
