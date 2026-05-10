#include "pch.h"
#include "Input.h"
#include "Core/Device.h"
#include "Core/GameEngine.h"
#include "World/World.h"
#include "World/Level.h"

bool Input::Init()
{
	_hInst = GameEngine::Instance().GetHINSTANCE();
	_hWnd = GameEngine::Instance().GetHWND();

	if (FAILED(DirectInput8Create(_hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)_input.GetAddressOf(), nullptr)))
		_type = eInputSystemType::WINDOW;
	else
		_type = eInputSystemType::DINPUT;

	if (_type == eInputSystemType::DINPUT)
	{
		if (false == InitInput())
			return false;
	}

	return true;
}

void Input::Tick(float deltaTime)
{
	if (_type == eInputSystemType::DINPUT)
	{
		UpdateKeyBoard();
		UpdateMouse();
	}

	UpdateMousePos(deltaTime);
	UpdateInput(deltaTime);
}

bool Input::GetKeyState(uint8 key)
{
	return _keyState[key] & KEY_PUSH;
}

void Input::Destroy()
{
}

bool Input::InitInput()
{
	//키보드 장치 생성
	if (FAILED(_input->CreateDevice(GUID_SysKeyboard, _keyboard.GetAddressOf(), nullptr)))
		return false;

	//키보드 형식 지정
	if (FAILED(_keyboard->SetDataFormat(&c_dfDIKeyboard)))
		return false;

	if (Device::Instance().GetWindowMode())
	{
		if (FAILED(_keyboard->SetCooperativeLevel(_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
			return false;
	}
	else
	{
		if (FAILED(_keyboard->SetCooperativeLevel(_hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE)))
			return false;
	}

	//키보드 장치 획득
	if (FAILED(_keyboard->Acquire()))
		return false;

	//마우스 장치 생성
	if (FAILED(_input->CreateDevice(GUID_SysMouse, _mouse.GetAddressOf(), nullptr)))
		return false;

	//마우스 형식 지정
	if (FAILED(_mouse->SetDataFormat(&c_dfDIMouse)))
		return false;

	if (Device::Instance().GetWindowMode())
	{
		if (FAILED(_mouse->SetCooperativeLevel(_hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
			return false;
	}
	else
	{
		if (FAILED(_mouse->SetCooperativeLevel(_hWnd, DISCL_BACKGROUND | DISCL_EXCLUSIVE)))
			return false;
	}

	//마우스 장치 획득
	if (FAILED(_mouse->Acquire()))
		return false;

	return true;
}

bool Input::UpdateKeyBoard()
{
	//키보드 상태를 갱신
	if (!_keyboard)
		return false;

	HRESULT ret = _keyboard->GetDeviceState(256, (LPVOID)&_keyState);
	if (FAILED(ret))
	{
		if (ret == DIERR_INPUTLOST || ret == DIERR_NOTACQUIRED)
			_keyboard->Acquire();
	}

	return true;
}

bool Input::UpdateMouse()
{
	//마우스 상태를 갱신
	if (!_mouse)
		return false;

	HRESULT ret = _mouse->GetDeviceState(sizeof(_mouseState), (LPVOID)&_mouseState);
	if (FAILED(ret))
	{
		if (ret == DIERR_INPUTLOST || ret == DIERR_NOTACQUIRED)
			_mouse->Acquire();
	}

	return true;
}

void Input::UpdateInput(float deltaTime)
{
	if (_type == eInputSystemType::DINPUT)
	{
		for (int i = 0; i < MOUSE_BUTTON_TYPE::Type::End; ++i)
		{
			if (_mouseState.rgbButtons[i] & KEY_PUSH)
			{
				//현재 마우스 버튼을 눌렀거나, 눌려져 있다.

				//지금 막 마우스를 눌렀다.
				if (!_mouseDown[i] && !_mouseHold[i])
				{
					_mouseDown[i] = true;
					_mouseHold[i] = true;
				}
				else
				{
					_mouseDown[i] = false;
				}
			}
			else
			{
				//마우스를 누른 상태가 아님.

				//이전 프레임에 눌러져있는 상태
				if (_mouseHold[i])
				{
					_mouseDown[i] = false;
					_mouseHold[i] = false;
					_mouseUp[i] = true;
				}
				else
				{
					_mouseUp[i] = false;
				}
			}


		}
	}
}

void Input::UpdateMousePos(float delatTime)
{
	//마우스 좌표를 얻어온다.
	POINT mousePT;
	GetCursorPos(&mousePT);

	//스크린 좌표를 클라이언트 좌표로 넣어준다.
	ScreenToClient(_hWnd, &mousePT);

	//해상도와 윈도우 창 크기를 가지고 온다.
	FVector2D ratio = Device::Instance().GetRSRate();
	FResolution viewPortRS = Device::Instance().GetRS();

	FVector2D mousePos;

	//윈도우창에서의 마우스 위치를 해상도 비율을 곱해서 DirectX Viewport상의 위치를 구해준다.
	mousePos._x = mousePT.x * ratio._x;
	mousePos._y = mousePT.y * ratio._y;

	//윈도우는 y가 아래로 +
	//dx는 y가 위로 +
	//뷰포트 해상도를 이용해서 y좌표를 반전
	mousePos._y = viewPortRS._height - mousePos._y;


	if (_mouseCompute)
		_mouseMove = mousePos - _mousePos;
	else
		_mouseCompute = true;

	_mousePos = mousePos;

	Ptr<World> world = GameEngine::Instance().GetWorld();
	Ptr<Level> level = world->GetCurLevel();
	if (nullptr == level)
		return;

	FVector3D cameraPos = level->GetCameraWorldPos();

	//마우스 좌표를 화면 좌측 상단 기준으로 변환
	//마우스 좌푤흘 화면 좌측 상단 기준에서 중앙기준으로 변환
	_mouseWorldPos._x = cameraPos._x + _mousePos._x - viewPortRS._width * 0.5f;
	_mouseWorldPos._y = cameraPos._y + _mousePos._y - viewPortRS._height * 0.5f;
}

uint8 Input::ConvertKey(uint8 key)
{
	if (_type == eInputSystemType::DINPUT)
	{

		switch (key)
		{
		case VK_LBUTTON:
			return DIK_MOUSELBUTTON;
		case VK_RBUTTON:
			return DIK_MOUSERBUTTON;
		case VK_MBUTTON:
			return DIK_MOUSEWHEEL;
		case VK_BACK:
			return DIK_BACK;
		case VK_TAB:
			return DIK_TAB;
		case VK_RETURN:
			return DIK_RETURN;
		case VK_LCONTROL:
			return DIK_LCONTROL;
		case VK_RCONTROL:
			return DIK_RCONTROL;
		case VK_LMENU:
			return DIK_LALT;
		case VK_RMENU:
			return DIK_RALT;
		case VK_LSHIFT:
			return DIK_LSHIFT;
		case VK_RSHIFT:
			return DIK_RSHIFT;
		case VK_PAUSE:
			return DIK_PAUSE;
		case VK_CAPITAL:
			return DIK_CAPSLOCK;
		case VK_ESCAPE:
			return DIK_ESCAPE;
		case VK_SPACE:
			return DIK_SPACE;
		case VK_NEXT:
			return DIK_PGDN;
		case VK_PRIOR:
			return DIK_PGUP;
		case VK_END:
			return DIK_END;
		case VK_HOME:
			return DIK_HOME;
		case VK_LEFT:
			return DIK_LEFT;
		case VK_UP:
			return DIK_UP;
		case VK_RIGHT:
			return DIK_RIGHT;
		case VK_DOWN:
			return DIK_DOWN;
		case VK_PRINT:
			return DIK_SYSRQ;
		case VK_INSERT:
			return DIK_INSERT;
		case VK_DELETE:
			return DIK_DELETE;
		case VK_HELP:
			return 0;
		case '0':
			return DIK_0;
		case '1':
			return DIK_1;
		case '2':
			return DIK_2;
		case '3':
			return DIK_3;
		case '4':
			return DIK_4;
		case '5':
			return DIK_5;
		case '6':
			return DIK_6;
		case '7':
			return DIK_7;
		case '8':
			return DIK_8;
		case '9':
			return DIK_9;
		case 'A':
			return DIK_A;
		case 'B':
			return DIK_B;
		case 'C':
			return DIK_C;
		case 'D':
			return DIK_D;
		case 'E':
			return DIK_E;
		case 'F':
			return DIK_F;
		case 'G':
			return DIK_G;
		case 'H':
			return DIK_H;
		case 'I':
			return DIK_I;
		case 'J':
			return DIK_J;
		case 'K':
			return DIK_K;
		case 'L':
			return DIK_L;
		case 'M':
			return DIK_M;
		case 'N':
			return DIK_N;
		case 'O':
			return DIK_O;
		case 'P':
			return DIK_P;
		case 'Q':
			return DIK_Q;
		case 'R':
			return DIK_R;
		case 'S':
			return DIK_S;
		case 'T':
			return DIK_T;
		case 'U':
			return DIK_U;
		case 'V':
			return DIK_V;
		case 'W':
			return DIK_W;
		case 'X':
			return DIK_X;
		case 'Y':
			return DIK_Y;
		case 'Z':
			return DIK_Z;
		case VK_OEM_3:
			return DIK_GRAVE;
		case VK_OEM_MINUS:
			return DIK_MINUS;
		case VK_OEM_NEC_EQUAL:
			return DIK_EQUALS;
		case VK_OEM_4:
			return DIK_LBRACKET;
		case VK_OEM_6:
			return DIK_RBRACKET;
		case VK_OEM_5:
			return DIK_BACKSLASH;
		case VK_OEM_1:
			return DIK_SEMICOLON;
		case VK_OEM_7:
			return DIK_APOSTROPHE;
		case VK_OEM_COMMA:
			return DIK_COMMA;
		case VK_OEM_PERIOD:
			return DIK_PERIOD;
		case VK_OEM_2:
			return DIK_SLASH;
		case VK_NUMPAD0:
			return DIK_NUMPAD0;
		case VK_NUMPAD1:
			return DIK_NUMPAD1;
		case VK_NUMPAD2:
			return DIK_NUMPAD2;
		case VK_NUMPAD3:
			return DIK_NUMPAD3;
		case VK_NUMPAD4:
			return DIK_NUMPAD4;
		case VK_NUMPAD5:
			return DIK_NUMPAD5;
		case VK_NUMPAD6:
			return DIK_NUMPAD6;
		case VK_NUMPAD7:
			return DIK_NUMPAD7;
		case VK_NUMPAD8:
			return DIK_NUMPAD8;
		case VK_NUMPAD9:
			return DIK_NUMPAD9;
		case VK_MULTIPLY:
			return DIK_MULTIPLY;
		case VK_ADD:
			return DIK_ADD;
		case VK_SEPARATOR:
			return DIK_NUMPADCOMMA;
		case VK_SUBTRACT:
			return DIK_SUBTRACT;
		case VK_DECIMAL:
			return DIK_DECIMAL;
		case VK_DIVIDE:
			return DIK_DIVIDE;
			//case VK_RETURN:		
			//return DIK_NUMPADENTER;
		case VK_F1:
			return DIK_F1;
		case VK_F2:
			return DIK_F2;
		case VK_F3:
			return DIK_F3;
		case VK_F4:
			return DIK_F4;
		case VK_F5:
			return DIK_F5;
		case VK_F6:
			return DIK_F6;
		case VK_F7:
			return DIK_F7;
		case VK_F8:
			return DIK_F8;
		case VK_F9:
			return DIK_F9;
		case VK_F10:
			return DIK_F10;
		case VK_F11:
			return DIK_F11;
		case VK_F12:
			return DIK_F12;
		case VK_F13:
			return DIK_F13;
		case VK_F14:
			return DIK_F14;
		case VK_F15:
			return DIK_F15;
		case VK_F16:
		case VK_F17:
		case VK_F18:
		case VK_F19:
		case VK_F20:
		case VK_F21:
		case VK_F22:
		case VK_F23:
		case VK_F24:
			return 0;
		case VK_NUMLOCK:
			return DIK_NUMLOCK;
		case VK_SCROLL:
			return DIK_SCROLL;
		case VK_LWIN:
			return DIK_LWIN;
		case VK_RWIN:
			return DIK_RWIN;
		case VK_APPS:
			return DIK_APPS;
		case VK_OEM_102:
			return DIK_OEM_102;
		}

		return 0xff;

	}

	return key;
}
