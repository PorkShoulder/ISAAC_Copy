#pragma once
#include"../Core/Object.h"

//==============================================
// 모든 컴포넌트의 공통 부모 클래스.
// 게임 오브젝트의 부품 기본 클래스.
//==============================================

class Component : public Object
{
	friend class Actor;
public:
	Component();
	virtual ~Component();

protected:
	Weak<class Level> _level;	// 이 컴포넌트가 속한 레벨 
	Weak<class Actor> _owner;	// 이 컴포넌트를 소유하고 있는 액터
	std::string _name;			// 컴포넌트 이름
	int32 _id = 1;				// 컴포넌트 아이디

public:
	const std::string& GetName() const { return _name; }
	void SetName(const std::string& name) { _name = name; }
	const int32 GetComponentID()const { return _id; }
	void SetComponentID(int32 id) { _id = id; }
	//
	const Ptr<class Actor> GetOwner() const;	// 컴포넌트 id 설정
	void SetOwner(Ptr<class Actor> owner);		// 소유 Actor 반환
	Ptr<class Level> GetLevel() const;			// 소유 Actor 설정
	//
	virtual bool Init(int32 id, const std::string& name, Ptr<class Actor> owner);
	virtual void Tick(float deltaTime);
	virtual void Collision(float deltaTiem);
	virtual void Render(float deltaTime);

	virtual void Destroy() override;
};

