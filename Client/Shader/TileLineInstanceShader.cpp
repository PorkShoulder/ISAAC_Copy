#include "pch.h"
#include "TileLineInstanceShader.h"

TileLineInstanceShader::TileLineInstanceShader()
{}

TileLineInstanceShader::~TileLineInstanceShader()
{}

bool TileLineInstanceShader::Init()
{
    if (!LoadVertexShader("TileLineInstanceVS", "TileInstance.fx"))
        return false;

    if (!LoadPixelShader("TileLineInstancePS", "TileInstance.fx"))
        return false;

    AddInputLayoutDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);

    if (!CreateInputLayout())
        return false;

    return true;
}

void TileLineInstanceShader::Destroy()
{

}
