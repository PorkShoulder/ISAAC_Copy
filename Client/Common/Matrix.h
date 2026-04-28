#pragma once
#include "Vector4D.h"


//내부 데이터를 16바이트의 배수에 맞게 강제로 맟추게 
//컴파일러에게 알려주는 키워드
//GPU가 16바이트 단위로 읽음.

_declspec(align(16)) union FMatrix
{
	// 월드 행렬 = 크기행렬 * 회전행렬 * 이동행렬
	// 위치 회전 크기 -> Dx렌더링 파이프라인에 넘기기 위한 용도.

	DirectX::XMMATRIX _m;
	struct
	{
		// 
		float _11, _12, _13, _14;
		float _21, _22, _23, _24;
		float _31, _32, _33, _34;
		float _41, _42, _43, _44;
	};
    FVector4D _v[4] = {};

    FMatrix();
    FMatrix(const FMatrix& m);
    FMatrix(FMatrix&& m) noexcept;
    FMatrix(const DirectX::XMMATRIX& m);
    FMatrix(DirectX::XMMATRIX&& m) noexcept;
    FMatrix(const FVector4D v[4]);

    FVector4D& operator[] (int32 index);

    const FMatrix& operator = (const FMatrix& m);
    const FMatrix& operator = (const DirectX::XMMATRIX& m);
    const FMatrix& operator = (const FVector4D v[4]);

    FMatrix operator * (const FMatrix& m) const;
    FMatrix operator * (const DirectX::XMMATRIX& m) const;

    //단위행렬을 만드는 함수
    void Indentity();

    //전치 행렬
    void Transpose();

    //역행렬
    void Inverse();

    //크기
    void Scaling(const struct FVector3D& v);
    void Scaling(float x, float y, float z);
    void Scaling(const FVector2D& v);
    void Scaling(float x, float y);


    //회전
    void Rotation(const FVector3D& v);
    void Rotation(float x, float y, float z);
    void RotationX(float x);
    void RotationY(float y);
    void RotationZ(float z);
    void RotationAxis(const FVector3D& axis, float angle);

    //이동
    void Translation(const FVector3D& v);
    void Translation(float x, float y, float z);
    void Translation(const FVector2D& v);
    void Translation(float x, float y);

    void ExtractScale(OUT FVector3D& outVal) const;
    void ExtractPosition(OUT FVector3D& outVal) const;
    void ExtractEuler(OUT FRotator& outVal) const;

    FVector3D ExtractScale() const;
    FVector3D ExtractPosition() const;
    FVector3D ExtractEuler() const;

};