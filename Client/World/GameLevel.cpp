#include "pch.h"
#include "GameLevel.h"
#include "RoomManager.h"
#include "../Object/Actor.h"
#include "../Object/Player.h"


bool GameLevel::Init(const std::string& path)
{
    Level::Init(path);  // 매니저 초기화

    SpawnPlayer();
        
    // RoomManager로 저장된 방 로드
    _roomManager = New<RoomManager>();
    _roomManager->Init(This<Level>());
    _roomManager->CollectRoomFiles();
    int32 count = min(_roomManager->GetRoomFileCount(),
        _roomManager->GetMaxRoomCount());
    _roomManager->GenerateLayout(count);
    _roomManager->AssignRooms();
    _roomManager->LoadAllRooms();    // TileMap + 액터 로드
    _roomManager->ActivateStartRoom();

    return true;
}
