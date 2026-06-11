#include "pch.h"
#include "RoomManager.h"
#include "Level.h"


#include "../World/World.h"

#include "../Object/TileMap.h"
#include"../Object/Player.h"

#include "../Core/DirectoryManager.h"
#include "../Core/GameEngine.h"

#include "../Component/TileComponent.h"




RoomManager::RoomManager()
{
}

RoomManager::~RoomManager()
{
}



void RoomManager::Init(Ptr<class Level > level)
{
	// 초기화 레벨 참조 저장, 방 크기 설정.
	_level = level;
    // To do 타일 자동으로 계산되도록 향후 변경
	_roomWorldWidth =780.f;
	_roomWorldHeight =468.f;
}

void RoomManager::CollectRoomFiles()
{
    _roomFiles.clear();

    // Resources 캐시 경로 가져오기
    auto resPath = DirectoryManager::Instance().GetCachePath("Resources");
    if (!resPath.has_value())
        return;

    // Resources/Room 폴더 경로 가져오기
    std::filesystem::path roomDir;
    if (!DirectoryManager::Instance().GetDirectory(resPath.value(), "Room", roomDir))
        return;

    // Room 폴더 내 파일 순회
    for (auto& entry : std::filesystem::directory_iterator(roomDir))
    {
        // 파일인지 확인 + .room 확장자인지 확인
        if (DirectoryManager::Instance().IsFile(entry.path()) &&
            DirectoryManager::Instance().IsExtension(entry.path(), ".room"))
        {
            FRoomFileEntry roomEntry;
            roomEntry.filePath = entry.path().string();

            // gridW, gridH 미리 읽기
            std::ifstream tempFile(entry.path(), std::ios::binary);
            if (tempFile.is_open())
            {
                int32 roomCount = 0;
                tempFile.read((char*)&roomCount, sizeof(int32));

                // 이름 길이 + 이름 스킵
                int32 nameLen = 0;
                tempFile.read((char*)&nameLen, sizeof(int32));
                tempFile.seekg(nameLen, std::ios::cur);

                // 위치(FVector3D) 스킵
                tempFile.seekg(sizeof(FVector3D), std::ios::cur);

                // gridW, gridH 읽기
                tempFile.read((char*)&roomEntry.gridW, sizeof(int32));
                tempFile.read((char*)&roomEntry.girdH, sizeof(int32));

                // emptyCells 읽기
                int32 emptyCount = 0;
                tempFile.read((char*)&emptyCount, sizeof(int32));

                for (int32 i = 0; i < emptyCount; ++i)
                {
                    int32 ex, ey;
                    tempFile.read((char*)&ex, sizeof(int32));
                    tempFile.read((char*)&ey, sizeof(int32));
                    roomEntry.emptyCells.push_back({ ex, ey });
                }

                
            }

            _roomFiles.push_back(roomEntry);
        }
    }
}

