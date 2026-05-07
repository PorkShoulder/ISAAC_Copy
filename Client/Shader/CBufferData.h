#pragma once
#include "Common/Matrix.h"
#include "Common/Vector2D.h"
#include "Common/Vector3D.h"
#include "Common/Vector4D.h"

/*
공간변환
로컬[local] --> 월드[world] --> 뷰[view] --> 투영[projection] --> 화면[screen]

월드행렬 = 크기 * 회전 * 이동
뷰행렬: 월드를 카메라 기준으로 재정의
투영행렬: 3D좌표를 2D 평면으로
*/

struct FTransformCBufferData
{
    FMatrix _world;
    FMatrix _view;
    FMatrix _proj;
    FMatrix _wv;    // _world * _view
    FMatrix _wvp;   // _world * _view * _proj
};

struct FColorCBufferData
{
    FVector4D _color;
};

struct FMaterialCBufferData
{
    FVector4D _basaColor;
    float _opacity;
    int32 _textureWidht;
    int32 _textureHeight;
    float _empty;               // 16바이트 맞춤용 더미
};

struct FAnimation2DCBufferData
{
    FVector2D _uvLT;
    FVector2D _uvRB;
    int32 _animFilp;
    FVector3D _empty;
};

struct FTileMapCBufferData
{
    FVector2D _uvLT;
    FVector2D _uvRB;
};
