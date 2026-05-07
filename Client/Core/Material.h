#pragma once
#include "Asset.h"
#include "Texture.h"
#include <vector>

//======================================
// 텍스처+색상+투명도+쉐이더 -> 오브젝트
//=====================================

struct FMaterialTextureInfo
{
	std::string _name;				// TextureManager에서 찾을때 쓰는 키.
	Weak<class Texture> _texture;	// 약참조-> 실 소유권은 매니저쪽에서 
	int32 _registerNum;				// share.fx의 레지스터 번호
	int32 _shaderBufferType;		// 버퍼 타입 구분용
	int32 _textureIndex;			// 여러장을 가질수 있으니 몇번째인지.
};


class Material :
    public Asset
{
    friend class MaterialManager;

public:
    Material();
    virtual ~Material();

protected:
    std::vector<FMaterialTextureInfo> _textures;    // 기본텍스처 + 노멀맵
    Ptr<class Shader> _pixelShader;                 //재질마다 다른 픽셀쉐이더를 쓸 수도 있어서
    Ptr<class MaterialCBuffer> _buffer;             //재질마다 상수버퍼 데이터가 다를 수 도 있어서.
    eTextureSampleType _sampleType = eTextureSampleType::TEXTURE_SAMPLE_LINEAR; //gpu로 정보 전달 
    FVector4D _baseColor = FVector4D(1.f, 1.f, 1.f, 1.f); //색깔
    float _opacity = 1.f; //투명도

public:
    // 재질 속성 설정 Set
    void SetBaseColor(float r, float g, float b, float a);
    void SetBaseColor(const FVector4D& color);
    void SetOpacity(float op);
    void SetSamplerType(eTextureSampleType type);
    // 텍스처를 이 Material에 연결. Ptr 이나 string
    void AddTexture(Ptr<class Texture> texture, 
        int32 registerNum, 
        int32 shaderBufferType = SHADER_TYPE::PIXEL, 
        int32 textureIndex = 0);
    void AddTexture(const std::string& name, 
        int32 registerNum, 
        int32 shaderBufferType = SHADER_TYPE::PIXEL, 
        int32 textureIndex = 0);
    // 렌더링 할 때 호출 텍스처들을 쉐이더 슬롯에 바인딩하고 샘플러 설정.
    void SetPixelShader(const std::string& name);
    void SetMaterial();
    void ResetMaterial();

    Ptr<Material> Clone(); // 같은 재질을 복사해서 새로만듬.

    virtual void Destroy() override;
};

