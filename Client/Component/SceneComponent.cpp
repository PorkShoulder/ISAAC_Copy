#include "pch.h"
#include "SceneComponent.h"
#include "Object/Actor.h"

#include "Shader/ShaderManager.h"
#include "Shader/TranformCBuffer.h"

#include "Render/RenderManager.h"

#include "Editor/EditorEngine.h"

SceneComponent::SceneComponent() : _renderLayerName("Default"), _isRender(false)
{
}

SceneComponent::~SceneComponent()
{
}

bool SceneComponent::Init(int32 id, const std::string& name, Ptr<class Actor> owner)
{
    Component::Init(id, name, owner);

    _transformCBuffer = FIND_CBUFFER("Transform", TranformCBuffer);

    //그려야 할 얘들만 랜더매니저에 넣을꺼에요.
    if (_isRender)
        RenderManager::Instance().AddRenderComponent(owner->GetActorID(), This<SceneComponent>());

    return true;
}

void SceneComponent::Tick(float deltaTime)
{
    Component::Tick(deltaTime);

    for (auto& it : _childs)
    {
        Ptr<SceneComponent> child = it.second;
        if (nullptr == child)
            continue;

        if (!child->IsActive() || !child->IsEnable())
            continue;

        child->Tick(deltaTime);
    }
}

void SceneComponent::Collision(float deltaTime)
{
    Component::Collision(deltaTime);
}

void SceneComponent::Render(float deltaTime)
{
    Component::Render(deltaTime);

    /*for (auto& it : _childs)
    {
        Ptr<SceneComponent> child = it.second;
        if (nullptr == child)
            continue;

        if (!child->IsActive() || !child->IsEnable())
            continue;

        child->Render(deltaTime);
    }*/
}

void SceneComponent::DrawInspector()
{
    Component::DrawInspector();

    ImGui::SeparatorText("SceneComponent");

    std::string label = std::format("World Transform##{0}", GetComponentID());
    ImGui::SeparatorText(label.c_str());

    FVector3D worldPos = GetWorldPosition();
    std::string posLabel = std::format("World Position##{0}", GetComponentID());
    if (ImGui::DragFloat3(posLabel.c_str(), &worldPos._x, 0.5f))
        SetWorldPosition(worldPos);

    FVector3D worldRot = GetWorldRotation();
    std::string rotLabel = std::format("World Rotation##{0}", GetComponentID());
    if (ImGui::DragFloat3(rotLabel.c_str(), &worldRot._x, 0.5f))
        SetWorldRotation(worldRot);

    FVector3D worldScale = GetWorldScale();
    std::string scaleLabel = std::format("World Scale##{0}", GetComponentID());
    if (ImGui::DragFloat3(scaleLabel.c_str(), &worldScale._x, 0.5f))
        SetWorldScale(worldScale);


    std::string rlabel = std::format("Relative Transform##{0}", GetComponentID());
    ImGui::SeparatorText(rlabel.c_str());

    FVector3D rPos = GetRelativePosition();
    std::string rPosLabel = std::format("Relative Position##{0}", GetComponentID());
    if (ImGui::DragFloat3(rPosLabel.c_str(), &rPos._x, 0.5f))
        SetRelativePosition(rPos);

    FVector3D rRot = GetRelativeRotation();
    std::string rrotLabel = std::format("Relative Rotation##{0}", GetComponentID());
    if (ImGui::DragFloat3(rrotLabel.c_str(), &rRot._x, 0.5f))
        SetRelativeRotation(rRot);

    FVector3D rScale = GetWorldScale();
    std::string rscaleLabel = std::format("Relative Scale##{0}", GetComponentID());
    if (ImGui::DragFloat3(rscaleLabel.c_str(), &rScale._x, 0.5f))
        SetRelativeScale(rScale);
}

void SceneComponent::Destroy()
{
    for (auto& it : _childs)
    {
        DESTROY(it.second)
            //it.second->Destroy();
            //Delete(it.second);
    }

    _childs.clear();

    if (_isRender)
    {
        Ptr<Actor> owner = Lock<Actor>(_owner);
        if (!owner)
            return;

        RenderManager::Instance().RemoveRenderComponent(_renderLayerName, owner->GetActorID(), GetComponentID());
    }

}

void SceneComponent::SetRenderLayer(const std::string& name)
{
    _renderLayerName = name;

    RenderManager::Instance().RefreshLayer();
}

