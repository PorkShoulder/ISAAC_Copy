#include "pch.h"
#include "SpriteComponent.h"

#include "../Core/Texture.h"
#include "../Core/AssetManager.h"
#include "../Core/Mesh.h"

#include "../Shader/ShaderManager.h"
// #include "../Shader/SpriteCBuffer.h"
// #include "../Shader/SpriteShader.h"
#include "../Shader/TranformCBuffer.h"

#include "../World/Level.h"

// #include "Editor/EditorEngine.h"

SpriteComponent::SpriteComponent()
{
    _isRender = true;
}

SpriteComponent::~SpriteComponent()
{
}

bool SpriteComponent::Init(int32 id, const std::string& name, Ptr<class Actor> owner)
{
    SceneComponent::Init(id, name, owner);

    _spriteCBuffer = ShaderManager::Instance().FindCBuffer<SpriteCBuffer>("Sprite");

    SetShader("SpriteShader");

    _mesh = MESH_MANAGER->FindMesh("TexRect");

    return true;
}

void SpriteComponent::Tick(float deltaTime)
{
    SceneComponent::Tick(deltaTime);

    if (_animation)
        _animation->Tick(deltaTime);

}

void SpriteComponent::Collision(float deltaTiem)
{
    SceneComponent::Collision(deltaTiem);
}

void SpriteComponent::Render(float deltaTime)
{
    SceneComponent::Render(deltaTime);

    if (_animation)
        _animation->SetShader();

    ShaderManager::Instance().SetSample(eTextureSampleType::TEXTURE_SAMPLE_LINEAR);

    _spriteCBuffer->SetTint(_tint);
    _spriteCBuffer->Update();

    _transformCBuffer->SetWorldMatrix(_matrix._world);

    Ptr<Level> level = Lock<Level>(_level);
    if (nullptr == level)
        return;

    FMatrix view = level->GetViewMatrix();
    FMatrix proj = level->GetProjMatrix();

    _transformCBuffer->SetViewMatrix(view);
    _transformCBuffer->SetProjMatrix(proj);

    _transformCBuffer->Update();

    _shader->SetShader();

    if (_texture)
        _texture->SetShader(0, SHADER_TYPE::PIXEL, _textureIndex);

    _mesh->Render();
}

void SpriteComponent::Destroy()
{
    SceneComponent::Destroy();

    DESTROY(_animation);
}

void SpriteComponent::DrawInspector()
{
    ImGui::SeparatorText("SpriteComponent");

    std::string label = std::format("World Transform##{0}", GetComponentID());
    ImGui::SeparatorText(label.c_str());

    FVector3D worldPos = GetWorldPosition();
    std::string posLabel = std::format("World Position##{0}", GetComponentID());
    if (ImGui::DragFloat3(posLabel.c_str(), &worldPos._x, 0.5f))
        SetWorldPosition(worldPos);

    FVector3D worldRot = GetWorldRotation();
    std::string rotLabel = std::format("World Rotation##{0}", GetComponentID());
    if (ImGui::DragFloat3(rotLabel.c_str(), &worldRot._x, 0.5f))
        SetWorldRotation(worldRot);

    FVector3D worldScale = GetWorldScale();
    std::string scaleLabel = std::format("World Scale##{0}", GetComponentID());
    if (ImGui::DragFloat3(scaleLabel.c_str(), &worldScale._x, 0.5f))
        SetWorldScale(worldScale);


    std::string rlabel = std::format("Relative Transform##{0}", GetComponentID());
    ImGui::SeparatorText(rlabel.c_str());

    FVector3D rPos = GetRelativePosition();
    std::string rPosLabel = std::format("Relative Position##{0}", GetComponentID());
    if (ImGui::DragFloat3(rPosLabel.c_str(), &rPos._x, 0.5f))
        SetRelativePosition(rPos);

    FVector3D rRot = GetRelativeRotation();
    std::string rrotLabel = std::format("Relative Rotation##{0}", GetComponentID());
    if (ImGui::DragFloat3(rrotLabel.c_str(), &rRot._x, 0.5f))
        SetRelativeRotation(rRot);

    FVector3D rScale = GetWorldScale();
    std::string rscaleLabel = std::format("Relative Scale##{0}", GetComponentID());
    if (ImGui::DragFloat3(rscaleLabel.c_str(), &rScale._x, 0.5f))
        SetRelativeScale(rScale);

    ImGui::SeparatorText("Tint");
    if (ImGui::ColorEdit4("Tint", &_tint._x))
        SetTint(_tint);

    if (_animation)
        _animation->DrawInspector();
}

