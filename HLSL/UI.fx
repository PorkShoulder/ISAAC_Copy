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

cbuffer UI : register(b3)
{
    float4 gBrushTint;
    float2 gBrushAnimLTUV;
    float2 gBrushAnimRBUV;
    int gBrushAnimEnable;
    int gBrushTextureEnable;
    float2 gBrushEmpty;
}

cbuffer ProgressBar : register(b4)
{
    float gPercent;
    int gBarDir;
    float2 gProgressBarEmpty;
}

float2 UpdateAnimation2DUI(float2 UV)
{
    if(gBrushAnimEnable == 0)
        return UV;
    
    float2 result = (float2) 0;
    
    if(UV.x == 0.f)
        result.x = gBrushAnimLTUV.x;
    else
        result.x = gBrushAnimRBUV.x;
    
    if (UV.y == 0.f)
        result.y = gBrushAnimLTUV.y;
    else
        result.y = gBrushAnimRBUV.y;
    
    return result;
}


VS_Output_Tex UIVS(VS_Input_Tex input)
{
    VS_Output_Tex output = (VS_Output_Tex) 0;
    
    float3 pos = input.Pos;
    
    output.Pos = mul(float4(pos, 1.f), gWVP);
    output.UV = UpdateAnimation2DUI(input.UV);
    
    return output;
}

PS_Output_Single UIPS(VS_Output_Tex input)
{
    PS_Output_Single output = (PS_Output_Single) 0;
    
    float4 color = float4(1.f, 1.f, 1.f, 1.f);
    
    if(gBrushTextureEnable)
        color = gBaseTexture.Sample(gBaseSample, input.UV);
    
    output.Color = color * gBrushTint;
    
    return output;
}

VS_Output_Tex ProgressBarFillVS(VS_Input_Tex input)
{
    VS_Output_Tex output = (VS_Output_Tex) 0;
    float3 pos = input.Pos;
    
    output.Pos = mul(float4(pos, 1.f), gWVP);
    output.UV = UpdateAnimation2DUI(input.UV);
    
    return output;
}

PS_Output_Single ProgressBarFillPS(VS_Output_Tex input)
{
    PS_Output_Single output = (PS_Output_Single) 0;
    
    //clip(-1) : 픽셀 버림
    //clip(1) : 픽셀 유지
    //Clip(0) : 픽셀 유지
    if(gBarDir == 0)
        clip(input.UV.x <= gPercent ? 1 : -1);
    else
        clip(input.UV.y >= (1.f - gPercent) ? 1 : -1);
    
    float4 color = float4(1.f, 1.f, 1.f, 1.f);
    
    if (gBrushTextureEnable)
        color = gBaseTexture.Sample(gBaseSample, input.UV);
    
    output.Color = color * gBrushTint;
    
    return output;
}