const std::string& SceneComponent::GetRenderLayerName() const
{
    return _renderLayerName;
}

const std::map<int32, Ptr<SceneComponent>>& SceneComponent::GetChilds() const
{
    return _childs;
}

const Ptr<SceneComponent> SceneComponent::GetParent() const
{
    return Lock<SceneComponent>(_parent);
}

//자식컴포넌트가 누군가의 자식을 붙을 때
void SceneComponent::AttachToComponent(Ptr<SceneComponent> comp)
{
    comp->AddChild(This<SceneComponent>());
}

//부모가 컴포넌트를 추가할때
void SceneComponent::AddChild(Ptr<SceneComponent> comp)
{
    if (nullptr == comp)
        return;

    if (comp->GetComponentID() == GetComponentID())
        return;

    comp->_parent = This<SceneComponent>();
    _childs[comp->_id] = comp;
    UpdateTransform();
}

Ptr<SceneComponent> SceneComponent::FindComponent(int32 id) const
{
    auto it = _childs.find(id);
    if (_childs.end() == it)
    {
        for (auto& subIt : _childs)
        {
            Ptr<SceneComponent> comp = subIt.second->FindComponent(id);
            if (nullptr != comp)
                return comp;
        }

        return nullptr;
    }
    else
    {
        return it->second;
    }
}

const FTransform& SceneComponent::GetWorldTransform() const
{
    return _world;
}

void SceneComponent::SetWorldTransform(const FTransform& transform)
{
    SetWorldScale(transform._scale);
    SetWorldPosition(transform._position);
    SetWorldRotation(transform._rotation);
}

void SceneComponent::SetWorldTransform(const FVector3D& pos, const FVector3D& scale, const FRotator& rot)
{
    SetWorldScale(scale);
    SetWorldPosition(pos);
    SetWorldRotation(rot);
}

const FVector3D& SceneComponent::GetWorldScale() const
{
    return _world._scale;
}

void SceneComponent::SetWorldScale(const FVector3D& scale)
{
    SetWorldScale(scale._x, scale._y, scale._z);
}

void SceneComponent::SetWorldScale(float x, float y, float z)
{
    //나의 상대크기는 내 크기/부모의 크기
    if (Ptr<SceneComponent> parentComp = Lock<SceneComponent>(_parent))
    {
        _relative._scale._x = x / parentComp->_world._scale._x;
        _relative._scale._y = y / parentComp->_world._scale._y;
        _relative._scale._z = z / parentComp->_world._scale._z;
    }
    else
    {
        _relative._scale._x = x;
        _relative._scale._y = y;
        _relative._scale._z = z;
    }

    UpdateTransform();
}

void SceneComponent::SetWorldScale(const FVector2D& scale)
{
    SetWorldScale(scale._x, scale._y, _world._scale._z);
}

void SceneComponent::SetWorldScale(float x, float y)
{
    SetWorldScale(x, y, _world._scale._z);
}

const FVector3D& SceneComponent::AddWorldScale(const FVector3D& scale)
{
    SetWorldScale(scale._x + _world._scale._x, scale._y + _world._scale._y, scale._z + _world._scale._z);

    return _world._scale;
}

const FVector3D& SceneComponent::AddWorldScale(float x, float y, float z)
{
    SetWorldScale(x + _world._scale._x, y + _world._scale._y, z + _world._scale._z);

    return _world._scale;
}

const FVector3D& SceneComponent::AddWorldScale(const FVector2D& scale)
{
    SetWorldScale(scale._x + _world._scale._x, scale._y + _world._scale._y, _world._scale._z);

    return _world._scale;
}

const FVector3D& SceneComponent::AddWorldScale(float x, float y)
{
    SetWorldScale(x + _world._scale._x, y + _world._scale._y, _world._scale._z);

    return _world._scale;
}

const FVector3D& SceneComponent::AddWorldScale(float value)
{
    SetWorldScale(value + _world._scale._x, value + _world._scale._y, _world._scale._z);

    return _world._scale;
}

const FVector3D& SceneComponent::AddWorldScaleX(float x)
{
    SetWorldScale(x + _world._scale._x, _world._scale._y, _world._scale._z);

    return _world._scale;
}

const FVector3D& SceneComponent::AddWorldScaleY(float y)
{
    SetWorldScale(_world._scale._x, y + _world._scale._y, _world._scale._z);

    return _world._scale;
}

