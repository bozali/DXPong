
struct VertexShaderInput
{
    float4 Position : POSITION;
    float4 Color    : COLOR;
};

struct PixelShaderInput
{
    float4 Position : SV_POSITION;
    float4 Color    : COLOR;
};

cbuffer ConstantBufferChangesEveryFrame : register(b0)
{
    float4x4 World;
}

cbuffer ConstantBufferChangesEveryPrimitive : register(b1)
{
    float4x4 ModelWorld;
};



