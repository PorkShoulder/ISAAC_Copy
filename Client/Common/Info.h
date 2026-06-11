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

enum class eActorType
{
    Actor,
    Pawn,
    
    // 개별 UI
    Tile,
    Player, 
    Obstacle,
    Door,
    Item,
    Npc,
    Monster,
    End
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
        Press,
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
    // 플레이어
    COLLISION_CHANNEL_PLAYER,
    COLLISION_CHANNEL_PLAYER_BULLET,
    // 몬스터
    COLLISION_CHANNEL_MONSTER,
    COLLISION_CHANNEL_MONSTER_BULLET,
    COLLISION_CHANNEL_MONSTER_DETECT,
    // 장애물
    COLLISION_CHANNEL_OBSTACLE,

    COLLISION_CHANNEL_ITEM,
    COLLISION_CHANNEL_DOOR,
    COLLISION_CHANNEL_BOSS,
    COLLISION_CHANNEL_NPC,
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
    COLLISION_RESPONSE_IGNORE,      //충돌 무시
    COLLISION_RESPONSE_BLOCK,       //충돌 막힘
    COLLISION_RESPONSE_OVERLAP,     //충돌 겹침
    COLLISION_RESPONSE_END,
};

//현재 충돌 상태
enum eCollisionState
{
    COLLISION_STATE_RELEASE,    //충돌했다가 떨어질때
    COLLISION_STATE_BLOCK,      //처음 충돌할때
    COLLISION_STATE_OVERLAP,    //겹쳐져 있을때
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
// 아이템 능력치
// ***이곳으로 뺀 이유는 나머지 액티브나 소모품과 시너지 및 소모품이나 액티브도 능력치를 올릴수있게끔 하려고
struct FItemStat
{
    float attackPower = 0.f;
    float speed = 0.f;
    float hp = 0.f;
    float range = 0.f;
    float fireRate = 0.f;
};

// 소모품을 따로 뺀이유 -> 나중에 UI에서 보여져야할떄 이곳에 있는게 표기하기 더 편할거 같음
struct FConsumableStat
{
    int coin = 0;
    int key = 0;
    int bomb = 0;
};

// 룸 이동정보
enum class eRoomDir
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    COUNT,
    END
};


// ================ Door =======================
enum class eDoorType
{
    NORMAL,
    KEY,
    COIN,
    BOSS_CLEAR,
    END
};
inline const char* DoorTypeName[] = {
    "NORMAL",
    "KEY",
    "COIN",
    "BOSS_CLEAR"
};
struct FDoorSpawnData
{
    std::string textureName; // 아틀라스 텍스처 이름
    std::wstring texturePath; // 아틀라스 텍스처 경로
  
    FVector4D frame;  //문틀
    FVector4D left;   //좌짝
    FVector4D right;  //우짝
    FVector4D openImage;  //열린상태

    eDoorType doorType = eDoorType::NORMAL;
    bool bOpen = false;
    bool bBattle = false;

    FVector2D renderSize = {64.f, 48.f};        // 렌더 크기
    FVector2D collisionSize = { 40.f, 24.f };   // 충돌체 크기
    eRoomDir exitDir = eRoomDir::UP;
};

// ================ Monster =======================
enum class eMonsterType
{
    Monster_knight,
    Monster_fatty,
    Monster_BOSS,
    End
    // 나중에 이름 달아야함.
};
inline const char* MonsterTypeName[] = { "Knight", "Fatty", "Boss" };

struct FMonsterData
{
    std::string textureName;    // 아틀라스 텍스처 이름
    std::wstring texturePath;   // 아틀라스 텍스처 경로
    float moveSpeed = 100.f;
    float chargeSpeed = 300.f;
    float detectRange = 200.f;
    float attackPower = 1.f;
    float hp = 10.f;
    eMonsterType monsterType = eMonsterType::End;

    FVector2D renderSize = { 64.f, 48.f };      // 렌더 크기
    FVector2D collisionSize = { 40.f, 24.f };   // 충돌체 크기
    
    // 애니메이션 프레임 
    std::vector<FVector4D> idleFrames;
    std::vector<FVector4D> moveFrontFrames;
    std::vector<FVector4D> moveSideFrames;
    std::vector<FVector4D> moveBackFrames; // 없는 경우도 있음.
    std::vector<FVector4D> deathFrames;

};

inline const FMonsterData MonsterInfo[] =
{
    // Monster_knight 
    {
        "", L"",
        100.f,   // moveSpeed
        300.f,   // chargeSpeed
        200.f,   // detectRange
        1.f,     // attackPower
        10.f,    // hp
        eMonsterType::Monster_knight,
    },
    // Monster_fatty
    {
        "", L"",
        60.f,
        200.f,
        400.f,
        3.f,
        100.f,
        eMonsterType::Monster_fatty,
    },
    //boss
    {
        "", L"",
        60.f,
        200.f,
        400.f,
        3.f,
        100.f,
        eMonsterType::Monster_BOSS,
    }

};