const FVector3D& SceneComponent::GetWorldPosition() const
{
    return _world._position;
}

void SceneComponent::SetWorldPosition(const FVector3D& pos)
{
    SetWorldPosition(pos._x, pos._y, pos._z);
}

void SceneComponent::SetWorldPosition(float x, float y, float z)
{
    if (Ptr<SceneComponent> parentComp = Lock<SceneComponent>(_parent))
    {
        FMatrix matParentInv = parentComp->_matrix._world;
        matParentInv.Inverse();
        _relative._position = FVector3D(x, y, z).TransformCoord(matParentInv);
    }
    else
    {
        _relative._position._x = x;
        _relative._position._y = y;
        _relative._position._z = z;
    }

    UpdateTransform();
}

void SceneComponent::SetWorldPosition(const FVector2D& pos)
{
    SetWorldPosition(pos._x, pos._y, _world._position._z);
}

void SceneComponent::SetWorldPosition(float x, float y)
{
    SetWorldPosition(x, y, _world._position._z);
}

const FVector3D& SceneComponent::AddWorldPosition(const FVector3D& pos)
{
    SetWorldPosition(pos._x + _world._position._x, pos._y + _world._position._y, pos._z + _world._position._z);

    return _world._position;
}

const FVector3D& SceneComponent::AddWorldPosition(float x, float y, float z)
{
    SetWorldPosition(x + _world._position._x, y + _world._position._y, z + _world._position._z);

    return _world._position;
}

const FVector3D& SceneComponent::AddWorldPosition(const FVector2D& pos)
{
    SetWorldPosition(pos._x + _world._position._x, pos._y + _world._position._y, _world._position._z);

    return _world._position;
}

const FVector3D& SceneComponent::AddWorldPosition(float x, float y)
{
    SetWorldPosition(x + _world._position._x, y + _world._position._y, _world._position._z);

    return _world._position;
}

const FVector3D& SceneComponent::AddWorldPosition(float value)
{
    SetWorldPosition(value + _world._position._x, value + _world._position._y, _world._position._z);

    return _world._position;
}

const FVector3D& SceneComponent::AddWorldPositionX(float x)
{
    SetWorldPosition(x + _world._position._x, _world._position._y, _world._position._z);

    return _world._position;
}

const FVector3D& SceneComponent::AddWorldPositionY(float y)
{
    SetWorldPosition(_world._position._x, y + _world._position._y, _world._position._z);

    return _world._position;
}

const FRotator& SceneComponent::GetWorldRotation() const
{
    return _world._rotation;
}

void SceneComponent::SetWorldRotation(const FRotator& rot)
{
    SetWorldRotation(rot._x, rot._y, rot._z);
}

void SceneComponent::SetWorldRotation(float x, float y)
{
    SetWorldRotation(x, y, _world._rotation._z);
}

const FTransform& SceneComponent::GetRelativeTransform() const
{
    return _relative;
}

void SceneComponent::SetRelativeTransform(const FTransform& transform)
{
    SetRelativePosition(transform._position);
    SetRelativeScale(transform._scale);
    SetRelativeRotation(transform._rotation);
}

void SceneComponent::SetRelativeTransform(const FVector3D& pos, const FVector3D& scale, const FRotator& rot)
{
    SetRelativePosition(pos);
    SetRelativeScale(scale);
    SetRelativeRotation(rot);
}

const FVector3D& SceneComponent::GetRelativeScale() const
{
    return _relative._scale;
}

void SceneComponent::SetRelativeScale(const FVector3D& scale)
{
    SetRelativeScale(scale._x, scale._y, scale._z);
}

void SceneComponent::SetRelativeScale(float x, float y, float z)
{
    _relative._scale._x = x;
    _relative._scale._y = y;
    _relative._scale._z = z;

    UpdateTransform();
}

void SceneComponent::SetRelativeScale(const FVector2D& scale)
{
    SetRelativeScale(scale._x, scale._y, _relative._scale._y);
}

void SceneComponent::SetRelativeScale(float x, float y)
{
    SetRelativeScale(x, y, _relative._scale._y);
}

const FVector3D& SceneComponent::AddRelativeScale(const FVector3D& scale)
{
    SetRelativeScale(
        scale._x + _relative._scale._x,
        scale._y + _relative._scale._y,
        scale._z + _relative._scale._z);

    return _relative._scale;
}