void RoomManager::GenerateLayout(int32 roomCount)
{
    // 배치 형태 랜덤 생성
    _roomPos.clear();
    
    _startX = 0;
    _startY = 0;

    // 시작 방 배치
    FRoomInfo startPos;
    startPos.gridX = 0;
    startPos.gridY = 0;
    _roomPos[{0, 0}] = startPos;

    // 방향별 오프셋
    const int32 dx[] = { 0,0,-1,1 };
    const int32 dy[] = { -1,1,0,0 };

    // 랜덤 시드를 현재 시간으로 초기화
    srand(static_cast<unsigned>(time(nullptr)));

    // 원하는 개수가 될 떄까지 확장 반복
    while ((int32)_roomPos.size() < roomCount)
    {
        // 현재 존재하는 좌표를 벡터로 복사 
        std::vector<std::pair<int32, int32>> keys;
        for (auto& pair : _roomPos)
            keys.push_back(pair.first);

        // 기존 위치값 중 랜덤 하나를 선택 
        int32 randIdx = rand() % (int32)keys.size();
        int32 baseX = keys[randIdx].first;
        int32 baseY = keys[randIdx].second;

        // 랜덤 방향 선택
        int32 dir = rand() % 4;
        int32 newX = baseX + dx[dir];
        int32 newY = baseY + dy[dir];

        // 이미 방이 있는 위치면 스킵하고 다시 시도
        if (_roomPos.find({ newX, newY }) != _roomPos.end())
            continue;

        // 새 셀 추가
        FRoomInfo cell;
        cell.gridX = newX;
        cell.gridY = newY;
        _roomPos[{newX, newY}] = cell;
    }
    // 모든 셀의 인접관계 설정.
    ConnectNeighbors();

}
void RoomManager::ConnectNeighbors()
{
    // 인접 방 연결 각 셀에서 4방향을 확인하여 인접 셀이 존재하면 포인터 연결 
    const int32 dx[] = { 0, 0, -1, 1 };
    const int32 dy[] = { 1, -1, 0, 0 };

    for (auto& [key, cell] : _roomPos)
    {
        for (int32 d = 0; d < 4; ++d)
        {
            auto it = _roomPos.find({ key.first + dx[d], key.second + dy[d] });
            // 인접 셀이 있으면 포인터 연결, 없으면 nullptr (= 벽)
            cell.neighbors[d] = (it != _roomPos.end()) ? &(it->second) : nullptr;
        }
    }
}

bool RoomManager::TryPlaceRoom(FRoomInfo& cell)
{
    // 방이 차지할 모든 셀이 비어있는지 확인
    for (auto& offset : cell.occupiedCells)
    {
        int32 gx = cell.gridX + offset.first;
        int32 gy = cell.gridY + offset.second;
        // 겹침 확인.
        if (_occupiedGrid.find({ gx, gy }) != _occupiedGrid.end())
            return false;

    }
    // 전부 비어져있으면 공간 확보.
    for (auto& offset : cell.occupiedCells)
    {
        int32 gx = cell.gridX + offset.first;
        int32 gy = cell.gridY + offset.second;
        _occupiedGrid.insert({ gx, gy });
    }
    return true;
}

void RoomManager::AssignRooms()
{
    // .room 파일 랜덤 배정
    if (_roomFiles.empty()) 
        return;

    // 셔플용 복사본
    std::vector<FRoomFileEntry> shuffled = _roomFiles;
    
    //  랜덤셔플
    for (int32 i = (int32)shuffled.size() - 1; i > 0; --i)
    {
        int32 j = rand() % (i + 1);
        std::swap(shuffled[i], shuffled[j]);
    }

    int32 idx = 0;
    for (auto& [key, cell] : _roomPos)
    {
        if (idx >= (int32)shuffled.size())
            break;

        // 변경 — 빈칸 제외
        cell.occupiedCells.clear();
        for (int32 gy = 0; gy < shuffled[idx].girdH; ++gy)
        {
            for (int32 gx = 0; gx < shuffled[idx].gridW; ++gx)
            {
                // emptyCells에 포함되어 있으면 건너뜀
                bool skip = false;
                for (auto& e : shuffled[idx].emptyCells)
                {
                    if (e.first == gx && e.second == gy)
                    {
                        skip = true; break;
                    }
                }
                if (!skip)
                    cell.occupiedCells.push_back({ gx, gy });
            }
        }

        // 겹침 확인
        if (TryPlaceRoom(cell))
        {

            cell.roomFilePath = shuffled[idx].filePath;
            ++idx;
        }
        else
        {
            // 겹치면 이 셀은 건너뜀 (occupiedCells 비우기)
            cell.occupiedCells.clear();
        }
    }
}

