cbuffer ColorBuffer : register(b0)
{
    float4 color;
}

struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
    
    float2 maxFrame : MAX_FRAME;
    float2 curFrame : CUR_FRAME;
    float4 color : COLOR;
};

Texture2D map : register(t0);
SamplerState samp : register(s0);

float4 PS(PixelInput input) : SV_TARGET
{
    float2 uv = input.uv + (input.curFrame / input.maxFrame);
    
    float4 tex = map.Sample(samp, uv);
    
    return tex * input.color;
}