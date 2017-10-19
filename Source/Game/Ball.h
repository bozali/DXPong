#pragma once

#include <Game/GameObject.h>
#include <Game/Paddle.h>


class Ball
    : public GameObject
{
    DirectX::XMFLOAT2   m_Direction;
    FLOAT               m_Speed;
public:
    Ball(_In_ ID3D11Device* pDevice);

    __forceinline void __stdcall Move(_In_ const FLOAT X, _In_ const FLOAT Y)
    {
        Super::m_BoundingBox.Center.x += X;
        Super::m_BoundingBox.Center.y += Y;
    }

    __forceinline void __stdcall SetDirection(_In_ const FLOAT X, _In_ const FLOAT Y)
    {
        m_Direction.x *= X;
        m_Direction.y *= Y;
    }

    __forceinline DirectX::XMFLOAT2& __stdcall GetDirection() { return m_Direction; }
    __forceinline FLOAT __stdcall GetSpeed() { return m_Speed; }
};