const FVector3D& SceneComponent::AddRelativeScale(float x, float y, float z)
{
    SetRelativeScale(
        x + _relative._scale._x,
        y + _relative._scale._y,
        z + _relative._scale._z);

    return _relative._scale;
}

const FVector3D& SceneComponent::AddRelativeScale(const FVector2D& scale)
{
    SetRelativeScale(
        scale._x + _relative._scale._x,
        scale._y + _relative._scale._y,
        _relative._scale._z);

    return _relative._scale;
}

const FVector3D& SceneComponent::AddRelativeScale(float x, float y)
{
    SetRelativeScale(
        x + _relative._scale._x,
        y + _relative._scale._y,
        _relative._scale._z);

    return _relative._scale;
}

const FVector3D& SceneComponent::AddRelativeScale(float value)
{
    SetRelativeScale(
        value + _relative._scale._x,
        value + _relative._scale._y,
        value + _relative._scale._z);

    return _relative._scale;
}

const FVector3D& SceneComponent::AddRelativeScaleX(float x)
{
    SetRelativeScale(
        x + _relative._scale._x,
        _relative._scale._y,
        _relative._scale._z);

    return _relative._scale;
}

const FVector3D& SceneComponent::AddRelativeScaleY(float y)
{
    SetRelativeScale(
        _relative._scale._x,
        y + _relative._scale._y,
        _relative._scale._z);

    return _relative._scale;
}

const FVector3D& SceneComponent::GetRelativePosition() const
{
    return _relative._position;
}

void SceneComponent::SetRelativePosition(const FVector3D& pos)
{
    SetRelativePosition(pos._x, pos._y, pos._z);
}

void SceneComponent::SetRelativePosition(float x, float y, float z)
{
    _relative._position._x = x;
    _relative._position._y = y;
    _relative._position._z = z;

    UpdateTransform();
}

void SceneComponent::SetRelativePosition(const FVector2D& pos)
{
    SetRelativePosition(pos._x, pos._y, _relative._position._z);
}

void SceneComponent::SetRelativePosition(float x, float y)
{
    SetRelativePosition(x, y, _relative._position._z);
}

const FVector3D& SceneComponent::AddRelativePosition(const FVector3D& scale)
{
    SetRelativePosition(
        scale._x + _relative._position._x,
        scale._y + _relative._position._y,
        scale._z + _relative._position._z);

    return _relative._position;
}

const FVector3D& SceneComponent::AddRelativePosition(float x, float y, float z)
{
    SetRelativePosition(
        x + _relative._position._x,
        y + _relative._position._y,
        z + _relative._position._z);

    return _relative._position;
}

const FVector3D& SceneComponent::AddRelativePosition(const FVector2D& scale)
{
    SetRelativePosition(
        scale._x + _relative._position._x,
        scale._y + _relative._position._y,
        _relative._position._z);

    return _relative._position;
}

const FVector3D& SceneComponent::AddRelativePosition(float x, float y)
{
    SetRelativePosition(
        x + _relative._position._x,
        y + _relative._position._y,
        _relative._position._z);

    return _relative._position;
}

const FVector3D& SceneComponent::AddRelativePosition(float value)
{
    SetRelativePosition(
        value + _relative._position._x,
        value + _relative._position._y,
        value + _relative._position._z);

    return _relative._position;
}

const FVector3D& SceneComponent::AddRelativePositionX(float x)
{
    SetRelativePosition(
        x + _relative._position._x,
        _relative._position._y,
        _relative._position._z);

    return _relative._position;
}

const FVector3D& SceneComponent::AddRelativePositionY(float y)
{
    SetRelativePosition(
        _relative._position._x,
        y + _relative._position._y,
        _relative._position._z);

    return _relative._position;
}

const FRotator& SceneComponent::GetRelativeRotation() const
{
    return _relative._rotation;
}

void SceneComponent::SetRelativeRotation(const FRotator& rot)
{
    SetRelativeRotation(rot._x, rot._y, rot._z);
}

void SceneComponent::SetRelativeRotation(float x, float y, float z)
{
    _relative._rotation._x = x;
    _relative._rotation._y = y;
    _relative._rotation._z = z;

    UpdateTransform();
}

