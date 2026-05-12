#pragma once
#include "SceneComponent.h"
#include "Animation2D.h"

//액터에 붙어서 애니메이션2D를 재생하고 관리할수 있게 해주는 컴포넌트
class SpriteComponent : public SceneComponent
{
public:
    SpriteComponent();
    virtual ~SpriteComponent();

protected:
    Ptr<class Mesh> _mesh;              // 스프라이트를 그릴 사각형 메쉬
    Ptr<class Shader> _shader;          // 스프라이트 전용 쉐이더 -> 애니메이션 UV전환 및 좌우 반전.
    Ptr<class Texture> _texture;        // 스프라이트시트 이미지
    Ptr<class Animation2D> _animation;  // 애니메이션 재생 관리.

    Ptr<class SpriteCBuffer> _spriteCBuffer;
    int32 _textureIndex = 0;
    FVector4D _tint = FVector4D(1.f, 1.f, 1.f, 1.f);

public:
    virtual bool Init(int32 id, const std::string& name, Ptr<class Actor> owner) override;
    virtual void Tick(float deltaTime) override;
    virtual void Collision(float deltaTiem) override;
    virtual void Render(float deltaTime) override;

    virtual void Destroy() override;

    virtual void DrawInspector() override; // 에디터에서 컴포넌트 속성을 ImGui로 표시하는 함수 

    void SetShader(const std::string& name);
    void SetTint(const FVector4D& tint);
    void SetTint(float x, float y, float z);
    void SetOpacity(float op);

    void SetTexture(const std::string& name, int textureIndex = 0);
    void SetTexture(Ptr<class Texture> texture, int textureIndex = 0);
    void SetTextureIndex(int32 index);

    void AddAnimSequence(const std::string& name, bool loop = false, bool reverse = false, float playTime = 1.f, float playRate = 1.f);
    void AddAnimSequence(Ptr<class Animation2DData> data, bool loop = false, bool reverse = false, float playTime = 1.f, float playRate = 1.f);
    void SetPlayTime(const std::string& name, float time);
    void SetPlayRate(const std::string& name, float rate);
    void SetLoop(const std::string& name, bool loop);
    void SetReverse(const std::string& name, bool reverse);
    void ChangeAnimation(const std::string& name);
    void SetPlay(const std::string& name, bool play);
    void SetAnimFilp(bool filp);

    Ptr<class Animation2D> CreateAnimation();
    Ptr<class Animation2D> GetAnimation();

public:
    template<typename T>
    void AddNotify(const std::string& seqName, int32 frame, T* obj, void (T::* memFunc)())
    {
        _animation->AddNotify(seqName, frame, obj, memFunc);
    }

    template<typename T>
    void AddNotify(const std::string& seqName, int32 frame, T&& func)
    {
        _animation->AddNotify(seqName, frame, std::forward<T>(func));
    }
};

