#include "pch.h"
#include "World.h"
#include "Level.h"

#include "../Object/Actor.h"

World::World()
{
}

World::~World()
{
}

void World::Init(const std::string& name)
{
    //todo save & load
    _curLevel = CreateLevel<Level>(name);
}

void World::Tick(float deltaTime)
{
    if (_curLevel)
        _curLevel->Tick(deltaTime);
}

void World::Collision(float deltaTime)
{
    if (_curLevel)
        _curLevel->Collision(deltaTime);
}

void World::Render(float deltaTime)
{
    if (_curLevel)
        _curLevel->Render(deltaTime);
}

void World::RenderUI(float deltaTime)
{
    if (_curLevel)
        _curLevel->RenderUI(deltaTime);
}

void World::Destroy()
{
    DESTROY(_curLevel)
        DESTROY(_nextLevel)
}

Ptr<class Level> World::GetCurLevel() const
{
    return _curLevel;
}

Ptr<class Actor> World::GetPlayer() const
{
    return Lock<Actor>(_player);
}

void World::SetMainPlayer(Ptr<class Actor> player)
{
    _player = player;
}
