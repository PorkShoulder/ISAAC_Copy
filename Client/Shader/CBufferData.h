#pragma once
#include "Common/Matrix.h"
#include "Common/Vector2D.h"
#include "Common/Vector3D.h"
#include "Common/Vector4D.h"

/*
공간변환
로컬[local] --> 월드[world] --> 뷰[view] --> 투영[projection] --> 화면[screen]
로컬[local] : 모델을 만들때의 기준(3DMax, blender..)

월드[world] : 전체 게임 세상에서의 트랜스폼
월드행렬 = 크기 * 회전 * 이동

뷰[view]
뷰행렬
월드를 카메라 기준으로 재정의
월드 좌표계가 아닌 카메라 위치를 기준으로 물체의 좌표를 다시 계산한다.

투영
투영 행렬
3D좌표를 2D 평면으로
*/

struct FTransformCBufferData
{
    FMatrix _world;
    FMatrix _view;
    FMatrix _proj;
    FMatrix _wv; // _world * _view 
    FMatrix _wvp;// _world * _view * _proj
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
    float _empty;               //16바이트 맞춤을 위해서 넣어주는 더미바이트
};

struct FAnimation2DCBufferData
{
    FVector2D _uvLT;
    FVector2D _uvRB;
    int32 _animFilp;
    FVector3D _empty;
};

struct FSpriteCBufferData
{
    FVector4D _tint;
};

struct FTileMapCBufferData
{
    FVector2D _uvLT;
    FVector2D _uvRB;
};

struct FUIBrushData
{
    FVector4D _tint;
    FVector2D _uvLT;
    FVector2D _uvRB;
    int32 _animEnable;
    int32 _textureEnable;
    FVector2D _empty;
};

struct FProgressBarData
{
    float _percent = 1.f;
    int32 _barDir = 0;
    FVector2D _empty;
};
