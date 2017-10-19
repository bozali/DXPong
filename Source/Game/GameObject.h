#pragma once

#include <Rendering/StaticMesh.h>

class GameObject
    : public StaticMesh
{
protected:
    typedef GameObject      Super;

    DirectX::BoundingBox    m_BoundingBox;
    // DirectX::XMFLOAT3       m_Position;
    DirectX::XMMATRIX       m_Matrix;
public:
    void __stdcall SetPosition(_In_ const FLOAT X, _In_ const FLOAT Y, _In_ const FLOAT Z);
    void __stdcall Render(_In_ ID3D11DeviceContext* pDeviceContext, _In_ ID3D11Buffer* pcbEveryPrimitive) final override;

    __forceinline DirectX::BoundingBox& __stdcall GetBounding() { return m_BoundingBox; }
    __forceinline DirectX::XMFLOAT3& __stdcall GetPosition() { return m_BoundingBox.Center; }
    __forceinline DirectX::XMMATRIX& __stdcall GetMatrix() { return m_Matrix; }
};
