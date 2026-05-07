#pragma once
#include "Core/Object.h"
#include "SBufferData.h"

//구조화 버퍼
//gpu에 대용량 데이터를 보낼때 사용
//tN..
//맞춤X

//상수버퍼는 총 용량이 64kb
//16바이트 맞춤

class SBuffer : public Object
{
public:
    SBuffer();
    virtual ~SBuffer();

protected:
    ComPtr<ID3D11Buffer> _buffer;
    ComPtr<ID3D11ShaderResourceView> _srv;
    int32 _size = 0;
    int32 _elementCount = 0;
    int32 _register = 0;
    int32 _type = 0;

public:
    virtual void Update() = 0;
    bool Create(int32 size, int32 eleCnt, int32 regi, int32 type);
    void Bind();

protected:
    void SetData(void* data, int32 eleCnt);
    void Resize(int32 eleCnt);

public:
    virtual void Destroy() override;
};

