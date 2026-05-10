#include "pch.h"
#include "Animation2DSequence.h"

#include "GameEngine.h"

Animation2DSequence::Animation2DSequence()
{}

Animation2DSequence::~Animation2DSequence()
{}

Ptr<class Animation2DData> Animation2DSequence::GetAnimation2DData() const
{
    return _data;
}

const std::string& Animation2DSequence::GetName() const
{
    return _data->GetName();
}

void Animation2DSequence::SetData(Ptr<class Animation2DData> data)
{
    _data = data;
}

void Animation2DSequence::SetPlayTime(float playTime)
{
    _playTime = playTime;

    if (_data)
        _frameTime = playTime / _data->GetFrameCount();
}

void Animation2DSequence::SetPlayRate(float rateTime)
{
    _playRate = rateTime;
}

void Animation2DSequence::SetLoop(bool loop)
{
    _loop = loop;
}

void Animation2DSequence::SetReverse(bool reverse)
{
    _reverse = reverse;
}

void Animation2DSequence::SetPlay(bool play)
{
    _play = play;
}

void Animation2DSequence::DrawInspector()
{
    //ImGui::SeparatorText("AnimSequence");

    ImGui::Checkbox("Play", &_play);
    ImGui::Checkbox("Reverse", &_reverse);
    ImGui::Checkbox("Loop", &_loop);

    ImGui::DragFloat("PlayTime", &_playTime, 0.1f, 0.f, 100.f);
    ImGui::DragFloat("PlayRate", &_playRate, 0.1f, 0.f, 10.f);
}

void Animation2DSequence::Tick(float deltaTime)
{
    //재생중이 아니라면
    if (!_play)
        return;

    _time += deltaTime * _playRate;

    if (_time >= _frameTime)
    {
        _time -= _frameTime;

        if (_reverse)
        {
            --_frame;

            if (_frame < 0)
            {
                if (_loop)
                {
                    _frame = _data->GetFrameCount() - 1; //다시 끝으로 지정
                }
                else
                {
                    _frame = 0;
                    SetPlay(false);
                }
            }
        }
        else
        {
            //진행한 애니메이션 프레임이 총 프레임 개수보다 크거나 같다면
            //이전 애니메이션 프레임이 마지막 프레임이였다는 의미이므로
            //프레임을 마지막 프레임 혹은 첫번째 프레임으로 바꾼다.

            ++_frame;

            if (_frame == _data->GetFrameCount() - 1)
            {
                if (_loop)
                    _frame = 0;
                else
                {
                    _frame = _data->GetFrameCount() - 1;
                    SetPlay(false);
                }
            }
        }

        //현재 프레임에 걸려있는 노티파이를 호출
        InvokeNotify();
    }
}

void Animation2DSequence::Destroy()
{

}

void Animation2DSequence::InvokeNotify()
{
    auto it = _notifyes.find(_frame);
    if (_notifyes.end() == it)
        return;

    FAnimation2DNofity& itVal = it->second;
    size_t size = itVal._funcs.size();
    for (size_t i = 0; i < size; ++i)
    {
        if (itVal._funcs[i])
            itVal._funcs[i]();
    }
}
