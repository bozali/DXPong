#include <Rendering/ConstantBuffers.h>
#include <Game/GameObject.h>


void
__stdcall GameObject::SetPosition(_In_ const FLOAT X, _In_ const FLOAT Y, _In_ const FLOAT Z)
{
    Super::m_BoundingBox.Center.x = X;
    Super::m_BoundingBox.Center.y = Y;
    Super::m_BoundingBox.Center.z = Z;

    // m_Position.x = X;
    // m_Position.y = Y;
    // m_Position.z = Z;
}


void
__stdcall GameObject::Render(_In_ ID3D11DeviceContext* pDeviceContext, _In_ ID3D11Buffer* pcbEveryPrimitive)
{
    UINT uStride = sizeof(PCVertex);
    UINT uOffset = 0;

    // m_Matrix = DirectX::XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
    m_Matrix = DirectX::XMMatrixTranslation(Super::m_BoundingBox.Center.x, Super::m_BoundingBox.Center.y, Super::m_BoundingBox.Center.z);
    ConstantBufferChangesEveryPrimitive cbChangesEveryPrimitive { m_Matrix };

    /** Update ConstantBuffers */
    pDeviceContext->UpdateSubresource(pcbEveryPrimitive, 0, nullptr, &cbChangesEveryPrimitive, 0, 0);
    pDeviceContext->VSSetConstantBuffers(1, 1, &pcbEveryPrimitive);

    /** Rendering */
    pDeviceContext->IASetVertexBuffers(0, 1, Super::m_pVertexBuffer.GetAddressOf(), &uStride, &uOffset);
    pDeviceContext->IASetIndexBuffer(Super::m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    pDeviceContext->DrawIndexed(Super::m_dwIndexCount, 0, 0);
}
