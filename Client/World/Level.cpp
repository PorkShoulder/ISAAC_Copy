#include "pch.h"
#include "Level.h"

bool Level::Init(const std::string& path)
{
    return false;
}

void Level::Tick(float deltaTime)
{
}

void Level::Collision(float deltaTime)
{
}

void Level::Render(float deltaTime)
{
}

void Level::RenderUI(float deltaTime)
{
}

void Level::Destroy()
{
}

void Level::AddTag(const std::string& tag, int32 id)
{
}

void Level::DeleteTag(Ptr<class Actor> actor)
{
}

const std::map<int32, Ptr<class Actor>>& Level::GetActors() const
{
    // TODO: 여기에 return 문을 삽입합니다.
}

void Level::SetMainCamera(Ptr<class CameraComponent> camera)
{
}

Ptr<class CameraComponent> Level::GetMainCamera() const
{
    return Ptr<class CameraComponent>();
}

const FMatrix& Level::GetViewMatrix() const
{
    // TODO: 여기에 return 문을 삽입합니다.
}

const FMatrix& Level::GetProjMatrix() const
{
    // TODO: 여기에 return 문을 삽입합니다.
}

const FVector3D& Level::GetCameraWorldPos() const
{
    // TODO: 여기에 return 문을 삽입합니다.
}

const FMatrix& Level::GetUIProjMatrix() const
{
    // TODO: 여기에 return 문을 삽입합니다.
}

void Level::AddCollision(std::pair<int32, int32>& colID, Ptr<class CollisionComponent> comp)
{
}

void Level::RemoveCollision(std::pair<int32, int32>& colID)
{
}

Ptr<class CollisionComponent> Level::FindCollider(std::pair<int32, int32>& colID)
{
    return Ptr<class CollisionComponent>();
}

void Level::FindActors(const std::string& tag, OUT std::vector<Ptr<class Actor>>& outArr)
{
}

void Level::RemoveActor(int32 id)
{
}

Ptr<Actor> Level::FindActor(int32 id)
{
    return Ptr<Actor>();
}