void SceneComponent::SetRelativeRotation(const FVector2D& rot)
{
    SetRelativeRotation(rot._x, rot._y, _relative._rotation._z);
}

void SceneComponent::SetRelativeRotation(float x, float y)
{
    SetRelativeRotation(x, y, _relative._rotation._z);
}

const FRotator& SceneComponent::AddRelativeRotation(const FVector3D& rot)
{
    SetRelativeRotation(
        rot._x + _relative._rotation._x,
        rot._y + _relative._rotation._y,
        rot._z + _relative._rotation._z);

    return _relative._rotation;
}

const FRotator& SceneComponent::AddRelativeRotation(float x, float y, float z)
{
    SetRelativeRotation(
        x + _relative._rotation._x,
        y + _relative._rotation._y,
        z + _relative._rotation._z);

    return _relative._rotation;
}

const FRotator& SceneComponent::AddRelativeRotation(const FVector2D& scale)
{
    SetRelativeRotation(
        scale._x + _relative._rotation._x,
        scale._y + _relative._rotation._y,
        _relative._rotation._z);

    return _relative._rotation;
}

const FRotator& SceneComponent::AddRelativeRotation(float x, float y)
{
    SetRelativeRotation(
        x + _relative._rotation._x,
        y + _relative._rotation._y,
        _relative._rotation._z);

    return _relative._rotation;
}

const FRotator& SceneComponent::AddRelativeRotation(float value)
{
    SetRelativeRotation(
        value + _relative._rotation._x,
        value + _relative._rotation._y,
        value + _relative._rotation._z);

    return _relative._rotation;
}

const FRotator& SceneComponent::AddRelativeRotationX(float x)
{
    SetRelativeRotation(
        x + _relative._rotation._x,
        _relative._rotation._y,
        _relative._rotation._z);

    return _relative._rotation;
}

const FRotator& SceneComponent::AddRelativeRotationY(float y)
{
    SetRelativeRotation(
        _relative._rotation._x,
        y + _relative._rotation._y,
        _relative._rotation._z);

    return _relative._rotation;
}

void SceneComponent::UpdateTransform()
{
    _matrix._scale.Scaling(_relative._scale);
    _matrix._rotation.Rotation(_relative._rotation);
    _matrix._tranlate.Translation(_relative._position);

    //크기 * 회전 * 이동
    FMatrix matLocal = _matrix._scale * _matrix._rotation * _matrix._tranlate;

    if (Ptr<SceneComponent> parentComp = Lock<SceneComponent>(_parent))
    {
        _matrix._world = matLocal * parentComp->_matrix._world;
    }
    else
    {
        _matrix._world = matLocal;
    }

    _matrix._world.ExtractPosition(_world._position);
    _matrix._world.ExtractEuler(_world._rotation);
    _matrix._world.ExtractScale(_world._scale);

    for (auto& it : _childs)
    {
        if (it.second)
            it.second->UpdateTransform();
    }

    //회전된 축을 구하기
    FVector3D axis[AXIS_TYPE::END] =
    {
        FVector3D(FVector3D::Axis_X),
        FVector3D(FVector3D::Axis_Y),
        FVector3D(FVector3D::Axis_Z),
    };

    _axis[AXIS_TYPE::X] = axis[AXIS_TYPE::X].TransformNormal(_matrix._rotation);
    _axis[AXIS_TYPE::Y] = axis[AXIS_TYPE::Y].TransformNormal(_matrix._rotation);
    _axis[AXIS_TYPE::Z] = axis[AXIS_TYPE::Z].TransformNormal(_matrix._rotation);
}

//부모 : x 10, y 10, Z 0
//자식 : x 15, y 10, z 0
//------ x 5,    0   z 0

void SceneComponent::SetWorldRotation(const FVector2D& rot)
{
    SetWorldRotation(rot._x, rot._y, _world._rotation._z);
}

void SceneComponent::SetWorldRotation(float x, float y, float z)
{
    if (Ptr<SceneComponent> parentComp = Lock<SceneComponent>(_parent))
    {
        _relative._rotation._x = x - parentComp->_world._rotation._x;
        _relative._rotation._y = y - parentComp->_world._rotation._y;
        _relative._rotation._z = z - parentComp->_world._rotation._z;
    }
    else
    {
        _relative._rotation._x = x;
        _relative._rotation._y = y;
        _relative._rotation._z = z;
    }

    UpdateTransform();
}

//