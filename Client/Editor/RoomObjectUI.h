#pragma once
#include "RoomEditor.h"
#include "../Common/Info.h"


struct FAnimSequenceData
{
    std::string name;           // 실제 등록용 고유 이름
    std::string displayName;    // UI 표시용 이름: IDLE, MOVE, ATTACK
    std::string textureName;
    std::wstring texturePath;
    std::vector<FVector4D> frames;
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
    bool _seqLoop = true;
    char _seqName[64] = "IDLE";
    std::vector<FAnimSequenceData> _animSequences;

public:
    virtual bool Init(const std::string& name) override;
    virtual void Render(float deltaTime) override;
    virtual void Destroy() override;

private:
    std::string MakeObjectName(const std::string& typeName);
    bool IsAnimatedObject(eActorType type) const;
    void RenderAnimationSelect();
    void RenderActorList();
    // Door
    FDoorSpawnData _editDoorData;
    void RenderDoorUI();
    // Monster
    FMonsterData _editMonsterData;
    void RenderMonsterUI();
    void RenderFrameList(const char* label, std::vector<FVector4D>& frames);
    
    //Obstacle
    FObstacleData _editObstacleData;
    void RenderObstacleUI();
    
    //Item
    FItemData _editItemData;
    void RenderItemUI();

    //Npc
    FNpcData _editNpcData;
    void RenderNpcUI();

};