#pragma once

#include <Config/DX.h>


class StaticMesh
{
protected:
    typedef StaticMesh                      Super;

    Microsoft::WRL::ComPtr<ID3D11Buffer>    m_pVertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer>    m_pIndexBuffer;
    DWORD                                   m_dwIndexCount;
public:
    virtual void __stdcall Render(_In_ ID3D11DeviceContext* pDeviceContext, _In_ ID3D11Buffer* pcbEveryPrimitive) = 0;
};

