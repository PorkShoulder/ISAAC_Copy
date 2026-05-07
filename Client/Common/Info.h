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

//쉐이더 타입결정
namespace SHADER_TYPE
{
    enum eType
    {
        VERTEX = 0x1,
        PIXEL = 0x2,
        GRAPHIC = VERTEX | PIXEL
    };
}

//샘플러
//텍스쳐 를 그릴 때 해당 텍스쳐의 픽셀을 어떻게 가져올것인지를 정하는 방법
//UV 좌표 --> 텍스쳐의 좌표를 0과 1사이로 
enum eTextureSampleType
{
    TEXTURE_SAMPLE_POINT,       //포인터 샘플러 --> UV 좌표에서 가장 가까운 픽셀 1개를 그대로 가져옴(보간 없음)
    TEXTURE_SAMPLE_LINEAR,      //선형 샘플러   --> UV 좌표 주변 4개의 픽셀을 거리에 따라 가중 평균, 부드럽게 보간
    TEXTURE_SAMPLE_ANISOTROPIC, //이방성 샘플러 --> 카메라가 비스듬히 볼 때, 품질 유지, 경사진 표면에서 선형샘플러보다 휠씬 선명함, 근데 무거움
    TEXURE_SAMPLE_END
};

enum class eAssetType
{
    MESH,
    TEXTURE,
    MATERIAL,
    ANIMATION,
    SOUND,
    FONT,
    END
};