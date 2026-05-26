#include "pch.h"
#include "RoomManager.h"
#include "Level.h"
#include "../Object/TileMap.h"

RoomManager::RoomManager()
{
}

RoomManager::~RoomManager()
{
}

void RoomManager::Init(Ptr<class Level > level, float roomWorldW, float roomWorldH)
{
	// 초기화 레벨 참조 저장, 방 크기 설정.
	_level = level;
	_roomWorldWidth = roomWorldW;
	_roomWorldHeight = roomWorldH;
}

void RoomManager::CollectRoomFiles(const std::wstring& folderPath)
{
	//.room 파일 수집 
    _roomFiles.clear();

    WIN32_FIND_DATA findData;
    std::wstring searchPath = folderPath + L"\\*.room";

    HANDLE hFind = FindFirstFile(searchPath.c_str(), &findData);
    if (hFind == INVALID_HANDLE_VALUE)
        return;
    do
    {
        // 폴더는 건너뛰고 파일만 수집
        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            continue;
        std::wstring fullPath = folderPath + L"\\" + findData.cFileName;
        std::string path(fullPath.begin(), fullPath.end());
        _roomFiles.push_back(path);

    } while (FindNextFile(hFind, &findData));

    FindClose(hFind);
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

void RoomManager::AssignRooms()
{
    // .room 파일 랜덤 배정
    if (_roomFiles.empty()) 
        return;

    // 셔플용 복사본
    std::vector<std::string> shuffled = _roomFiles;
    
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
        cell.roomFilePath = shuffled[idx];
        ++idx;
    }
}

void RoomManager::LoadAllRooms()
{
    for (auto& [key, cell] : _roomPos)
    {
        // 그리드 좌표 × 방 크기 = 월드 좌표
        FVector3D worldPos;
        worldPos._x = cell.gridX * (_roomWorldWidth + _roomGap);
        worldPos._y = cell.gridY * (_roomWorldHeight + _roomGap);
        worldPos._z = 0.f;

        // 방 이름: "Room_X_Y" 형식
        std::string name = "Room_" + std::to_string(cell.gridX)
            + "_" + std::to_string(cell.gridY);

        // Level에 TileMap 액터 스폰
        cell.tileMap = _level->SpawnActor<TileMap>(name, worldPos, FVector3D(1, 1, 1), FRotator(0, 0, 0));

        // .room 파일에서 타일 데이터 로드
        if (!cell.roomFilePath.empty())
        {
            std::ifstream file(cell.roomFilePath, std::ios::binary);
            if (file.is_open())
            {
                int32 roomCount = 0;
                file.read((char*)&roomCount, sizeof(int32));
                
                // 첫 번째 방 데이터 로드
                cell.tileMap->Load(file);
                cell.tileMap->SetWorldPosition (worldPos);
                cell.isLoaded = true;
            }
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
    if (!_currentRoom)
        return;

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
