struct PS_Output_Single //
{
    float4 Color : SV_TARGET;
};

cbuffer Transform : register(b0) // cbuffer -> c++에서 GPU로 전달하는 상수 데이터를 쉐이더에서 읽기 위한 메모리 바인딩 영역
{
    // CPU에서 GPU로 데이털르 넘기는 통로 
    matrix gWorld;      //월드 행렬
    matrix gView;       //뷰 행렬
    matrix gProj;       //투영 행렬
    matrix gWV;         // _world * _view 
    matrix gWVP;        // _world * _view * _proj
}

cbuffer Color : register(b0)
{
    float4 gColor;
}

cbuffer Material : register(b1)
{
    float4 gMtrlBaseColor;
    float gMtrlOpacity;
    int gMtrlTextureWidth;
    int gMtrlTextureHeight;
    float gEmpty;

}

cbuffer Animation2D : register(b1)
{
    float2 gAnim2DLTUV;
    float2 gAnim2DRBUV;
    int gAnimFilp;
    float3 gAnimEmpty;
}

cbuffer TileMap : register(b2)
{
    float2 gTileLTUV;
    float2 gTileRBUV;
}

struct FTileInstanceData
{
    float2 worldPos;
    float2 uvLT;
    float2 uvRB;
    float2 size;
};

struct FTileLineInstanceData
{
    float2 worldPos;
    float4 color;
    float2 size;
};

SamplerState gBaseSample : register(s0);

Texture2D gBaseTexture : register(t0);

StructuredBuffer<FTileInstanceData> gTileInstance : register(t1);

StructuredBuffer<FTileLineInstanceData> gTileLineInstance : register(t2);

float2 UpdateAnimation2D(float2 UV)
{
    float2 result = (float2) 0.f;
    
    if (UV.x == 0.f)
        result.x = gAnim2DLTUV.x;
    else
        result.x = gAnim2DRBUV.x;
    
    if (UV.y == 0.f)
        result.y = gAnim2DLTUV.y;
    else
        result.y = gAnim2DRBUV.y;
    
    if (gAnimFilp == 1)
        result.x = 1 - result.x;
    
    return result;
}
