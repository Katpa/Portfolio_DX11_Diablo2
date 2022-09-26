cbuffer World : register(b0)
{
    matrix world;
}

cbuffer View : register(b1)
{
    matrix view;
}

cbuffer Projection : register(b2)
{
    matrix projection;
}

struct VertexInput
{
    float4 pos : POSITION;
    float2 uv : UV;
    
    matrix transform : INSTANCE_TRANSFORM;
    float2 maxFrame : INSTANCE_MAX_FRAME;
    float2 curFrame : INSTANCE_CUR_FRAME;
    float4 color : INSTANCE_COLOR;
};

struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
    
    float2 maxFrame : MAX_FRAME;
    float2 curFrame : CUR_FRAME;
    float4 color : COLOR;
};

PixelInput VS(VertexInput input)
{
    PixelInput output;
    
    output.pos = mul(input.pos, input.transform);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
    
    output.uv = input.uv;
    
    output.maxFrame = input.maxFrame;
    output.curFrame = input.curFrame;
    output.color = input.color;
    
    return output;
}