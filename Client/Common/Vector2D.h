#pragma once
#include <math.h>


struct FVector2D
{
	

	float _x = 0.f;
	float _y = 0.f;

	FVector2D();								// 아무 값도 안주고 객체를 만들때 호출
	FVector2D(float x, float y);				// 좌표값 2개를 받아서 객체를 만들 때 호출
	FVector2D(const FVector2D& other);			// 복사 생성자 같은 타입 객체를 복사해서 새 객체를 만들 때 호출
	FVector2D(FVector2D&& other) noexcept;		// 이동 생성자 복사하지 않고 가져와서 새객체를 만들떄 호출 임시객체나 더이상 쓰지 않을 객체 대상


	//operator =
	const FVector2D& operator=(const FVector2D& other);
	const FVector2D& operator=(float value);
	const FVector2D& operator=(double value);
	const FVector2D& operator=(int32 value);

	//operator +
	FVector2D operator+(const FVector2D& other) const;
	FVector2D operator+(const float value) const;
	FVector2D operator+(const double value) const;
	FVector2D operator+(const int32 value) const;

	//operator +=
	const FVector2D& operator+=(const FVector2D& other);
	const FVector2D& operator+=(const float value);
	const FVector2D& operator+=(const double value);
	const FVector2D& operator+=(const int32 value);

	//operator ++
	const FVector2D& operator ++();
	FVector2D operator ++(int);

	//operator -
	FVector2D operator-(const FVector2D& other) const;
	FVector2D operator-(const float other) const;
	FVector2D operator-(const double other) const;
	FVector2D operator-(const int32 other) const;

	//operator -=
	const FVector2D& operator-=(const FVector2D& other);
	const FVector2D& operator-=(const float value);
	const FVector2D& operator-=(const double value);
	const FVector2D& operator-=(const int32 value);

	//operator --
	const FVector2D& operator --();
	FVector2D operator --(int);

	//operator *
	FVector2D operator*(const FVector2D& other) const;
	FVector2D operator*(const float other) const;
	FVector2D operator*(const double other) const;
	FVector2D operator*(const int32 other) const;

	//operator *=
	const FVector2D& operator*=(const FVector2D& other);
	const FVector2D& operator*=(const float value);
	const FVector2D& operator*=(const double value);
	const FVector2D& operator*=(const int32 value);

	//operator /
	FVector2D operator/(const FVector2D& other) const;
	FVector2D operator/(const float other) const;
	FVector2D operator/(const double other) const;
	FVector2D operator/(const int32 other) const;

	//operator /=
	const FVector2D& operator/=(const FVector2D& other);
	const FVector2D& operator/=(const float value);
	const FVector2D& operator/=(const double value);
	const FVector2D& operator/=(const int32 value);

	//operator ==
	bool operator ==(const FVector2D& other);

	//operator !=
	bool operator !=(const FVector2D& other);

	//둘 다 벡터의 크기
	float Length() const;
	float Size() const;

	//벡터의 정규화
	void Normalize();

	//벡터의 내적
	float Dot(const FVector2D& other) const;

	//두 벡터 사이의 거리
	float Distance(const FVector2D& other) const;

	//벡터의 정규화
	static FVector2D Normalize(const FVector2D& other);

	static FVector2D Zero;



};







