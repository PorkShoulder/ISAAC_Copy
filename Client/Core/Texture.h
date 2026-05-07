#pragma once
#include "Asset.h"
#include "DirectXTex.h"

//=========================================================
// 이미지 파일을 읽어서 GPU용 텍스처 리소스 SRV를 만들고,
// 쉐이더가 그 텍스처를 읽을 수 있게 슬롯에 연결하는 클래스
//=========================================================

struct FTextureInfo
{
	DirectX::ScratchImage _image;			// cpu메모리에 로드된 이미지 원본 데이터 
	ComPtr<ID3D11ShaderResourceView> _srv;	// gpu가 쉐이더에서 텍스처를 읽을 수 있게해주는 뷰
	uint32 _width = 0;						// 가로세로 크기 
	uint32 _height = 0;						// 가로세로 크기
	std::wstring _fileName;					// 로드한 파일 이름
};


class Texture : public Asset
{
public:
	Texture() = default;
	virtual ~Texture() = default;

protected:
	std::vector<FTextureInfo> _textureInfos;		 // 하나의 텍스쳐 객체가 여러 장의 이미지를 가질 수 있음.
public:
	const FTextureInfo* GetTexture(int32 idx) const; // idx번째 텍스처 정보를 반환. const포인터로 돌려주니까 외부에서 수정x
	bool LoadTexture(const std::wstring& fileName);	 // 이미지 파일을 읽어서 ScratchImahe에 저장 후 SRV로 만든다.
	
	void SetShader(int32 registerNum, int32 shaderBufferType, int32 textureIndex);
	void ResetShader(int32 registerNum, int32 shaderBufferType);

	virtual void Destroy() override;

protected:
	bool CreateResourceView(int32 idx);





};

