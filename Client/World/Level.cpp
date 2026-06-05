#include "pch.h"
#include "Level.h"
#include "TagManager.h"
#include "CameraManager.h"

#include "Collision/CollisionManager.h"


#include "Object/Player.h"
#include "Object/Monster.h"
#include "Object/TileMap.h"

#include "Core/Device.h"

#include "Component/CameraComponent.h"
#include "Component/CollisionComponent.h"

#include "Common/LogManager.h"

#include "UI/UIManager.h"
#include "UI/Image.h"
#include "UI/Button.h"
#include "UI/ProgressBar.h"
#include "UI/TextBlock.h"

#include "RoomManager.h"


Level::Level()
{
}

Level::~Level()
{
}

bool Level::Init(const std::string& path)
{
    _tagManager = New<TagManager>();
    _tagManager->Init();

    _cameraManager = New<CameraManager>();
    _cameraManager->Init(This<Level>());

    _collisionManager = New<CollisionManager>();
    _collisionManager->Init();

    _uiManager = New<UIManager>();
    _uiManager->Init(This<Level>());

    //if (_useRandomMap)
    //{
    //    _roomManager = New<RoomManager>();
    //    _roomManager->Init(This<Level>(), 960.f, 576.f);  // 15*64, 9*64
    //    _roomManager->CollectRoomFiles(L"../Resources/Room");  // .room 수집

    //    // 방호출 제한 
    //    int32 count = min(_roomManager->GetRoomFileCount(), _roomManager->GetMaxRoomCount());
    //    _roomManager->GenerateLayout(count);
    //    _roomManager->AssignRooms();                             // 각 셀에 .room 배정
    //    _roomManager->LoadAllRooms();                            // 전부 스폰 + 로드
    //    _roomManager->ActivateStartRoom();                       // 시작 방 활성화
    //}
    



    //todo : level save & load
    
    // 초기값 설정
    FVector3D scale = FVector3D(1.f, 1.f, 1.f);
    FVector3D pos = FVector3D(0.f, 0.f, 1.f);
    FRotator rot = FRotator(0.f, 0.f, 0.f);

    Ptr<Player> t1 = SpawnActor<Player>("ISAAC", pos, scale, rot); // 플레이어의 이름 설정
    if (t1) 
    {
        t1->AddTags("Player", "Human"); 
    }
    return true;
}

void Level::Tick(float deltaTime)
{
    if (_roomManager)
        _roomManager->Tick(deltaTime);

    //지울 액터들
    for (auto it : _removeActors)
    {
        Ptr<Actor> actor = FindActor<Actor>(it);
        if (nullptr == actor)
            continue;

        DeleteTag(actor);

        DESTROY(actor)
            //actor->Destroy();
            //Delete(actor);

            //현재 활성화 목록에서 지운다.
            _actors.erase(it);
    }

    _removeActors.clear();

    for (auto& it : _actors)
    {
        if (!it.second->IsActive())
        {
            RemoveActor(it.first);
            continue;
        }

        if (!it.second->IsEnable())
            continue;

        it.second->Tick(deltaTime);
    }

    _uiManager->Tick(deltaTime);
    

}

void Level::Collision(float deltaTime)
{
    _collisionManager->Collision(deltaTime);

    _uiManager->MouseEvent();

    for (auto& it : _actors)
    {
        if (!it.second->IsActive())
        {
            RemoveActor(it.first);
            continue;
        }

        if (!it.second->IsEnable())
            continue;

        //액터의 매개변수가 deltaTime인
        //collision 함수는 충돌 후처리 함수라고 볼 수 있다.
        it.second->Collision(deltaTime);
    }
}

void Level::Render(float deltaTime)
{
    for (auto& it : _actors)
    {
        if (!it.second->IsActive())
        {
            RemoveActor(it.first);
            continue;
        }

        if (!it.second->IsEnable())
            continue;

        it.second->Render(deltaTime);
    }
}

void Level::RenderUI(float deltaTime)
{
    //Device::Instance().GetTarget2D()->BeginDraw();

    _uiManager->Render();

    //Device::Instance().GetTarget2D()->EndDraw();
}

void Level::Destroy()
{
    if (_roomManager)
        DESTROY(_roomManager);

    for (auto& it : _actors)
        DESTROY(it.second)

        _actors.clear();

    DESTROY(_tagManager);
    DESTROY(_cameraManager);
    DESTROY(_collisionManager);
    DESTROY(_uiManager);
    
}

void Level::Save(std::ofstream& file)
{
    //todo : level save
    for (auto& it : _actors)
    {
        if (!it.second->IsActive())
            continue;

        //it.second->Save(file);
    }
}

void Level::Load(std::ifstream& file)
{
    //todo : level load
    for (auto& it : _actors)
    {
        if (!it.second->IsActive())
            continue;

        //it.second->Load(file);
    }
}

void Level::AddTag(const std::string& tag, int32 id)
{
    _tagManager->Add(tag, id);
}

void Level::DeleteTag(Ptr<class Actor> actor)
{
    if (actor->_tags.size() < 1)
        return;

    for (auto& it : actor->_tags)
        _tagManager->Erase(it, actor->_id);
}

const std::map<int32, Ptr<class Actor>>& Level::GetActors() const
{
    return _actors;
}

void Level::SetMainCamera(Ptr<class CameraComponent> camera)
{
    _cameraManager->SetMainCamera(camera);
}

Ptr<class CameraComponent> Level::GetMainCamera() const
{
    return _cameraManager->GetMainCamera();
}

const FMatrix& Level::GetViewMatrix() const
{
    return _cameraManager->GetViewMatrix();
}

const FMatrix& Level::GetProjMatrix() const
{
    return _cameraManager->GetProjMatrix();
}

const FVector3D& Level::GetCameraWorldPos() const
{
    return _cameraManager->GetCameraWorldPos();
}

const FMatrix& Level::GetUIProjMatrix() const
{
    return _cameraManager->GetUIProjMatrix();
}

void Level::AddCollision(std::pair<int32, int32>& colID, Ptr<CollisionComponent> comp)
{
    _collisionManager->Insert(colID, comp);
}

void Level::RemoveCollision(std::pair<int32, int32>& colID)
{
    _collisionManager->Remove(colID);
}

Ptr<class CollisionComponent> Level::FindCollider(std::pair<int32, int32>& colID)
{
    return _collisionManager->FindCollider(colID);
}

Ptr<Actor> Level::FindActor(int32 id)
{
    auto it = _actors.find(id);
    if (_actors.end() == it)
        return nullptr;

    return it->second;
}

void Level::FindActors(const std::string& tag, OUT std::vector<Ptr<class Actor>>& outArr)
{
    std::vector<int32> actorIDs;
    _tagManager->GetActorIDs(tag, actorIDs);

    if (actorIDs.size() < 1)
        return;

    for (auto i : actorIDs)
    {
        Ptr<Actor> actor = FindActor(i);
        if (nullptr == actor)
            continue;

        outArr.push_back(actor);
    }
}

void Level::RemoveActor(int32 id)
{
    _removeActors.push_back(id);
}

void Level::GenerateRandomMap()
{
    _roomManager = New<RoomManager>();
    _roomManager->Init(This<Level>());
    _roomManager->CollectRoomFiles();
    int32 count = min(_roomManager->GetRoomFileCount(), _roomManager->GetMaxRoomCount());
    _roomManager->GenerateLayout(count);
    _roomManager->AssignRooms();
    _roomManager->LoadAllRooms();
    _roomManager->ActivateStartRoom();
}
