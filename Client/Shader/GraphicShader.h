#pragma once
#include "Shader.h"
#include <vector>

//====================================================
// Dx에서 그래픽 파이프라인용 쉐이더 세트를 관리함.
// 버텍스쉐이더 + 픽셀쉐이더 + 인풋레이아웃을 묶어서 관리
//====================================================

// 정점 데이터를 어떻게 읽을지
// 어떤 Vertex Shader를 쓸지
// 어떤 Pixel Shader를 쓸지.



class GraphicShader  : public Shader
{
public:
	GraphicShader();
	~GraphicShader();

private:
	// ID3D11InputLayout
	
	ComPtr<ID3D11InputLayout> _inputLayout = nullptr;		// 버텍스 쉐이더에서 GPU가 버텍스를 어떻게 읽을지 알려주는 설명서.
	std::vector<D3D11_INPUT_ELEMENT_DESC> _descs;			// GPU에게 넘길 정점 구조 설명서들
	uint32 _inputLayoutSize = 0;							// 위 설명서에서 다음멤버는 몇 번째 바이트부터 읽을지 계산하여 누적.

	ComPtr<ID3D11VertexShader> _vs = nullptr;		// GPU에 세팅할 수 있는 버텍스 쉐이더 객체
	ComPtr<ID3DBlob> _vsBlob = nullptr;				// 버텍스 쉐이더 코드를 컴파일하고 나온 바이트 코드를 저장하기 위한 버퍼 
	ComPtr<ID3D11PixelShader> _ps = nullptr;		// 픽셀 쉐이더 객체
	ComPtr<ID3DBlob> _psBlob = nullptr;				// 픽셀 쉐이더 코드를 컴파일하고 나온 바이트 코드를 저장하기 위한 버퍼
	
public:
	virtual bool Init() override;		// 쉐이더 초기화 함수
	virtual void Destroy() override;	// 쉐이더가 갖고있는 자원들 정리함수
	virtual void SetShader() final;		// GPU에게 쉐이더 세트를 세팅하는 함수.
	void AddInputLayoutDesc(const char* semetic, uint32 sementicIndex, DXGI_FORMAT fmt, uint32 inputSlot,
		uint32 size, D3D11_INPUT_CLASSIFICATION inputSlotClass, uint32 instanceDataSteRate);
	bool CreateInputLayout();
	bool LoadVertexShader(const char* entryName, const char* fileName);
	bool LoadPixelShader(const char* entryName, const char* fileName);


};

