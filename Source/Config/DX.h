#pragma once

#pragma warning (push)
#pragma warning (disable : 4005)
#pragma warning (disable : 28251)
    #include <DirectXCollision.h>
    #include <DirectXMath.h>
    #include <wrl/client.h>
    #include <D3DX11.h>
    #include <D3DX10.h>
    #include <D3D11.h>
    #include <D3D10.h>
    #include <DInput.h>
#pragma warning (pop)


#define DX_HHR(hResult, pText)                      \
if(FAILED(hResult)) {                               \
    MessageBox(nullptr, pText, nullptr, g_MBType);  \
    return EXIT_FAILURE;                            \
}


#include <stdio.h>


class ComException
{
    HRESULT m_hResult;
public:
    __forceinline ComException(_In_ HRESULT hResult)
        : m_hResult(hResult)
    {
    }

    __forceinline const TCHAR*
        __stdcall What() const
    {
        static const TCHAR s_aString[64] { NULL };
        wprintf_s(s_aString, TEXT("Failed with HRESULT of %08X"), static_cast<UINT>(m_hResult));
        return s_aString;
    }

    __forceinline static void
        __stdcall ThrowIfFailed(_In_ HRESULT hResult)
    {
        if (FAILED(hResult))
        {
            throw ComException(hResult);
        }
    }
};

