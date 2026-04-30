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

// 정점(버텍스) 버퍼 구조체
struct FVertexBuffer
{
    //GPU에 있는 버텍스 버퍼
    ComPtr<ID3D11Buffer> _buffer = nullptr;

    //정점 한개당 크기
    int32 _size = 0;

    //정점의 개수
    int32 _count = 0;

    //GPU 전송 전 RAM에 들고 있는 정점의 정보
    std::vector<byte> _data;

};


// 정점을 어떻게 구성할것인지 알려주는 버퍼 구조체
struct FIndexBuffer
{
    // GPU에 있는 인덱스 버퍼
    ComPtr<ID3D11Buffer> _buffer = nullptr;

    // 인덱스 한개의 크기
    int32 _size = 0;

    // 인덱스의 총 개수 
    int32 _count = 0;

    // 데이터 해석방식
    DXGI_FORMAT _fmt = DXGI_FORMAT_UNKNOWN; //인덱스 하나의 데이터 타입 = 아직 미정

    //GPU 전송 전 RAM에 들고 있는 인덱스 데이터 
    std::vector<byte> _data;

};

//정점 구조체(색, 위치)
struct FVertexColor
{
    FVector3D _pos;
    FVector4D _color;

    FVertexColor(){} //매개변수없이 만들때 -> 멤버 변수가 기본값으로 초기화
    FVertexColor(const FVector3D& pos, const FVector4D& color) : _pos(pos), _color(color)
    { }
    FVertexColor(float x, float y, float z, float r, float g, float b, float a)
    {
        _pos._x = x;
        _pos._y = y;
        _pos._z = z;

        _color._x = r;
        _color._y = g;
        _color._z = b;
        _color._w = a;

    }
};