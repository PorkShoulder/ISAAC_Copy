#pragma once
#include "Component.h"

#include <map>


//====================================================
// 트랜스폼(위치,크기,회전)을 가진 컴포넌트의 부모클래스
//====================================================

class SceneComponent : public Component
{
	friend class Actor;
public:
	SceneComponent();
	virtual ~SceneComponent();

protected:
	Ptr<class TranformCBuffer> _transformCBuffer;	// 렌더링될때 자기 위치,크기,회전행렬 정보
	Weak<SceneComponent> _parent;					// 부모 컴포넌트
	std::map<int32, Ptr<SceneComponent>> _childs;	// 자식 컴포넌트 목록

	std::string _renderLayerName; //렌더링 순서 그룹,

    // 실제 위치 크기 회전 저장
	FTransform _world;			// 월드상의 나의 위치, 크기, 회전 (월드 위치)
	FTransform _relative;		// 부모로부터 나의 위치, 크기, 회전 (상대 위치)
	FTransformMatrix _matrix;	// 행렬 정보 실제로GPU에 보내는 정보
	FVector3D _axis[AXIS_TYPE::END] = // 기본 축 방향 3개를 배열로 저장해둠.
	{
		FVector3D(FVector3D::Axis_X),
		FVector3D(FVector3D::Axis_Y),
		FVector3D(FVector3D::Axis_Z),
	};
	//그려야 할 씬컴포넌트들을 구별하기 위함.
	bool _isRender = false;

public:
	virtual bool Init(int32 id, const std::string& name, //
		Ptr<class Actor> owner) override;
	virtual void Tick(float deltaTime) override;
	virtual void Collision(float deltaTime) override;
	virtual void Render(float deltaTime) override;

	virtual void Destroy() override;

	void SetRenderLayer(const std::string& name);
	const std::string& GetRenderLayerName() const;
	const std::map<int32, Ptr<SceneComponent>>& GetChilds() const;
public:
	const Ptr<SceneComponent> GetParent() const;
	void AttachToComponent(Ptr<SceneComponent> comp);
	void AddChild(Ptr<SceneComponent> comp);
	Ptr<SceneComponent> FindComponent(int32 id) const;

public:
    //===================
    //= World Transform =
    //===================
    const FTransform& GetWorldTransform() const;
    void SetWorldTransform(const FTransform& transform);
    void SetWorldTransform(const FVector3D& pos, const FVector3D& scale, const FRotator& rot);

    //===============
    //= World Scale =
    //===============
    const FVector3D& GetWorldScale() const;
    void SetWorldScale(const FVector3D& scale);
    void SetWorldScale(float x, float y, float z);
    void SetWorldScale(const FVector2D& scale);
    void SetWorldScale(float x, float y);
    const FVector3D& AddWorldScale(const FVector3D& scale);
    const FVector3D& AddWorldScale(float x, float y, float z);
    const FVector3D& AddWorldScale(const FVector2D& scale);
    const FVector3D& AddWorldScale(float x, float y);
    const FVector3D& AddWorldScale(float value);
    const FVector3D& AddWorldScaleX(float x);
    const FVector3D& AddWorldScaleY(float y);


    //==================
    //= World Position =
    //==================
    const FVector3D& GetWorldPosition() const;
    void SetWorldPosition(const FVector3D& pos);
    void SetWorldPosition(float x, float y, float z);
    void SetWorldPosition(const FVector2D& pos);
    void SetWorldPosition(float x, float y);
    const FVector3D& AddWorldPosition(const FVector3D& pos);
    const FVector3D& AddWorldPosition(float x, float y, float z);
    const FVector3D& AddWorldPosition(const FVector2D& pos);
    const FVector3D& AddWorldPosition(float x, float y);
    const FVector3D& AddWorldPosition(float value);
    const FVector3D& AddWorldPositionX(float x);
    const FVector3D& AddWorldPositionY(float y);


    //==================
    //= World Rotation =
    //==================
    const FRotator& GetWorldRotation() const;
    void SetWorldRotation(const FRotator& rot);
    void SetWorldRotation(float x, float y, float z);
    void SetWorldRotation(const FVector2D& rot);
    void SetWorldRotation(float x, float y);
    //WorldRotation의 경우 ExtractEuler 함수 반환 과정에서
    //부동 소수점 오차가 누적되어 값이 오차가 심해서 생략

   //======================
   //= Relative Transform =
   //======================
    const FTransform& GetRelativeTransform() const;
    void SetRelativeTransform(const FTransform& transform);
    void SetRelativeTransform(const FVector3D& pos, const FVector3D& scale, const FRotator& rot);

    //==================
    //= Relative Scale =
    //==================
    const FVector3D& GetRelativeScale() const;
    void SetRelativeScale(const FVector3D& scale);
    void SetRelativeScale(float x, float y, float z);
    void SetRelativeScale(const FVector2D& scale);
    void SetRelativeScale(float x, float y);
    const FVector3D& AddRelativeScale(const FVector3D& scale);
    const FVector3D& AddRelativeScale(float x, float y, float z);
    const FVector3D& AddRelativeScale(const FVector2D& scale);
    const FVector3D& AddRelativeScale(float x, float y);
    const FVector3D& AddRelativeScale(float value);
    const FVector3D& AddRelativeScaleX(float x);
    const FVector3D& AddRelativeScaleY(float y);

    //=====================
    //= Relative Position =
    //=====================
    const FVector3D& GetRelativePosition() const;
    void SetRelativePosition(const FVector3D& pos);
    void SetRelativePosition(float x, float y, float z);
    void SetRelativePosition(const FVector2D& pos);
    void SetRelativePosition(float x, float y);
    const FVector3D& AddRelativePosition(const FVector3D& scale);
    const FVector3D& AddRelativePosition(float x, float y, float z);
    const FVector3D& AddRelativePosition(const FVector2D& scale);
    const FVector3D& AddRelativePosition(float x, float y);
    const FVector3D& AddRelativePosition(float value);
    const FVector3D& AddRelativePositionX(float x);
    const FVector3D& AddRelativePositionY(float y);


    //=====================
    //= Relative Rotation =
    //=====================
    const FRotator& GetRelativeRotation() const;
    void SetRelativeRotation(const FRotator& rot);
    void SetRelativeRotation(float x, float y, float z);
    void SetRelativeRotation(const FVector2D& rot);
    void SetRelativeRotation(float x, float y);
    const FRotator& AddRelativeRotation(const FVector3D& scale);
    const FRotator& AddRelativeRotation(float x, float y, float z);
    const FRotator& AddRelativeRotation(const FVector2D& scale);
    const FRotator& AddRelativeRotation(float x, float y);
    const FRotator& AddRelativeRotation(float value);
    const FRotator& AddRelativeRotationX(float x);
    const FRotator& AddRelativeRotationY(float y);

private:
    //나의 월드위치, 월드크기, 월드 회전 
    //이건 전부 다 부모로부터 영향을 받아서 결정이 된다.
    void UpdateTransform();


};