void RoomManager::LoadAllRooms()
{
    // 1차: 로드만 해서 실제 크기 파악
    for (auto& [key, cell] : _roomPos)
    {
        if (!cell.roomFilePath.empty())
        {
            std::filesystem::path filePath(cell.roomFilePath);
            if (DirectoryManager::Instance().IsFile(filePath))
            {
                std::ifstream file(filePath, std::ios::binary);
                if (file.is_open())
                {
                    int32 roomCount = 0;
                    file.read((char*)&roomCount, sizeof(int32));

                    cell.tileMap = _level->SpawnActor<TileMap>(
                        "Room_" + std::to_string(cell.gridX) + "_" + std::to_string(cell.gridY),
                        FVector3D(0, 0, 0), FVector3D(1, 1, 1), FRotator(0, 0, 0));
                    cell.tileMap->SetEnable(false);  // ← 렌더링 방지
                    cell.tileMap->Load(file);
                    cell.isLoaded = true;

                }
            }
        }
    }

    // 2차: 실제 크기 기반으로 위치 재배치
    for (auto& [key, cell] : _roomPos)
    {
        if (cell.tileMap)
        {
            FVector3D worldPos;
            worldPos._x = cell.gridX * (_roomWorldWidth + _roomGap);
            worldPos._y = cell.gridY * (_roomWorldHeight + _roomGap);
            worldPos._z = 0.f;
            cell.tileMap->SetWorldPosition(worldPos);
            cell.tileMap->SetEnable(true);  // ← 활성화
        }
    }
}

void RoomManager::ActivateStartRoom()
{
    // 시작방 활성화
    auto it = _roomPos.find({ _startX,_startY });
    if (it != _roomPos.end())
    {
        _currentRoom = &(it->second);

        if (_currentRoom->tileMap && _level)
            _level->SetTileMap(_currentRoom->tileMap);

        ActivateRoom(_currentRoom);
    }

}
void RoomManager::ActivateRoom(FRoomInfo* cell)
{
    
    if (!cell || cell->isActivated)
        return;
    
    cell->isActivated = true;

    for (auto& monster : cell->monsters)
        monster->SetEnable(true);
    // 몬스터가 있으면 전투 시작
    if (!cell->monsters.empty())
        StartBattle(cell);


}

void RoomManager::DeactivateRoom(FRoomInfo* cell)
{
    if (!cell || !cell->isActivated) return;
    cell->isActivated = false;

    for (auto& monster : cell->monsters)
        monster->SetEnable(false);
}

void RoomManager::Tick(float deltaTime)
{
    if (!_currentRoom || !_level)
        return;

    // 전투 체크 (기존)
    if (_currentRoom->isBattleActive)
    {
        bool allDead = true;
        for (auto& monster : _currentRoom->monsters)
        {
            if (monster && monster->IsEnable())
            {
                allDead = false;
                break;
            }
        }
        if (allDead)
            EndBattle(_currentRoom);
        return;  // 전투 중에는 방 이동 불가
    }

    // 플레이어 위치로 방 전환 감지
    Ptr<Player> player = Cast<Actor, Player>(GameEngine::Instance().GetWorld()->GetPlayer());
    if (!player) return;

    FVector2D playerPos;
    playerPos._x = player->GetWorldPosition()._x;
    playerPos._y = player->GetWorldPosition()._y;

    FRoomInfo* nextRoom = FindCellAtWorldPos(playerPos);

    if (nextRoom && nextRoom != _currentRoom)
    {
        DeactivateRoom(_currentRoom);
        _currentRoom = nextRoom;
        ActivateRoom(_currentRoom);

        // 타일맵 전환
        if (_currentRoom->tileMap)
            _level->SetTileMap(_currentRoom->tileMap);
    }
}

FRoomInfo* RoomManager::FindCellAtWorldPos(const FVector2D& pos)
{
    int32 gx = (int32)floor(pos._x / (_roomWorldWidth + _roomGap));
    int32 gy = (int32)floor(pos._y / (_roomWorldHeight + _roomGap));

    auto it = _roomPos.find({ gx, gy });
    return (it != _roomPos.end()) ? &(it->second) : nullptr;
}

void RoomManager::Destroy()
{

}

void RoomManager::RegisterDoor(Ptr<Door> door)
{
    if (!_currentRoom || !door)
        return;
    _currentRoom->doors.push_back(door);
}

void RoomManager::RegisterMonster(Ptr<Monster> monster)
{
    if (!monster || !_currentRoom)
        return;
    _currentRoom->monsters.push_back(monster);
}

