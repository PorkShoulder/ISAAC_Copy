#pragma once

struct FResolution
{
	uint32 _width  = 0;
	uint32 _height = 0;
};

//위치, 크기, 회전
struct FTransform
{
    FVector3D _scale;
    FVector3D _position;
    FRotator _rotation;
};