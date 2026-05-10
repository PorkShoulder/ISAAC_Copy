#pragma once
#include "../Core/Object.h"

#define DIRECTINPUT_VERSION 0x0800

#define DIK_MOUSELBUTTON 0xfc
#define DIK_MOUSERBUTTON 0xfd
#define DIK_MOUSEWHEEL   0xfe

#define KEY_PUSH 0x80

#include <dinput.h> // dx에서 제공하는 input

#pragma comment(lib, "dinput8.lib")

class Input : public Object
{
    friend class InputSystem;

public:
    Input() = default;
    virtual ~Input() = default;
    Input(const Input&) = delete;
    Input(Input&&) = delete;
    Input& operator= (const Input&) = delete;
    Input& operator= (Input&&) = delete;

private:
    HINSTANCE _hInst = 0;
    HWND      _hWnd = 0;
    ComPtr<IDirectInput8> _input = nullptr;

    //키보드와 연결되서 키보드 입력 상태를 가져오는 객체
    ComPtr<IDirectInputDevice8> _keyboard = nullptr;

    //마우스와 연결되서 마우스 입력 상태를 가져오는 객체
    ComPtr<IDirectInputDevice8> _mouse = nullptr;

    eInputSystemType _type = eInputSystemType::DINPUT;

    //키보드 키 입력 상태를 저장할 배열
    uint8 _keyState[256] = {};


    //LONG    lX;               : 마우스 X축 상대 이동량(이전 위치로부터 지금 X축으로 얼마나 이동했냐)
    //LONG    lY;               : 마우스 Y축 상대 이동량(이전 위치로부터 지금 Y축으로 얼마나 이동했냐)
    //LONG    lZ;               : 마우스 Z축 상대 이동량(마우스 휠 이동량)
    //BYTE    rgbButtons[4];
    //[0] : 보통 마우스 왼쪽버튼
    //[1] : 보통 마우스 오른쪽 버튼
    //[2] : 보통 마우스 휠
    DIMOUSESTATE _mouseState = {};

    //마우스 눌림(왼쪽, 오른쪽, 휠)
    bool _mouseDown[MOUSE_BUTTON_TYPE::End] = {};

    //마우스 누르고 있는중(왼쪽, 오른쪽, 휠)
    bool _mouseHold[MOUSE_BUTTON_TYPE::End] = {};

    //마우스 버튼 떼기(왼쪽, 오른쪽, 휠)
    bool _mouseUp[MOUSE_BUTTON_TYPE::End] = {};

    FVector2D _mousePos;
    FVector2D _mouseWorldPos;
    FVector2D _mouseMove;

    bool _mouseCompute = true;

public:
    bool GetMouseDown(MOUSE_BUTTON_TYPE::Type type)
    {
        return _mouseDown[type];
    }

    bool GetMouseHold(MOUSE_BUTTON_TYPE::Type type)
    {
        return _mouseHold[type];
    }

    bool GetMouseUp(MOUSE_BUTTON_TYPE::Type type)
    {
        return _mouseUp[type];
    }

    const FVector2D& GetMouseWorldPos() const
    {
        return _mouseWorldPos;
    }

    const FVector2D& GetMousePos() const
    {
        return _mousePos;
    }

    const FVector2D& GetMouseMove() const
    {
        return _mouseMove;
    }

public:
    bool Init();
    void Tick(float delatTime);
    bool GetKeyState(uint8 key);

    virtual void Destroy() override;

private:
    bool InitInput();
    bool UpdateKeyBoard();
    bool UpdateMouse();
    void UpdateInput(float deltaTime);
    void UpdateMousePos(float delatTime);
    uint8 ConvertKey(uint8 key);
};