void SpriteComponent::SetShader(const std::string& name)
{
    _shader = ShaderManager::Instance().FindShader(name);
}

void SpriteComponent::SetTint(const FVector4D& tint)
{
    _tint = tint;
}

void SpriteComponent::SetTint(float r, float g, float b)
{
    _tint._x = r;
    _tint._y = g;
    _tint._z = b;
}

void SpriteComponent::SetOpacity(float op)
{
    _tint._w = op;
}

void SpriteComponent::SetTexture(const std::string& name, int textureIndex)
{
    _texture = TEXTURE_MANAGER->Findtexture(name);
    _textureIndex = textureIndex;
}

void SpriteComponent::SetTexture(Ptr<class Texture> texture, int textureIndex)
{
    _texture = texture;
    _textureIndex = textureIndex;
}

void SpriteComponent::SetTextureIndex(int32 index)
{
    _textureIndex = index;
}

void SpriteComponent::AddAnimSequence(const std::string& name, bool loop, bool reverse, float playTime, float playRate)
{
    if (!_animation)
        _animation = CreateAnimation();

    if (_animation)
        _animation->AddSequeunce(name, loop, reverse, playTime, playRate);
}

void SpriteComponent::AddAnimSequence(Ptr<class Animation2DData> data, bool loop, bool reverse, float playTime, float playRate)
{
    if (!_animation)
        _animation = CreateAnimation();

    if (_animation)
        _animation->AddSequeunce(data, loop, reverse, playTime, playRate);
}

void SpriteComponent::SetPlayTime(const std::string& name, float time)
{
    if (!_animation)
        _animation = CreateAnimation();

    if (_animation)
        _animation->SetPlayTime(name, time);
}

void SpriteComponent::SetPlayRate(const std::string& name, float rate)
{
    if (!_animation)
        _animation = CreateAnimation();

    if (_animation)
        _animation->SetPlayRate(name, rate);
}

void SpriteComponent::SetLoop(const std::string& name, bool loop)
{
    if (!_animation)
        _animation = CreateAnimation();

    if (_animation)
        _animation->SetLoop(name, loop);
}

void SpriteComponent::SetReverse(const std::string& name, bool reverse)
{
    if (!_animation)
        _animation = CreateAnimation();

    if (_animation)
        _animation->SetReverse(name, reverse);
}

void SpriteComponent::ChangeAnimation(const std::string& name)
{
    if (!_animation)
        _animation = CreateAnimation();

    if (_animation)
        _animation->ChangeAnimation(name);
}

void SpriteComponent::SetPlay(const std::string& name, bool play)
{
    if (!_animation)
        _animation = CreateAnimation();

    if (_animation)
        _animation->SetPlay(name, play);
}

void SpriteComponent::SetAnimFilp(bool filp)
{
    if (!_animation)
        _animation = CreateAnimation();

    if (_animation)
        _animation->SetAnimFilp(filp);
}

Ptr<class Animation2D> SpriteComponent::CreateAnimation()
{
    _animation = New<Animation2D>();
    _animation->_owner = This<SpriteComponent>();

    if (!_animation->Init())
    {
        DESTROY(_animation);
        return nullptr;
    }

    return _animation;
}

Ptr<class Animation2D> SpriteComponent::GetAnimation()
{
    return _animation;
}
