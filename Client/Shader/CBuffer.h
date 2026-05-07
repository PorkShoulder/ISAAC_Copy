#pragma once
#include "../Core/Object.h"

//==========================================================
// C++에서 만든 행렬,색상,머터리얼 같은 데이터를 GPU 상수 버퍼에
// 복사해서, HLSL의 cbuffer register(b#)가 읽게 연결하는 클래스
//==========================================================

class CBuffer : public Object
{
public:
	CBuffer();
	~CBuffer();

protected:
	ComPtr<ID3D11Buffer> _buffer = nullptr;		// gpu에서 만든 상수버퍼 객체. 상수 데이터 전용
	int _size = 0;								// 버퍼 바이트의 크기 
	int _register = 0;							// 슬롯번호 -> Share.fx에서 register()의 번호 -> hlsl과 일치해야 읽기가능
	int _type = 0;								// 어떤 쉐이ㅓ 단계에서 쓸지 정하는 값

public:
	bool Create(int size, int regi, int type);	// gpu에 상수 버퍼를 생성하는 함수.
	void SetData(void* data);					// cpu에서 준비한 데이터를 gpu버퍼에 복사하는 함수. void*인 이유->

	virtual void Update() = 0;
	virtual void Destroy() override;
};