// ================ Obstacle =======================
enum class eObstacleType
{
    PIT,        // 구덩이
    FIRE,       // 모닥불
    IRON_WALL,  // 철벽
    ROCK_WALL,  // 돌벽
    SPIKE,      // 가시밭
    WEB,        // 거미줄
    POOP,       // 똥
    END
};
inline const char* ObstacleTypeName[] = { "Pit", "Fire", "IronWall", "RockWall", "Spike", "Web", "POOP"}; //  enum과 순서가 일치해야함 반드시!.

struct FObstacleData
{
    //경로
    std::string textureName;
    std::wstring texturePath;
    
    //상태 동작
    bool canFlyPass = false;
    bool canWalkPass = false;
    bool bulletBreakable = false;
    bool bulletPassThrough = false;
    bool bombBreakable = false;
    bool damage = false;
    float damageAmount = 1.f;
    int32 hp = 1;
    eObstacleType obstacleType = eObstacleType::END;

    // 렌더 크기 및 충돌체 크기
    
    FVector2D renderSize = { 50.f ,50.f };
    FVector2D collisionSize = { 50.f ,50.f };

    std::vector<FVector4D> animFrames;

};

inline const FObstacleData ObstaclePresets[] = 
{
    // PIT
     {
         "", L"",
         true, true, false, true, false, false,
         0.f, 0,
         eObstacleType::PIT,

     },
     // FIRE
     {
         "", L"",
         true, true, true, false, true, true,
         1.f, 1,
         eObstacleType::FIRE,
     },
     // IRON_WALL
     {
         "", L"",
         true, true, false, false, true, false,
         0.f, 1,
         eObstacleType::IRON_WALL,
     },
     // ROCK_WALL
     {
         "", L"",
         false, false, false, false, true, false,
         0.f, 1,
         eObstacleType::ROCK_WALL,
     },
     // SPIKE
     {
         "", L"",
         true, true, false, true, true, true,
         1.f, 1,
         eObstacleType::SPIKE,
     },
     // WEB
     {
         "", L"",
         true, true, false, true, true, false,
         0.f, 1,
         eObstacleType::WEB,
     },
     // POOP
     {
         "", L"",
         true, false, true, false, true, false,
         0.f, 4,
         eObstacleType::POOP,
     },
};
// ================ Item =======================
enum class eItemType
{
    PASSIVE,    // 패시브 아이템 (능력치, 효과)
    ACTIVE,     // 액티브 아이템
    COIN,       // 코인
    KEY,        // 열쇠
    BOMB,       // 폭탄
    HEART,      // 하트 (체력회복)
    END
};
inline const char* ItemTypeName[] = { "PASSIVE", "ACTIVE", "COIN", "KEY", "BOMB", "HEART" };

// 아이템 특수효과.
enum eItemEffect : uint32
{
    EFFECT_NONE     = 0,
    EFFECT_HOMING   = 1<<0,
};

struct FItemData
{
    std::string textureName;
    std::wstring texturePath;
    eItemType itemType = eItemType::PASSIVE;
    FVector2D renderSize = { 32.f, 32.f };
    FVector2D collisionSize = { 32.f, 32.f };
    std::vector<FVector4D> itemFrames;          // 아이템 자체 표시 프레임
    std::vector<FVector4D> equipHeadFrames;     // 습득시 머리 외형
    std::vector<FVector4D> equipBodyFrames;     // 습득시 몸통 외형
    std::vector<FVector4D> bulletFrames;       // 총알 외형 프레임

    // 추가 능력치
    float bonusDamage = 0.f;
    float bonusSpeed = 0.f;
    int32 bonusHp = 0;

    uint32 effect = EFFECT_NONE;
    FConsumableStat cost;
    
    int32 consumableAmount = 1; // 소모품일 때 지급 수량

};


// ================ NPC =======================
enum class eNpcType
{
    CONSUMABLE, //코인 열쇠 폭탄 지급
    EFFECT_ITEM,       //패시브, 액티브 아이템 지급
    END
};
inline const char* NpcTypeName[] = { "CONSUMABLE", "EFFECT_ITEM" };

struct FNpcData
{
    std::string textureName;
    std::wstring texturePath;
    eNpcType npcType = eNpcType::END;

    FVector2D renderSize = { 32.f, 32.f };
    FVector2D collisionSize = { 32.f, 32.f };

    // 애니메이션
    std::vector<FVector4D> idleFrames;

    // 비용 (플레이어가 지불) — 각 최대 5
    FConsumableStat cost;   // coin, key, bomb

    // 보상
    FConsumableStat rewardConsumable;   // rewardType == CONSUMABLE일 때
    FItemData       rewardItem;         // rewardType == ITEM일 때
};

/// ////////////////////////////////////////

