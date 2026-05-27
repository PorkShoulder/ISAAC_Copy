#pragma once
#include "../Core/Object.h"
#include <functional>
#include <set>

// 방 이동 방향 (인접 방 연결에 사용)
enum class eRoomDir 
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	COUTN,
	END
};

struct FRoomInfo
{
	// 그리드 좌표
	int32 gridX = 1;						
	int32 gridY = 1;
	// .Room파일 경로
	std::string roomFilePath;
	//실제 스폰된 타일맵
	Ptr<class TileMap> tileMap;
	// 이 망에 속한 몬스터 (미구현)
	std::vector<Ptr<class Actor>> monsters;
	// 현재 활성화 여부
	bool isActivated = false;
	// .room파일 로드 완료 여부 -> 중복방지
	bool isLoaded = false;
	// 인접 방 포인터 
	FRoomInfo* neighbors[4] = {}; 
	// ㄱ,ㄴ형태 사용중 공간 확인.
	std::vector<std::pair<int32, int32>> occupiedCells;
};

struct FRoomFileEntry
{
	std::string filePath;
	int32 gridW = 1;
	int32 girdH = 1;
	std::vector<std::pair<int32, int32>> emptyCells;
};


class RoomManager : public Object
{
public:
	RoomManager();
	virtual ~RoomManager();
private:
	// .room파일 경로
	std::vector<std::pair<int32, int32>> _roomGridSize;  
	std::vector<FRoomFileEntry> _roomFiles; // 추가
	// 생성된 방 배치맵 
	std::map<std::pair<int32, int32>, FRoomInfo> _roomPos;
	// 시작 방의 그리드 좌표
	int32 _startX = 0;
	int32 _startY = 0;
	//플레이어 현재 위치한 방
	FRoomInfo* _currentRoom = nullptr;

	//방하나의 월드 크기 타일(수) x타일(크기) 계산
	float _roomWorldWidth = 0.f;
	float _roomWorldHeight = 0.f;
	// 방 호출 제한
	int32 _maxRoomCount = 6; 
	//방 사이 간격
	float _roomGap = 0.f;
	// 방을 스폰할 레벨 참조
	Ptr<class Level> _level;
	
	// 사용중인 공간
	std::set<std::pair<int32, int32>> _occupiedGrid;

public:
	int32 GetRoomFileCount() const { return (int32)_roomFiles.size(); }
public:
	// 초기화 -> 레벨참조와 방 월드 크기 설정
	void Init(Ptr<class Level> level);
	// .room 확장자 파일 전부 읽음
	void CollectRoomFiles();
	// 랜덤 배치 형태 생성 
	void GenerateLayout(int32 roomCount);
	// 각 셀에 수집된 .room 파일중 하나를 핸덤 배정
	void AssignRooms();
	// 모든 셀의 TileMap을 스폰하고 .room파일로드
	void LoadAllRooms();
	// 시작 방만 활성화 (나머지는 비활성화)
	void ActivateStartRoom();
	
	
	int32 GetMaxRoomCount()const { return _maxRoomCount; }
	void SetMaxRoomCount(int32 count) { _maxRoomCount = count; }

	// 매 프레임 호출 — 플레이어 위치로 방 전환 감지
	void Tick(float deltaTime);
	virtual void Destroy() override;

private:
	// 방의 몬스터 활성화 (SetEnable(true))
	void ActivateRoom(FRoomInfo* cell);
	// 방의 몬스터 비활성화 (SetEnable(false))
	void DeactivateRoom(FRoomInfo* cell);
	// 월드 좌표 → 그리드 좌표 변환 후 해당 셀 반환
	FRoomInfo* FindCellAtWorldPos(const FVector2D& pos);
	// 모든 셀의 상하좌우 인접 방 포인터 연결
	void ConnectNeighbors();
	// 사용중 확인.
	bool TryPlaceRoom(FRoomInfo& cell);

};

