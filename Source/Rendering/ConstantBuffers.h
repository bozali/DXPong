#pragma once

#include <Config/DX.h>


/** InputLayout Description */
static D3D11_INPUT_ELEMENT_DESC s_aInputElementDescs[]
{
    { "POSITION", 0,    DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    {    "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};


/** Position-Color-Vertex */
struct PCVertex
{
    DirectX::XMFLOAT3 Position;
    DirectX::XMFLOAT4 Color;
};

struct ConstantBufferChangesEveryFrame
{
    DirectX::XMMATRIX World;
};

struct ConstantBufferChangesEveryPrimitive
{
    DirectX::XMMATRIX ModelWorld;
};



