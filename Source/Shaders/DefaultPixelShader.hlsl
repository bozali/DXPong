#include "ConstantBuffers.hlsli"

float4 main(PixelShaderInput Input) : SV_TARGET
{
    return Input.Color;
}