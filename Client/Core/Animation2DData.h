#pragma once
#include "Asset.h"


//애니메이션 한장한장의 데이터
//2가지 --> sprite, frame
//낱장           ---> 낱장일때는 텍스쳐 여러장을 로드하고 그것을 출력해주는 방식으로
//아틀라스 이미지 ---> frame을 통해 애니메이션 한장한장의 좌표값으로 출력을 하기 위함
class Animation2DData : public Asset
{
public:
    Animation2DData();
    virtual ~Animation2DData();

private:
    Ptr<class Texture> _texture;
    eAnimTextureType _type = eAnimTextureType::END;
    std::vector<FAnimationFrame> _frames;

public:
    //애니메이션 현재 타입 가져오기 frame? sprite?
    eAnimTextureType GetAnimTextureType() const;

    //텍스쳐 가져오기(그림)
    Ptr<class Texture> GetTexture() const;

    //프레임 가져오기
    const FAnimationFrame& GetFrame(int32 index) const;

    //몇 프레임 짜리 애니메이션인지?
    int32 GetFrameCount() const;

    void SetAnimationTextureType(eAnimTextureType type);
    void SetTexture(const std::string& name);
    void SetTexture(Ptr<class Texture> texture);
    void SetTexture(const std::string& name, const std::wstring& fileName);

    //아틀라스 이미지일때, 큰 아틀라스 이미지에서
    //좌표를 통해 어떤 이미지를 출력할지 정해주는 부분
    void AddFrame(const FVector2D& start, const FVector2D& size);
    void AddFrame(float startX, float startY, float sizeX, float sizeY);

    virtual void Destroy() override;
};

