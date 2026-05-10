#include "pch.h"
#include "TileMapCBuffer.h"

TileMapCBuffer::TileMapCBuffer()
{}

TileMapCBuffer::~TileMapCBuffer()
{}

void TileMapCBuffer::Update()
{
    SetData(&_data);
}
