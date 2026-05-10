#pragma once

class Utility
{
public:
    template<typename T>
    static T Clamp(const T& value, const T& minValue, const T& maxValue)
    {
        return max(minValue, min(value, maxValue));
    }
};


struct FResolution
{
    uint32 _width = 0;
    uint32 _height = 0;
};

//정점(버텍스) 버퍼 구조체
struct FVertexBuffer
{
    //gpu에 있는 버텍스버퍼
    ComPtr<ID3D11Buffer> _buffer = nullptr;

    //정점 한개당 크기
    int32 _size = 0;

    //정점의 개수
    int32 _count = 0;

    //gpu 전송 전 ram에 들고 있는 정점의 정보들
    std::vector<byte> _data;
};

//정점을 어떻게 구성할껀지 알려주는 버퍼 구조체
struct FIndexBuffer
{
    //gpu에 있는 인덱스버퍼
    ComPtr<ID3D11Buffer> _buffer = nullptr;

    //인덱스 한개의 크기
    int32 _size = 0;

    //인덱스의 총 개수
    int32 _count = 0;

    //데이터 해석 방식
    DXGI_FORMAT _fmt = DXGI_FORMAT_UNKNOWN;

    //gpu 전송 전 ram에 들고 있는 인덱스데이터
    std::vector<byte> _data;
};

//정점 구조체(색, 위치)
struct FVertexColor
{
    FVertexColor() {}
    FVertexColor(const FVector3D& pos, const FVector4D& color) : _pos(pos), _color(color)
    {
    }
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

    FVector3D _pos;
    FVector4D _color;
};

//위치, 크기, 회전
struct FTransform
{
    FVector3D _scale;
    FVector3D _position;
    FRotator _rotation;
};

struct FTransformMatrix
{
    FMatrix _scale;
    FMatrix _rotation;
    FMatrix _tranlate;
    FMatrix _world;

};

struct FAABB2D
{
    FVector2D _min;
    FVector2D _max;
};

struct FOBB2D
{
    FVector2D _center;
    FVector2D _axis[2];
    FVector2D _halfSize;
};

struct FVertexTexture
{
    FVector3D _pos;
    FVector2D _uv;

    FVertexTexture(float x, float y, float z, float u, float v) : _pos(x, y, z), _uv(u, v)
    {

    }
};

struct FAnimationFrame
{
    FVector2D _start;
    FVector2D _size;
};



enum class eObjectType
{
    OBJECT_TYPE_TEST_OBJ,
    END
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

namespace SHADER_TYPE
{
    enum eType
    {
        VERTEX = 0x1,
        PIXEL = 0x2,
        GRAPHIC = VERTEX | PIXEL
    };
}

namespace INPUT_TYPE
{
    enum eType
    {
        DOWN,       //키를 눌렀을 때
        HOLD,       //키를 누르고 있을 때
        UP,         //키를 뗐을 때
        END
    };
}

enum class eInputSystemType
{
    DINPUT,
    WINDOW,
    END
};

namespace MOUSE_BUTTON_TYPE
{
    enum Type
    {
        LButton,
        RButton,
        Wheel,
        End
    };
}

namespace AXIS_TYPE
{
    enum Type
    {
        X,
        Y,
        Z,
        END
    };
}

//채널
//각 충돌체가 어떤 충돌체랑 충돌을 할껀지 혹은 무시할껀지에
//대해서 관리해놓은 타입
//PLAYER , MONSTER, ITEM
//PLAYER - MONSTER --> 충돌
//PLAYER - ITEM --> 충돌
//MONSTER - PLAYER --> 충돌
//ITEM - PLAYER --> 충돌
//ITME /- MONSTER --> 충돌 x
enum eCollisionChannel : unsigned char
{
    COLLISION_CHANNEL_PLAYER,
    COLLISION_CHANNEL_MONSTER,
    COLLISION_CHANNEL_BULLET,
    COLLISION_CHANNEL_ITEM,
    COLLISION_CHANNEL_BOSS,
    COLLISION_CHANNEL_END,
};

//충돌을 진행할때는 충돌 모양이 필요함
//모양에 따라서 어떻게 연산을 할지가 달라져요
enum class eCollisionShape
{
    AABB,               //축에 정렬된 사각형
    OBB,                //기울어진 사각형
    Sphere,             //원
    End
};

//각 충돌체끼리 어떤 반응을 할것인지
//ex ) 부딪힐때만 반응, 겹칠때도 반응, 무시
enum eCollisionResponse : unsigned char
{
    COLLISION_RESPONSE_IGNORE,      //충돌 x
    COLLISION_RESPONSE_BLOCK,       //부딪힘
    COLLISION_RESPONSE_OVERLAP,     //겹침
    COLLISION_RESPONSE_END,
};

//현재 충돌 상태
enum eCollisionState
{
    COLLISION_STATE_RELEASE, //충돌했다가 떨어질때
    COLLISION_STATE_BLOCK,  //처음 충돌할때
    COLLISION_STATE_OVERLAP,//겹쳐져 있을때
    COLLISION_STATE_END
};


//샘플러
//텍스쳐 를 그릴 때 해당 텍스쳐의 픽셀을 어떻게 가져올것인지를 정하는 방법
//UV 좌표 --> 텍스쳐의 좌표를 0과 1사이로 
enum eTextureSampleType
{
    TEXTURE_SAMPLE_POINT, //포인터 샘플러 --> UV 좌표에서 가장 가까운 픽셀 1개를 그대로 가져옴(보간 없음)
    TEXTURE_SAMPLE_LINEAR,//선형 샘플러   --> UV 좌표 주변 4개의 픽셀을 거리에 따라 가중 평균, 부드럽게 보간
    TEXTURE_SAMPLE_ANISOTROPIC, //이방성 샘플러 --> 카메라가 비스듬히 볼 때, 품질 유지, 경사진 표면에서 선형샘플러보다 휠씬 선명함, 근데 무거움
    TEXURE_SAMPLE_END
};


enum class eRenderState
{
    BLEND,
    DEPTHSTENCIL,
    END
};


enum class eAnimTextureType
{
    SPRITE,
    FRAME,
    END
};

enum class eTileType
{
    NORMAL,         //갈수 있는 곳
    BLOCK,          //못 가는 곳
    END
};

namespace AI_EVENT_STATE
{
    enum Type
    {
        EXIT,
        ENTER,
        TICK,
        END
    };
}

enum class eTransitionRule
{
    AND,
    OR,
    END
};

namespace UI_BUTTON_STATE
{
    enum Type
    {
        ENABLE,
        DISABLE,
        HOVERED,
        PRESSED,
        CLICK,
        END
    };
}

namespace UI_BUTTON_EVENT_STATE
{
    enum Type
    {
        HOVERED,
        UNHOVERED,
        PRESSED,
        CLICK,
        END
    };
}

enum class BUTTONTYPE
{
    NUM,
    END
};
