#include "Share.fx"

struct VS_Input_Tex
{
    float3 Pos : POSITION;
    float2 UV : TEXCOORD;
};

struct VS_Output_Tex
{
    float4 Pos : SV_POSITION;
    float2 UV : TEXCOORD;
};


VS_Output_Tex TileVS(VS_Input_Tex input)
{
    VS_Output_Tex output = (VS_Output_Tex) 0;
    
    float3 pos = input.Pos;
    
    output.Pos = mul(float4(pos, 1.f), gWVP);
    
    //UV 계산
    if (input.UV.x == 0.f)
        output.UV.x = gTileLTUV.x;
    else
        output.UV.x = gTileRBUV.x;
    
    if (input.UV.y == 0.f)
        output.UV.y = gTileLTUV.y;
    else
        output.UV.y = gTileRBUV.y;
    
    return output;
}

PS_Output_Single TilePS(VS_Output_Tex input)
{
    PS_Output_Single output = (PS_Output_Single) 0;
    
    float4 color = gBaseTexture.Sample(gBaseSample, input.UV);
    output.Color = color;
    
    return output;
}