void RoomManager::MoveToRoom(eRoomDir dir, Ptr<class Player> player)
{
    if (!_currentRoom) return;

    FRoomInfo* nextRoom = _currentRoom->neighbors[(int)dir];
    if (!nextRoom || !nextRoom->tileMap) return;

    // 현재 방 비활성화
    DeactivateRoom(_currentRoom);
    _currentRoom = nextRoom;
    ActivateRoom(_currentRoom);

    if (_currentRoom->tileMap)
        _level->SetTileMap(_currentRoom->tileMap);

    // 플레이어를 반대편 입구로 텔레포트
    FVector3D newPos;
    newPos._x = nextRoom->gridX * (_roomWorldWidth + _roomGap);
    newPos._y = nextRoom->gridY * (_roomWorldHeight + _roomGap);
    newPos._z = player->GetWorldPosition()._z;

    // 방향에 따라 반대편 위치
    switch (dir)
    {
    case eRoomDir::UP:
        newPos._x += _roomWorldWidth * 0.5f;
        newPos._y += 40.f;  // 아래쪽 입구
        break;
    case eRoomDir::DOWN:
        newPos._x += _roomWorldWidth * 0.5f;
        newPos._y += _roomWorldHeight - 40.f;  // 위쪽 입구
        break;
    case eRoomDir::LEFT:
        newPos._x += _roomWorldWidth - 40.f;  // 오른쪽 입구
        newPos._y += _roomWorldHeight * 0.5f;
        break;
    case eRoomDir::RIGHT:
        newPos._x += 40.f;  // 왼쪽 입구
        newPos._y += _roomWorldHeight * 0.5f;
        break;
    }

    player->SetWorldPosition(newPos);
}

void RoomManager::StartBattle(FRoomInfo* room)
{
    if (!room || room->isBattleActive)
        return;
    room->isBattleActive = true;

    for (auto& door : room->doors)
    {
        if (door && door->IsBattleControl())
            door->SetOpen(false);
    }
}

void RoomManager::EndBattle(FRoomInfo* room)
{
    if (!room) return;
    room->isBattleActive = false;

    // 전투 종료 시 문 열기
    for (auto& door : room->doors)
    {
        if (door && door->IsBattleControl())
            door->SetOpen(true);
    }

    // 보스 방이었는지 확인
    bool wasBossRoom = false;
    for (auto& monster : room->monsters)
    {
        if (monster)
        {
            Ptr<Monster> mon = Cast<Actor, Monster>(monster);
            if (mon && mon->GetMonsterType() == eMonsterType::Monster_BOSS)
            {
                wasBossRoom = true;
                break;
            }
        }
    }

    // 보스 방이면 엔딩 문 스폰
    if (wasBossRoom && _level)
    {
        // 방 중앙에 엔딩 문 생성
        FVector3D roomCenter;
        roomCenter._x = room->gridX * (_roomWorldWidth + _roomGap) + _roomWorldWidth * 0.5f;
        roomCenter._y = room->gridY * (_roomWorldHeight + _roomGap) + _roomWorldHeight * 0.5f;
        roomCenter._z = 2.f;

        auto exitDoor = _level->SpawnActor<Door>("BossClearDoor", roomCenter,
            FVector3D(64.f, 48.f, 1.f), FRotator(0, 0, 0));

        if (exitDoor)
        {
            FDoorSpawnData exitData;
            exitData.doorType = eDoorType::BOSS_CLEAR;
            exitData.bOpen = true;
            exitData.bBattle = false;
            // 텍스처는 기존 문 것을 재사용하거나 별도 지정
            if (!room->doors.empty() && room->doors[0])
            {
                // 기존 문의 데이터에서 텍스처 복사
                auto& srcData = room->doors[0]->GetDoorData();
                exitData.textureName = srcData.textureName;
                exitData.texturePath = srcData.texturePath;
                exitData.frame = srcData.frame;
                exitData.openImage = srcData.openImage;
                exitData.left = srcData.left;
                exitData.right = srcData.right;
                exitData.renderSize = srcData.renderSize;
                exitData.collisionSize = srcData.collisionSize;
            }
            exitDoor->SetDoorData(exitData);
        }
    }
}