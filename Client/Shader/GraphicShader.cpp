#include "pch.h"
#include "GraphicShader.h"
#include "../Core/Device.h"
#include "../Core/DirectoryManager.h"
#include "../Common/LogManager.h"



GraphicShader::GraphicShader()
{
}

GraphicShader::~GraphicShader()
{
}

bool GraphicShader::Init()
{
	return false;
}

void GraphicShader::Destroy()
{

}

void GraphicShader::SetShader()
{
	// 컴파일된 쉐이더 세트를 GPU에 세팅하는 함수.

	Device::Instance().GetContext()->VSSetShader(_vs.Get(), nullptr, 0);    // 버텍스 쉐이더를 GPU에 세팅 
	Device::Instance().GetContext()->PSSetShader(_ps.Get(), nullptr, 0);    // 픽셀 쉐이더를 GPU에 세팅
	Device::Instance().GetContext()->IASetInputLayout(_inputLayout.Get());  // 인풋레이아웃을 GPU에 세팅. (CreateInputLayout에서 만듬.)
}

void GraphicShader::AddInputLayoutDesc(const char* semetic, uint32 sementicIndex, DXGI_FORMAT fmt, uint32 inputSlot, uint32 size, D3D11_INPUT_CLASSIFICATION inputSlotClass, uint32 instanceDataStepRate)
{
	// InputLayout
	// 버텍스를 GPU가 어떻게 해석할지 알려주는 설명서.
	D3D11_INPUT_ELEMENT_DESC desc = {};		
	desc.SemanticName = semetic;						// 시멘틱 이름	
	desc.SemanticIndex = sementicIndex;					// 같은 이름 시멘틱이 있을 경우 인덱스
	desc.Format = fmt;									// 이 데이터의 포맷.
	desc.InputSlot = inputSlot;							// 이 데이터가 어떤 버텍스버퍼에서 오는지 
	desc.InputSlotClass = inputSlotClass;				// 정점 단위 데이터인지, 인스턴스 단위 인지 확인용
	desc.InstanceDataStepRate = instanceDataStepRate;	// 인스턴스 일 때만 사용함. 
	desc.AlignedByteOffset = _inputLayoutSize;			// 정점 시작부터 이 데이터까지의 거리

	_inputLayoutSize += size;
	_descs.push_back(desc);
}

bool GraphicShader::CreateInputLayout()
{
	if (FAILED(Device::Instance().GetDevice()->CreateInputLayout(&_descs[0], (UINT)_descs.size(), _vsBlob->GetBufferPointer(), _vsBlob->GetBufferSize(), &_inputLayout)))
		return false;

	return true;
}

bool GraphicShader::LoadVertexShader(const char* entryName, const char* fileName)
{
	// fx파일을 찾아 컴파일하고 GPU가 쓸 수 있는 쉐이더 객체를 만드는 함수.
	
	// HLSL 폴더가 어디있는지 찾음.
	auto hlslPath = DirectoryManager::Instance().GetCachePath("HLSL"); 

	// HLSL 폴더가 없으면 실패 처리하고 함수 종료
	if (!hlslPath.has_value())
		return false;

	// 파일 경로나 폴더 경로를 둘 다 담을 수 있는 경로 변수
	std::filesystem::path fullPath;

	// HLSL 폴더 + fileName(.fx)
	if (!DirectoryManager::Instance().GetFile(hlslPath.value(), fileName, OUT fullPath))
		return false;

	// 컴파일 옵션을 저장할 변수
    uint32 flag = 0;
#ifdef _DEBUG
    flag = D3DCOMPILE_DEBUG;
#endif

    ID3D10Blob* errorBlob = nullptr;

    if (FAILED(D3DCompileFromFile(fullPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryName, "vs_5_0", flag, 0, _vsBlob.GetAddressOf(), &errorBlob)))
    {
#ifdef _DEBUG
        char errorText[255] = {};
        strcpy_s(errorText, (const char*)errorBlob->GetBufferPointer());
        LogManager::Instance().Debug(errorText);
#endif
        return false;
    }

    if (FAILED(Device::Instance().GetDevice()->CreateVertexShader(_vsBlob->GetBufferPointer(), _vsBlob->GetBufferSize(), nullptr, _vs.GetAddressOf())))
    {
        LogManager::Instance().Debug(__FILE__, __LINE__, "[CreateVertexShader Error]");
        return false;
    }

    return true;
}

bool GraphicShader::LoadPixelShader(const char* entryName, const char* fileName)
{
    //쉐이더 코드를 불러오고
        //해당 쉐이더코드를 컴파일 한 다음
        //컴파일 결과물로 나온 바이트 코드를 들고 있을꺼에요.
    auto hlslPath = DirectoryManager::Instance().GetCachePath("HLSL");
    if (!hlslPath.has_value())
        return false;

    std::filesystem::path fullPath;
    if (!DirectoryManager::Instance().GetFile(hlslPath.value(), fileName, OUT fullPath))
        return false;

    uint32 flag = 0;
#ifdef _DEBUG
    flag = D3DCOMPILE_DEBUG;
#endif

    ID3D10Blob* errorBlob = nullptr;

    if (FAILED(D3DCompileFromFile(fullPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryName, "ps_5_0", flag, 0, _psBlob.GetAddressOf(), &errorBlob)))
    {
#ifdef _DEBUG
        char errorText[255] = {};
        strcpy_s(errorText, (const char*)errorBlob->GetBufferPointer());
        LogManager::Instance().Debug(errorText);
#endif
        return false;
    }

    if (FAILED(Device::Instance().GetDevice()->CreatePixelShader(_psBlob->GetBufferPointer(), _psBlob->GetBufferSize(), nullptr, _ps.GetAddressOf())))
    {
        LogManager::Instance().Debug(__FILE__, __LINE__, "[CreatePixelShader Error]");
        return false;
    }

    return true;
}
