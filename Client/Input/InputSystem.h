#pragma once
#include "../Common/Singleton.h"
#include "../Common/Info.h"
#include "../Core/Defines.h"


#include <unordered_map>

class InputSystem : public Singleton<InputSystem>
{
    SINGLETON(InputSystem)

private:
    //현재 활성화 된 입력컨텐스트
    std::unordered_map<std::string, Ptr<class InputContext>> _activeContexts;

    //모든 입력컨텍스트
    std::unordered_map<std::string, Ptr<class InputContext>> _allContexts;

    //모든 입력액션
    std::unordered_map<std::string, Ptr<class InputAction>> _allActions;

    std::map<std::pair<int32, int32>, Weak<class InputComponent>> _inputComponents;

    Ptr<class Input> _input;

public:
    bool Init(Ptr<class Input> input);
    void Tick(float deltaTime);

public:
    void AddActiveInputContext(Ptr<class InputContext> context);
    void RemoveActiveContext(const std::string& name);
    Ptr<class InputContext> FindOrAddInputContext(const std::string& name);
    Ptr<class InputAction> FindOrAddInputAction(const std::string& name);
    bool IsActiveContext(const std::string& name);

    uint8 ConvertKey(uint8 key);

    void RegisterInputComponent(int32 actorID, Ptr<class InputComponent> inputComp);
    void RemoveInputComponent(int32 actorID, int32 componentID);
    void RefreshInputComponent(const std::string& contextName, const struct FInputMapping& mapping);

    const FVector2D& GetMouseWorldPos() const;
    const FVector2D& GetMousePos() const;
    const FVector2D& GetMouseMove() const;

    bool GetMouseDown(MOUSE_BUTTON_TYPE::Type type);
    bool GetMouseHold(MOUSE_BUTTON_TYPE::Type type);
    bool GetMouseUp(MOUSE_BUTTON_TYPE::Type type);

    virtual void Destroy() override;
};

