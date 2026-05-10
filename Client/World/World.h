#pragma once
#include "../Core/Object.h"

//Level ---> 맵(unity에서 Scene)
class World : public Object
{
public:
    World();
    virtual ~World();
    World(const World&) = delete;
    World(World&&) = delete;
    World& operator=(const World&) = delete;
    World& operator=(World&&) = delete;

public:
    Ptr<class Level> _curLevel;
    Ptr<class Level> _nextLevel;


    Weak<class Actor> _player;

public:
    virtual void Init(const std::string& name);
    virtual void Tick(float deltaTime);
    virtual void Collision(float deltaTime);
    virtual void Render(float deltaTime);
    virtual void RenderUI(float deltaTime);

    virtual void Destroy() override;

    Ptr<class Level> GetCurLevel() const;

    Ptr<class Actor> GetPlayer() const;
    void SetMainPlayer(Ptr<class Actor> player);

public:
    template<typename T>
    Ptr<T> CreateLevel(const std::string& path)
    {
        Ptr<T> level = New<T>();
        if (false == level->Init(path))
        {
            DESTROY(level)
                return nullptr;
        }

        return level;
    }
};

