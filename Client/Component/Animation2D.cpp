#include "pch.h"
#include "Animation2D.h"
#include "SpriteComponent.h"

#include "../Core/Animation2DData.h"
#include "../Core/AssetManager.h"
#include "../Core/Texture.h"

#include "../Shader/ShaderManager.h"
#include "../Shader/AnimCBuffer.h"

#include "../Editor/EditorEngine.h"

Animation2D::Animation2D()
{}

Animation2D::~Animation2D()
{}

void Animation2D::DrawInspector()
{
    ImGui::SeparatorText("Animation2D");

    std::vector<std::string> seqStr;
    for (auto& it : _sequences)
        seqStr.push_back(it.second->GetName());

    static int item_seleted_index = 0;
    int prev_index = item_seleted_index;
    if(ImGui::BeginCombo("AnimSequence", seqStr[item_seleted_index].c_str()))
    {
        size_t size = seqStr.size();
        for (size_t i = 0; i < size; ++i)
        {
            const bool is_seleted = (item_seleted_index == i);
            if (ImGui::Selectable(seqStr[i].c_str(), is_seleted))
                item_seleted_index = i;

            if (is_seleted)
                ImGui::SetItemDefaultFocus();
        }

        ImGui::EndCombo();
    }

    const std::string seleteSeq = seqStr[item_seleted_index];

    Ptr<Animation2DSequence> foundSeq = FindSequence(seleteSeq);
    if (foundSeq)
    {
        foundSeq->DrawInspector();
        ChangeAnimation(seleteSeq);
    }
}

void Animation2D::SetAnimFilp(bool filp)
{
    _filp = filp;
}

bool Animation2D::Init()
{
    _animBuffer = ShaderManager::Instance().FindCBuffer<AnimCBuffer>("Anim");

    return true;
}

void Animation2D::Tick(float deltaTime)
{
    if (!_currentSequence)
        return;
    _currentSequence->Tick(deltaTime);
}

bool Animation2D::AddSequeunce(const std::string& name, bool loop, bool reverse, float playTime, float playRate)
{
    Ptr<Animation2DSequence> seq = FindSequence(name);
    if (seq)
        return false;

    Ptr<Animation2DData> data = ANIMATION_MANAGER->FindAnimation(name);
    if (!data)
        return false;

    seq = New<Animation2DSequence>();
    seq->_owner = This<Animation2D>();
    seq->SetData(data);
    seq->SetPlayTime(playTime);
    seq->SetPlayRate(playRate);
    seq->SetLoop(loop);
    seq->SetReverse(reverse);
    seq->_frame = 0;
    seq->_time = 0;

    if (!_currentSequence)
    {
        _currentSequence = seq;

        if (Ptr<SpriteComponent> comp = Lock<SpriteComponent>(_owner))
            comp->SetTexture(_currentSequence->GetAnimation2DData()->GetTexture());
    }

    _sequences[name] = seq;

    return true;
}

bool Animation2D::AddSequeunce(Ptr<class Animation2DData> data, bool loop, bool reverse, float playTime, float playRate)
{
    Ptr<Animation2DSequence> seq = FindSequence(data->GetName());
    if (seq)
        return false;

    seq = New<Animation2DSequence>();
    seq->_owner = This<Animation2D>();
    seq->SetData(data);
    seq->SetPlayTime(playTime);
    seq->SetPlayRate(playRate);
    seq->SetLoop(loop);
    seq->SetReverse(reverse);
    seq->_frame = 0;
    seq->_time = 0;

    if (!_currentSequence)
    {
        _currentSequence = seq;

        if (Ptr<SpriteComponent> comp = Lock<SpriteComponent>(_owner))
            comp->SetTexture(_currentSequence->GetAnimation2DData()->GetTexture());
    }

    _sequences[data->GetName()] = seq;

    return true;
}

void Animation2D::SetPlayTime(const std::string& name, float playTime)
{
    Ptr<Animation2DSequence> seq = FindSequence(name);
    if (!seq)
        return;

    seq->SetPlayTime(playTime);
}

void Animation2D::SetPlayRate(const std::string& name, float playRate)
{
    Ptr<Animation2DSequence> seq = FindSequence(name);
    if (!seq)
        return;

    seq->SetPlayRate(playRate);
}

void Animation2D::SetLoop(const std::string& name, bool loop)
{
    Ptr<Animation2DSequence> seq = FindSequence(name);
    if (!seq)
        return;

    seq->SetLoop(loop);
}

void Animation2D::SetReverse(const std::string& name, bool reverse)
{
    Ptr<Animation2DSequence> seq = FindSequence(name);
    if (!seq)
        return;

    seq->SetReverse(reverse);
}

void Animation2D::SetPlay(const std::string& name, bool play)
{
    Ptr<Animation2DSequence> seq = FindSequence(name);
    if (!seq)
        return;

    seq->SetPlay(play);
}

void Animation2D::ChangeAnimation(const std::string& name)
{
    if (!_currentSequence)
        return;

    if (_sequences.size() < 1)
        return;

    if (_currentSequence->GetName() == name)
        return;


    Ptr<Animation2DSequence> seq = FindSequence(name);
    if (!seq)
        return;

    //이전 시퀀스의 시간을 초기화 한다.
    //_currentSequence->_frame = 0;
    //_currentSequence->_time = 0;

    //바꿀 시퀀스를 현재 시퀀스로 지정한다.
    //지정한 후에는 처음부터 재생할 수 있도록
    //프레임과 시간을 초기화 시켜준다.
    _currentSequence = seq;
    _currentSequence->_frame = 0;
    _currentSequence->_time = 0;
    _currentSequence->SetPlay(true);

    Ptr<SpriteComponent> comp = Lock<SpriteComponent>(_owner);
    if (comp)
        comp->SetTexture(_currentSequence->GetAnimation2DData()->GetTexture());
}

void Animation2D::SetShader()
{
    float LTX = 0.f;
    float LTY = 0.f;
    float RBX = 1.f;
    float RBY = 1.f;

    eAnimTextureType type = _currentSequence->_data->GetAnimTextureType();

    Ptr<Texture> texture = _currentSequence->_data->GetTexture();

    Ptr<SpriteComponent> comp = Lock<SpriteComponent>(_owner);
    if (nullptr == comp)
        return;

    switch (type)
    {
    case eAnimTextureType::SPRITE:
    {
        const FAnimationFrame& frame = _currentSequence->_data->GetFrame(_currentSequence->GetFrame());

        LTX = frame._start._x / texture->GetTexture(0)->_width;
        LTY = frame._start._y / texture->GetTexture(0)->_height;

        RBX = LTX + frame._size._x / texture->GetTexture(0)->_width;
        RBY = LTY + frame._size._y / texture->GetTexture(0)->_height;

        comp->SetTextureIndex(0);
    }
    break;
    case eAnimTextureType::FRAME:
    {
        comp->SetTextureIndex(_currentSequence->_frame);
    }
    break;
    case eAnimTextureType::END:
        break;
    default:
        break;
    }

    _animBuffer->SetUV(LTX, LTY, RBX, RBY);
    _animBuffer->SetAnimFilp(_filp);

    _animBuffer->Update();
}

void Animation2D::Destroy()
{
    //
}

Ptr<Animation2DSequence> Animation2D::FindSequence(const std::string& name)
{
    auto it = _sequences.find(name);
    if (_sequences.end() == it)
        return nullptr;

    return it->second;
}
