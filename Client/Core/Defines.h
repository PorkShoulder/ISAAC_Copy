#pragma once
#include <memory>

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




