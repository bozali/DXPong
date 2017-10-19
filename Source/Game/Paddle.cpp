#include <Rendering/ConstantBuffers.h>
#include <Game/Paddle.h>


Paddle::Paddle(_In_ ID3D11Device* pDevice)
{
    static PCVertex s_aVertices[] =
    {
        { { -10.0f, -10.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
        { { -10.0f,  50.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
        { {  10.0f,  50.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
        { {  10.0f, -10.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
    };

    Super::m_BoundingBox.Extents = DirectX::XMFLOAT3 { 10.0f, 50.0f, 0.0f };
    Super::m_BoundingBox.Center = DirectX::XMFLOAT3 { 0.0f, 0.0f, 0.0f };

    static DWORD s_adwIndices[] =
    {
        0, 1, 2,
        0, 2, 3
    };

    Super::m_dwIndexCount = ARRAYSIZE(s_adwIndices);

    {
        HRESULT hResult = E_FAIL;

        D3D11_BUFFER_DESC VertexBufferDesc { NULL };
        VertexBufferDesc.ByteWidth = sizeof(PCVertex) * ARRAYSIZE(s_aVertices);
        VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        VertexBufferDesc.StructureByteStride = NULL;
        VertexBufferDesc.CPUAccessFlags = NULL;
        VertexBufferDesc.MiscFlags = NULL;

        D3D11_SUBRESOURCE_DATA VertexBufferData { NULL };
        VertexBufferData.SysMemSlicePitch = NULL;
        VertexBufferData.pSysMem = s_aVertices;
        VertexBufferData.SysMemPitch = NULL;

        hResult = pDevice->CreateBuffer(&VertexBufferDesc, &VertexBufferData, Super::m_pVertexBuffer.GetAddressOf());
        ComException::ThrowIfFailed(hResult);


        D3D11_BUFFER_DESC IndexBufferDesc { NULL };
        IndexBufferDesc.ByteWidth = sizeof(PCVertex) * ARRAYSIZE(s_aVertices);
        IndexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        IndexBufferDesc.StructureByteStride = NULL;
        IndexBufferDesc.CPUAccessFlags = NULL;
        IndexBufferDesc.MiscFlags = NULL;

        D3D11_SUBRESOURCE_DATA IndexBufferData { NULL };
        IndexBufferData.SysMemSlicePitch = NULL;
        IndexBufferData.pSysMem = s_adwIndices;
        IndexBufferData.SysMemPitch = NULL;

        hResult = pDevice->CreateBuffer(&IndexBufferDesc, &IndexBufferData, Super::m_pIndexBuffer.GetAddressOf());
        ComException::ThrowIfFailed(hResult);
    }

    Super::m_Matrix = DirectX::XMMatrixIdentity();
}

