#include "pch.h"
#include "CBuffer.h"
#include "../Core/Device.h"
#include "../Common/Info.h"

CBuffer::CBuffer()
{
}

CBuffer::~CBuffer()
{
}

bool CBuffer::Create(int size, int regi, int type)
{
    _size = size;
    _register = regi;
    _type = type;

    
    D3D11_BUFFER_DESC desc = {};
    desc.Usage = D3D11_USAGE_DYNAMIC;           // 매 프레임 데이터가 바뀔수 있으니 동적 버퍼로 만든다.
    desc.ByteWidth = _size;                     // 버퍼 크기
    desc.ByteWidth = D3D11_CPU_ACCESS_WRITE;    // 버퍼 용도
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // 버퍼에 CPU가 무슨일을 할 수 있는지
   
    if (FAILED(Device::Instance().GetDevice()->CreateBuffer(&desc, nullptr, _buffer.GetAddressOf())))
        return false;

    return true;
}

void CBuffer::SetData(void* data)
{
    // cpu데이터를gpu로 복사하고 쉐이더에 세팅
    D3D11_MAPPED_SUBRESOURCE map = {};
    
    // gpu리소스에 cpu가 접근할 수 있도록 포인터를 얻어오는 함수
    CONTEXT->Map(_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
    
    //data->cpu데이터 를 map.pData(gpu메모리)에 _size만큼 복사함.
    memcpy(map.pData, data, _size);

    // 복사완료 후 gpu버퍼를 닫음 Map로 열도 Unmap로 닫음
    CONTEXT->Unmap(_buffer.Get(), 0);

    //복사한 데이터를 쉐이더에 세팅
    if (_type & SHADER_TYPE::VERTEX)
        CONTEXT->VSSetConstantBuffers(_register/*상수버퍼를 바인딩할 레지스터 번호*/, 1, _buffer.GetAddressOf());
    if (_type & SHADER_TYPE::PIXEL)
        CONTEXT->PSSetConstantBuffers(_register/*상수버퍼를 바인딩할 레지스터 번호*/, 1, _buffer.GetAddressOf());
    

}

void CBuffer::Update()
{
}

void CBuffer::Destroy()
{
}
