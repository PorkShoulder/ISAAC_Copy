#pragma once
#include "Core/Object.h"
#include "Object/Actor.h"
//#include "UI/UIManager.h"

#include <map>
#include <vector>

class Level : public Object
{
public:
    Level();
    virtual ~Level();
    Level(const Level&) = delete;
    Level(Level&&) = delete;
    Level& operator=(const Level&) = delete;
    Level& operator=(Level&&) = delete;

private:
    int32 _actorID = 0;
    std::map<int32, Ptr<class Actor>> _actors;
    std::vector<int32> _removeActors;
    Ptr<class TagManager> _tagManager;
    Ptr<class CameraManager> _cameraManager;
    Ptr<class CollisionManager> _collisionManager;
    Ptr<class UIManager> _uiManager;

public:
    virtual bool Init(const std::string& path);
    virtual void Tick(float deltaTime);
    virtual void Collision(float deltaTime);
    virtual void Render(float deltaTime);
    virtual void RenderUI(float deltaTime);


    virtual void Destroy() override;

    void AddTag(const std::string& tag, int32 id);
    void DeleteTag(Ptr<class Actor> actor);

    const std::map<int32, Ptr<class Actor>>& GetActors() const;

    void SetMainCamera(Ptr<class CameraComponent> camera);
    Ptr<class CameraComponent> GetMainCamera() const;
    const FMatrix& GetViewMatrix() const;
    const FMatrix& GetProjMatrix() const;
    const FVector3D& GetCameraWorldPos() const;

    const FMatrix& GetUIProjMatrix() const;

    void AddCollision(std::pair<int32, int32>& colID, Ptr<class CollisionComponent> comp);

    void RemoveCollision(std::pair<int32, int32>& colID);

    Ptr<class CollisionComponent> FindCollider(std::pair<int32, int32>& colID);

    void FindActors(const std::string& tag, OUT std::vector<Ptr<class Actor>>& outArr);

    void RemoveActor(int32 id);

    //레벨에서 객체들을 Actor 클래스로 관리하고 있으므로
    //Actor 클래스 타입으로 가져온다.
    Ptr<Actor> FindActor(int32 id);

    //타입을 넣어주면 해당 타입이 Actor 클래스의 자식 클래스가 맞다면
    //자식 클래스타입으로 가져오는 함수
    template<typename T>
    Ptr<T> FindActor(int32 id)
    {
        auto it = _actors.find(id);
        if (_actors.end() == it)
            return nullptr;

        return Cast<Actor, T>(it->second);
    }

    template<typename T>
    Ptr<T> SpawnActor(const FVector3D& pos, const FVector3D& scale, const FRotator& rot)
    {
        Ptr<T> actor = New<T>();

        actor->SetLevel(This<Level>());

        int32 actorID = _actorID++;

        if (!actor->Init(actorID, pos, scale, rot))
        {
            DESTROY(actor)
                return nullptr;
        }

        _actors[actorID] = actor;

        return actor;
    }

    template<typename T>
    Ptr<T> CreateWidget(const std::string& name)
    {
        return _uiManager->CreateWidget<T>(name);
    }

};

