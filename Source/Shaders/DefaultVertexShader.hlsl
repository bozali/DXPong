#include "ConstantBuffers.hlsli"

PixelShaderInput main(VertexShaderInput Input)
{
    PixelShaderInput Output = (PixelShaderInput)0;
    Output.Position = mul(mul(World, ModelWorld), Input.Position);
    Output.Color = Input.Color;

    return Output;
}