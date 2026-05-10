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

//instanceID  <--이건 자동으로 들어옵니다.
VS_Output_Tex TileInstanceVS(VS_Input_Tex input, uint instanceID : SV_InstanceID)
{
    VS_Output_Tex output = (VS_Output_Tex) 0;
 
    FTileInstanceData inst = gTileInstance[instanceID];
    
    float3 pos = input.Pos;
    pos.x = pos.x * inst.size.x + inst.worldPos.x;
    pos.y = pos.y * inst.size.y + inst.worldPos.y;
    pos.z = 1.f;
    
    output.Pos = mul(float4(pos, 1.f), gWVP);
    
    if (input.UV.x == 0.f)
        output.UV.x = inst.uvLT.x;
    else
        output.UV.x = inst.uvRB.x;
    
    if (input.UV.y == 0.f)
        output.UV.y = inst.uvLT.y;
    else
        output.UV.y = inst.uvRB.y;
    
    return output;
}

PS_Output_Single TileInstancePS(VS_Output_Tex input)
{
    PS_Output_Single output = (PS_Output_Single) 0;
    
    float4 color = gBaseTexture.Sample(gBaseSample, input.UV);
    output.Color = color;
    
    return output;
}

struct VS_Output_Line
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
};

VS_Output_Line TileLineInstanceVS(float3 Pos : POSITION, uint instanceID : SV_InstanceID)
{
    VS_Output_Line output = (VS_Output_Line) 0;
   
    FTileLineInstanceData inst = gTileLineInstance[instanceID];
    float3 pos = Pos;
    pos.x = pos.x * inst.size.x + inst.worldPos.x;
    pos.y = pos.y * inst.size.y + inst.worldPos.y;
    pos.z = 1.f;
    
    output.Pos = mul(float4(pos, 1.f), gWVP);
    output.Color = inst.color;
    
    return output;
}

PS_Output_Single TileLineInstancePS(VS_Output_Line input)
{
    PS_Output_Single output = (PS_Output_Single) 0;
    
    output.Color = input.Color;
    
    return output;
}
    