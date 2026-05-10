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

cbuffer Sprite : register(b2)
{
    float4 gSpriteTint;
}

VS_Output_Tex SpriteVS(VS_Input_Tex input)
{
    VS_Output_Tex output = (VS_Output_Tex) 0;
    
    float3 pos = input.Pos;
    
    output.Pos = mul(float4(pos, 1.f), gWVP);
    
    //UV 계산
    output.UV = UpdateAnimation2D(input.UV);
    //output.UV = input.UV;
    
    return output;
}


PS_Output_Single SpritePS(VS_Output_Tex input)
{
    PS_Output_Single output = (PS_Output_Single) 0;
    
    float4 color = gBaseTexture.Sample(gBaseSample, input.UV);
    output.Color = color * gSpriteTint;
    
    return output;
}
