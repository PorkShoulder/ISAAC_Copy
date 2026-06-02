#pragma once
#include "RoomEditor.h"
#include "../Common/Info.h"

// 타일에 배치되는 오브젝트 관리.

class RoomObjectUI : public RoomEditor
{
public:
    RoomObjectUI();
    virtual ~RoomObjectUI();

private:
    eActorType _placeType = eActorType::End; // 시작값.
    int _currentIndex = 0;
    
    std::map<std::string, int> _objectNameCounter;


public:
    virtual bool Init(const std::string& name) override;
    virtual void Render(float deltaTime) override;
    virtual void Destroy() override;

private:
    std::string MakeObjectName(const std::string& typeName);

    //void SpawnActor(const FVector3D& pos, const FVector3D& scale, const FVector3D& rot, eActorType type, )



};