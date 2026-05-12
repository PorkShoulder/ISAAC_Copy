#pragma once
#include "Core/Object.h"
#include "Core/Texture.h"

#include <vector>

struct FUIBrush
{
    Ptr<Texture> _texture;
    FVector4D _tint = FVector4D(1.f, 1.f, 1.f, 1.f);
    std::vector<FAnimationFrame> _frames;
    bool _animEnable = false;
    int32 _frame = 0;
    float _time = 0.f;
    float _frameTime = 0.f;
    float _playTime = 0.f;
    float _playRate = 1.f;
};

class Widget : public Object
{
public:
    Widget();
    virtual ~Widget();

protected:
    int32 _widgetID = -1;
    std::string _name;

    Weak<class Level> _level;
    Weak<class Actor> _ownerActor; //UI가 액터에 붙을수도 있으니
    
    //UI도 부모-자식간의 계층구조를 가질 수 있어서
    //부모위젯
    Weak<Widget> _parent;
    
    //자식 위젯
    std::vector<Ptr<Widget>> _children;
    
    //자식 위젯 중 지워질 위젯
    std::vector<int32> _removeWidgets;

    //UI용 쉐이더
    Ptr<class Shader> _shader;

    //UI용 메쉬
    Ptr<class Mesh> _mesh;

    //UI용 트랜스폼 상수 버퍼
    Ptr<class TranformCBuffer> _transformCBuffer;

    //UI용 상수버퍼
    Ptr<class UICBuffer> _uiCBuffer;

    //위치
    FVector2D _pos;

    //그려질 위치
    FVector2D _renderPos;
    
    //크기
    FVector2D _size;

    //UI가 여러개 있을 때 순서
    int16 _order = -1;

    //정렬을 해야되는지 여부
    bool _sorted = false;

public:
    int32 GetWidgetID() const { return _widgetID; }
    void SetOwnerActor(Weak<class Actor> owner);
    Ptr<class Actor> GetOwnerActor() const;

    void SetParent(Ptr<Widget> parent);
    Ptr<Widget> GetParent() const;

    const FVector2D& GetPos() const;
    const FVector2D& GetRenderPos() const;
    const FVector2D& GetSize() const;
    int16 GetOrder() const;
    void SetRenderPos(const FVector2D& renderPos);

    virtual void SetPos(const FVector2D& pos);
    virtual void SetSize(const FVector2D& size);

    void RemoveChild(int32 childID);
    void AddChild(Ptr<Widget> widget);

    void SetSorted(bool sort);

public:
    virtual bool Init(int32 id, const std::string& name, Weak<class Level> level);
    virtual void Tick(float deltaTime);
    virtual void Render();
    virtual void Render(const FVector3D& pos); //나중에 액터에 붙어야 할때

    //UI <--> 마우스 충돌
    virtual Ptr<Widget> MouseHit(const FVector2D& pos);
    
    //UI가 마우스랑 상호작용 가능한지 여부
    virtual bool IsInteractable();

    //UI 위로 마우스가 올라왔을때
    virtual void OnHovered();

    //올라왔던 마우스가 UI 영역을 벗어났을 때
    virtual void OnUnHovered();

    //마우스가 UI를 클릭할 때
    virtual void OnClick();

    //마우스가 UI를 누르고 있을 때
    virtual void OnPressed();


    virtual void Destroy() override;

public:
    //마우스가 UI 영역 안에 있는지
    bool IsInSide(const FVector2D& pos);
};

