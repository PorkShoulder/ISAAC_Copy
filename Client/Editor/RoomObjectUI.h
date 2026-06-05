#pragma once
#include "RoomEditor.h"
#include "../Common/Info.h"

struct FAnimSequenceData
{
    std::string name;               // 현재 상태 
    std::string textureName;        // 텍스처 파일 명
    std::vector<FVector4D> frames;  // (startX,Y sizeX,Y)
    bool loop = true;                
};

// 타일에 배치되는 오브젝트 관리.

class RoomObjectUI : public RoomEditor
{
public:
    RoomObjectUI();
    virtual ~RoomObjectUI();

private:
    eActorType _placeType = eActorType::End; // 시작값.
    // 오브젝트 선택
    int _currentIndex = 0;
    // 애니메이션 선택 
    int _currentAnimIndex = 0;
    std::map<std::string, int> _objectNameCounter;
    //
    std::vector<FAnimSequenceData> _animSequences;
    bool _seqLoop = true;
    char _seqName[64] = "IDLE";

public:
    virtual bool Init(const std::string& name) override;
    virtual void Render(float deltaTime) override;
    virtual void Destroy() override;

private:
    std::string MakeObjectName(const std::string& typeName);

    bool IsAnimatedObject(eActorType type) const;
    void RenderAnimationSelect();
    



};