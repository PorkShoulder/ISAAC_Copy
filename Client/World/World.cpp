#include "pch.h"
#include "World.h"
#include "Level.h"
#include "EditorLevel.h"

#include "../Object/Actor.h"

World::World()
{
}

World::~World()
{
}

void World::Init(const std::string& name)
{
    _curLevel = CreateLevel<EditorLevel>(name);
    //_curLevel = CreateLevel<TitleLevel>(name);
}

void World::Tick(float deltaTime)
{
    //레벨 전환 구현 
    if (_nextLevel)
    {
        DESTROY(_curLevel);
        _curLevel = _nextLevel;
        _nextLevel = nullptr;
    }

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
