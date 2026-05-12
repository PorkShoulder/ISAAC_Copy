#pragma once
#include "Common/Info.h"
#include "Defines.h"

#ifdef _USE_MEMORY_POOL
#include "MemoryPool.h"

class Object
{
    friend class ObjectPool;

public:
    Object() {}
    ~Object() {}

protected:
    FSegment _Segment;
    bool _bEnable = false; //살아는 있지만 활성화 여부
    bool _bActive = false; //살았는지 죽었는지 여부

public:
    bool IsActive() const { return _bActive; }
    bool IsEnable() const { return _bEnable; }
    void SetActive(bool active) { _bActive = active; }
    void SetEnable(bool enable) { _bEnable = enable; }

    virtual void Destroy() = 0;

    void* operator new(size_t size)
    {
        FSegment seg = MemoryPool::Instance().Rent((int32)size);
        return seg._Data;
    }

    void operator delete(void* ptr) noexcept
    {
        MemoryPool::Instance().Return(ptr);
    }

    template<typename T>
    Ptr<T> This()
    {
        return Cast<Object, T>(this);
    }
};
#elif _USE_OBJECT_POOL
class Object
{
    friend class ObjectPool;

public:
    Object() {}
    ~Object() {}

protected:
    bool _bEnable = false; //살아는 있지만 활성화 여부
    bool _bActive = false; //살았는지 죽었는지 여부
    bool _IsPooled = false; //얘가 풀에서 생성한 객체인지 여부
    eObjectType _Type = eObjectType::END;

public:
    bool IsActive() const { return _bActive; }
    bool IsEnable() const { return _bEnable; }
    void SetActive(bool active) { _bActive = active; }
    void SetEnable(bool enable) { _bEnable = enable; }
    eObjectType GetType() const { return _Type; }

    void Pool() { _IsPooled = true; }

    virtual void Destroy() = 0;

    template<typename T>
    Ptr<T> This()
    {
        return Cast<Object, T>(this);
    }
};
#else

#include <memory>
class Object : public std::enable_shared_from_this<Object>
{
    //friend class ObjectPool;

public:
    Object() {}
    ~Object() {}

protected:
    bool _bEnable = false; //살아는 있지만 활성화 여부
    bool _bActive = false; //살았는지 죽었는지 여부

public:
    bool IsActive() const { return _bActive; }
    bool IsEnable() const { return _bEnable; }
    void SetActive(bool active) { _bActive = active; }
    void SetEnable(bool enable) { _bEnable = enable; }

    virtual void DrawInspector();


    virtual void Destroy() = 0;

    template<typename T>
    Ptr<T> This()
    {
        return Cast<Object, T>(shared_from_this());
    }
};
#endif



