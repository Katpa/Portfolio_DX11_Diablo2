cbuffer ColorBuffer : register(b0)
{
    float4 color;
}

cbuffer FrameBuffer : register(b1)
{
    float2 maxFrame;
    float2 curFrame;
}

struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
};

Texture2D map : register(t0);
SamplerState samp : register(s0);

float4 PS(PixelInput input) : SV_TARGET
{
    float2 uv = input.uv + (curFrame / maxFrame);
    
    float4 tex = map.Sample(samp, uv);
    
    //if(tex.a == 0)
    //    return float4(0, 0, 0, 0);
    
    if(tex.r >= 0.95f && tex.g >= 0.95f && tex.b >= 0.95f)
        return float4(0, 0, 0, 0);    
    
    return tex * color;
}