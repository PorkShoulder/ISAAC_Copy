#include "pch.h"
#include "Device.h"
// #include "../Common/" 로그매니저


FVector2D Device::GetRSRate() const
{
    return FVector2D();
}

bool Device::Init(HWND _hwnd, uint32 _widht, uint32 _height, bool _windowMode)
{
    return false;
}

void Device::ClearBackBuffer(float _clearColor[4])
{
}

void Device::ClearDepthStencil(float _depth, uint8 _stencil)
{
}

void Device::SetTarget()
{
}

void Device::Render()
{
}

void Device::Destroy()
{
}
