#pragma once
#include <memory>

#define DESTROY(ptr) if(ptr) { ptr->Destroy(); Delete(ptr);}

template<typename T>
using Ptr = std::shared_ptr<T>; 

template<typename T>
using Weak = std::weak_ptr<T>; 

template<typename T, typename ...Args>
Ptr<T> New(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T>
Weak<T> MakeWeak(Ptr<T>& ptr)
{
    return ptr;
}

template<typename T>
void Delete(Ptr<T>& ptr)
{
    ptr = nullptr;
}

template<typename Src, typename Dest>
Ptr<Dest> Cast(Ptr<Src> src)
{
    return std::dynamic_pointer_cast<Dest>(src);
}

template<typename T>
Ptr<T> Lock(Weak<T> ptr)
{
    return ptr.lock();
}

//
#define SINGLETON(className) \
    friend class Singleton<className>;\
protected: \
    className() = default;\
    ~className() = default;\
    className(const className&) = delete;\
    className(className&&) = delete;\
    className& operator = (const className&) = delete;\
    className& operator = (className&&) = delete;\



