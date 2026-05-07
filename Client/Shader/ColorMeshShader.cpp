#include "pch.h"
#include "ColorMeshShader.h"

ColorMeshShader::ColorMeshShader()
{
}

ColorMeshShader::~ColorMeshShader()
{
}

bool ColorMeshShader::Init()
{
    // 쉐이더가 사용할 레이아웃, 버텍스, 픽셀 쉐이더를 초기화하는 역할.
    //ColorMesh용 쉐이더라고 세팅함.

    if (!LoadVertexShader("ColorMeshVS", "ColorMesh.fx"))
        return false;

    if (!LoadPixelShader("ColorMeshPS", "ColorMesh.fx"))
        return false;

    AddInputLayoutDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
    AddInputLayoutDesc("COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0);

    if (!CreateInputLayout())
        return false;

    return true;
}
