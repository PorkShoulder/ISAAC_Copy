#include "Share.fx"

struct VS_Input_Color
{
    float3 Pos : POSITION; //CPU 버퍼에서 이 부분은 위치정보
    float4 Color : Color; //CPU 버퍼에서 이 부분은 색상 정보
};

struct VS_Output_Color
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
};

//버텍스 쉐이더
VS_Output_Color ColorMeshVS(VS_Input_Color input)
{
    //0을 구조체 타입으로 형변환해서 대입해주면 구조체 모든 멤버가 0으로 초기화
    VS_Output_Color output = (VS_Output_Color) 0;
    
    //output.Pos의 x, y, z에는 input.Pos의 x, y, z가 들어가고, w에는 1이 들어간다.
    output.Pos = mul(float4(input.Pos, 1.f), gWVP);
    //output.Pos = float4(input.Pos, 1.f);
    output.Color = input.Color;
    
    return output;
}

//픽셀쉐이더
PS_Output_Single ColorMeshPS(VS_Output_Color input)
{
    PS_Output_Single output = (PS_Output_Single) 0;
    
    output.Color = input.Color;
    
    //output.Color.r = 1;
    return output;
}

float4 FrameMeshVS(float3 Pos : POSITION) : SV_POSITION
{
    float4 output = (float4) 0;
    
    float3 InputPos = Pos;
    
    output = mul(float4(InputPos, 1.f), gWVP);
    
    return output;
}

PS_Output_Single FrameMeshPS(float4 Pos : SV_POSITION)
{
    PS_Output_Single output = (PS_Output_Single) 0;
    
    output.Color = gColor;
    
    return output;
}



