#include "pch.h"
#include "TileSBuffer.h"

TileSBuffer::TileSBuffer()
{}

TileSBuffer::~TileSBuffer()
{}

void TileSBuffer::Update()
{
    if (_datas.empty())
        return;

    int32 count = GetCount();
    Resize(count);
    SetData(_datas.data(), count);
}
