#pragma once
#include "Core/Object.h"

class TestObject :
    public Object
{
public:
    TestObject();
    ~TestObject();

public:
    int Param = 0;

public:
    virtual void Destroy() override;
};

