#include <Rendering/ConstantBuffers.h>
#include <Rendering/StaticMesh.h>

#include <Game/GameObject.h>
#include <Game/Paddle.h>
#include <Game/Ball.h>

#include <Config/Platform.h>
#include <Config/DX.h>

#include <vector>
#include <memory>
#include <string>


#define CompileVertexShaderFromFile(pSourceFile, spBuffer)  \
D3DX11CompileFromFile(pSourceFile, nullptr, nullptr, "main", "vs_5_0", 0, 0, nullptr, spBuffer.GetAddressOf(), nullptr, nullptr)
#define CompilePixelShaderFromFile(pSourceFile, spBuffer)   \
D3DX11CompileFromFile(pSourceFile, nullptr, nullptr, "main", "ps_5_0", 0, 0, nullptr, spBuffer.GetAddressOf(), nullptr, nullptr)


__forceinline static LRESULT
__stdcall WindowProc(
    _In_ HWND hWindow,
    _In_ UINT uMessage,
    _In_ WPARAM wParam,
    _In_ LPARAM lParam)
{
    switch (uMessage)
    {
    case WM_DESTROY:
    case WM_CLOSE:
    case WM_QUIT:
        PostQuitMessage(NULL);
        return NULL;
        break;

    default:
        return DefWindowProc(hWindow, uMessage, wParam, lParam);
        break;
    }
}


constexpr UINT g_MBType = MB_OK | MB_ICONERROR;
constexpr UINT g_uClientWidth = 1024u;
constexpr UINT g_uClientHeight = 720u;


INT
__stdcall WinMain(
    _In_        HINSTANCE hInstance,
    _In_opt_    HINSTANCE hPrevInstance,
    _In_        LPSTR lpCmdLine,
    _In_        INT CmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(CmdShow);

    HWND hWindow = nullptr;

#pragma region Create Window
    {
        WNDCLASS WindowClass { NULL };
        WindowClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
        WindowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
        WindowClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
        WindowClass.style = CS_HREDRAW | CS_VREDRAW;
        WindowClass.lpszClassName = TEXT("DXTEST");
        WindowClass.lpfnWndProc = WindowProc;
        WindowClass.lpszMenuName = nullptr;
        WindowClass.hInstance = hInstance;
        WindowClass.cbClsExtra = NULL;
        WindowClass.cbWndExtra = NULL;

        if (!RegisterClass(&WindowClass))
        {
            MessageBox(nullptr, TEXT("Failed to register WNDCLASS"), nullptr, g_MBType);
            return EXIT_FAILURE;
        }

        RECT ClientRect
        {
            0l, 0l,
            static_cast<LONG>(g_uClientWidth),
            static_cast<LONG>(g_uClientHeight)
        };

        DirectX::XMINT2 ScreenSize =
        {
            GetSystemMetrics(SM_CXSCREEN),
            GetSystemMetrics(SM_CYSCREEN)
        };

        AdjustWindowRect(&ClientRect, WS_OVERLAPPEDWINDOW, FALSE);

        hWindow = CreateWindow(
            TEXT("DXTEST"), TEXT("DXTEST"),
            WS_OVERLAPPEDWINDOW,
            static_cast<INT>((ScreenSize.x / 2) - (ClientRect.right - ClientRect.left) / 2),
            static_cast<INT>((ScreenSize.y / 2) - (ClientRect.bottom - ClientRect.top) / 2),
            static_cast<INT>(ClientRect.right - ClientRect.left),
            static_cast<INT>(ClientRect.bottom - ClientRect.top),
            nullptr, nullptr, nullptr, nullptr);

        if (hWindow == nullptr)
        {
            MessageBox(nullptr, TEXT("Failed to create HWND"), nullptr, g_MBType);
            return EXIT_FAILURE;
        }

        ShowWindow(hWindow, SW_SHOW);
    }
#pragma endregion


    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRenderTargetView = nullptr;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext = nullptr;
    Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain = nullptr;
    Microsoft::WRL::ComPtr<ID3D11Device> pDevice = nullptr;

#pragma region Initialize D3D
    {
        DXGI_MODE_DESC DxgiModeDesc { NULL };
        DxgiModeDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        DxgiModeDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        DxgiModeDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        DxgiModeDesc.RefreshRate.Denominator = 1u;
        DxgiModeDesc.RefreshRate.Numerator = 60u;
        DxgiModeDesc.Height = g_uClientHeight;
        DxgiModeDesc.Width = g_uClientWidth;

        DXGI_SWAP_CHAIN_DESC DxgiSwapChainDesc { NULL };
        DxgiSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        DxgiSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        DxgiSwapChainDesc.BufferDesc = DxgiModeDesc;
        DxgiSwapChainDesc.OutputWindow = hWindow;
        DxgiSwapChainDesc.SampleDesc.Quality = 0;
        DxgiSwapChainDesc.SampleDesc.Count = 1;
        DxgiSwapChainDesc.BufferCount = 1;
        DxgiSwapChainDesc.Windowed = TRUE;
        DxgiSwapChainDesc.Flags = NULL;

        HRESULT hResult = E_FAIL;
        hResult = D3D11CreateDeviceAndSwapChain(
            nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, NULL,
            nullptr, NULL, D3D11_SDK_VERSION, &DxgiSwapChainDesc,
            pSwapChain.GetAddressOf(), pDevice.GetAddressOf(),
            nullptr, pDeviceContext.GetAddressOf());
        DX_HHR(hResult, TEXT("Failed to create ID3D11DeviceContext, ID3D11Device and IDXGISwapChain"));


        Microsoft::WRL::ComPtr<ID3D11Texture2D> pRenderTargetViewBackbuffer = nullptr;
        hResult = pSwapChain->GetBuffer(
            0, __uuidof(ID3D11Texture2D),
            reinterpret_cast<void**>(pRenderTargetViewBackbuffer.GetAddressOf()));
        DX_HHR(hResult, TEXT("Failed to get buffer from IDXGISwapChain"));

        hResult = pDevice->CreateRenderTargetView(
            pRenderTargetViewBackbuffer.Get(), nullptr,
            pRenderTargetView.GetAddressOf());
        DX_HHR(hResult, TEXT("Failed to create ID3D11RenderTargetView"));


        pDeviceContext->OMSetRenderTargets(1, pRenderTargetView.GetAddressOf(), nullptr);
    }
#pragma endregion


    Microsoft::WRL::ComPtr<ID3D11VertexShader> pDefaultVertexShader = nullptr;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pDefaultPixelShader = nullptr;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> pDefaultInputLayout = nullptr;

#pragma region Creating and Compiling Shaders
    {
        Microsoft::WRL::ComPtr<ID3D10Blob> pDefaultVertexShaderBuffer = nullptr;
        Microsoft::WRL::ComPtr<ID3D10Blob> pDefaultPixelShaderBuffer = nullptr;

        HRESULT hResult = E_FAIL;

        /** Compiling */
        hResult = CompileVertexShaderFromFile(TEXT("Source/Shaders/DefaultVertexShader.hlsl"), pDefaultVertexShaderBuffer);
        DX_HHR(hResult, TEXT("Failed to compile DefaultVertexShader.hlsl from file"));

        hResult = CompilePixelShaderFromFile(TEXT("Source/Shaders/DefaultPixelShader.hlsl"), pDefaultPixelShaderBuffer);
        DX_HHR(hResult, TEXT("Failed to compile DefaultPixelShader.hlsl from file"));

        /** Creating */
        hResult = pDevice->CreateVertexShader(
            pDefaultVertexShaderBuffer->GetBufferPointer(),
            pDefaultVertexShaderBuffer->GetBufferSize(),
            nullptr, pDefaultVertexShader.GetAddressOf());
        DX_HHR(hResult, TEXT("Failed to create VertexShader"));

        hResult = pDevice->CreatePixelShader(
            pDefaultPixelShaderBuffer->GetBufferPointer(),
            pDefaultPixelShaderBuffer->GetBufferSize(),
            nullptr, pDefaultPixelShader.GetAddressOf());
        DX_HHR(hResult, TEXT("Failed to create PixelShader"));

        hResult = pDevice->CreateInputLayout(
            s_aInputElementDescs, ARRAYSIZE(s_aInputElementDescs),
            pDefaultVertexShaderBuffer->GetBufferPointer(),
            pDefaultVertexShaderBuffer->GetBufferSize(),
            pDefaultInputLayout.GetAddressOf());
        DX_HHR(hResult, TEXT("Failed to create InputLayout"));

        /** Set pipeline */
        pDeviceContext->VSSetShader(pDefaultVertexShader.Get(), nullptr, NULL);
        pDeviceContext->PSSetShader(pDefaultPixelShader.Get(), nullptr, NULL);
        pDeviceContext->IASetInputLayout(pDefaultInputLayout.Get());
    }
#pragma endregion


    Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBufferChangesEveryPrimitive = nullptr;
    Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBufferChangesEveryFrame = nullptr;

#pragma region Creating Constantbuffers
    {
        D3D11_BUFFER_DESC ConstantBufferChangesEveryPrimitiveDesc { NULL };
        ConstantBufferChangesEveryPrimitiveDesc.ByteWidth = sizeof(ConstantBufferChangesEveryPrimitive);
        ConstantBufferChangesEveryPrimitiveDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        ConstantBufferChangesEveryPrimitiveDesc.Usage = D3D11_USAGE_DEFAULT;
        ConstantBufferChangesEveryPrimitiveDesc.StructureByteStride = NULL;
        ConstantBufferChangesEveryPrimitiveDesc.CPUAccessFlags = NULL;
        ConstantBufferChangesEveryPrimitiveDesc.MiscFlags = NULL;

        HRESULT hResult = E_FAIL;
        hResult = pDevice->CreateBuffer(
            &ConstantBufferChangesEveryPrimitiveDesc, nullptr, pConstantBufferChangesEveryPrimitive.GetAddressOf());
        DX_HHR(hResult, TEXT("Failed to create ConstantBufferChangesEveryPrimitive"));


        D3D11_BUFFER_DESC ConstantBufferChangesEveryFrameDesc { NULL };
        ConstantBufferChangesEveryFrameDesc.ByteWidth = sizeof(ConstantBufferChangesEveryFrame);
        ConstantBufferChangesEveryFrameDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        ConstantBufferChangesEveryFrameDesc.Usage = D3D11_USAGE_DEFAULT;
        ConstantBufferChangesEveryFrameDesc.StructureByteStride = NULL;
        ConstantBufferChangesEveryFrameDesc.CPUAccessFlags = NULL;
        ConstantBufferChangesEveryFrameDesc.MiscFlags = NULL;

        hResult = pDevice->CreateBuffer(
            &ConstantBufferChangesEveryFrameDesc, nullptr, pConstantBufferChangesEveryFrame.GetAddressOf());
        DX_HHR(hResult, TEXT("Failed to create ConstantBufferChangesEveryFrame"));


    }
#pragma endregion


    Microsoft::WRL::ComPtr<IDirectInputDevice8> pDIKeyboard;
    LPDIRECTINPUT8 DirectInput;

#pragma region Input
    {
        HRESULT hResult = E_FAIL;
        hResult = DirectInput8Create(
            hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
            reinterpret_cast<void**>(&DirectInput), nullptr);
        DX_HHR(hResult, TEXT("Failed to create DirectInput8"));

        hResult = DirectInput->CreateDevice(
            GUID_SysKeyboard, pDIKeyboard.GetAddressOf(), nullptr);
        DX_HHR(hResult, TEXT("Failed to create KeyboardDevice"));
        
        hResult = pDIKeyboard->SetDataFormat(&c_dfDIKeyboard);
        DX_HHR(hResult, TEXT("Failed to set Data Format"));

        hResult = pDIKeyboard->SetCooperativeLevel(hWindow, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
        DX_HHR(hResult, TEXT("Failed to set cooperativelevel"));
    }
#pragma endregion


    D3D11_VIEWPORT Viewport { NULL };
    Viewport.Height = static_cast<FLOAT>(g_uClientHeight);
    Viewport.Width = static_cast<FLOAT>(g_uClientWidth);
    Viewport.TopLeftX = 0.0f;
    Viewport.TopLeftY = 0.0f;
    Viewport.MinDepth = 0.0f;
    Viewport.MaxDepth = 1.0f;

    pDeviceContext->RSSetViewports(1, &Viewport);


    std::vector<std::shared_ptr<GameObject>> GameObjects;

    try {
        GameObjects.emplace_back(std::make_shared<Paddle>(pDevice.Get()));
        GameObjects.emplace_back(std::make_shared<Paddle>(pDevice.Get()));
        GameObjects.emplace_back(std::make_shared<Ball>(pDevice.Get()));
    }
    catch (const ComException& E) {
        MessageBox(nullptr, E.What(), nullptr, g_MBType);
    }

    GameObjects[0]->SetPosition(-static_cast<FLOAT>(g_uClientWidth / 2.0f - 35.0f), 0.0f, 0.0f);
    GameObjects[1]->SetPosition(static_cast<FLOAT>(g_uClientWidth / 2.0f - 35.0f), 0.0f, 0.0f);
    GameObjects[2]->SetPosition(0.0f, 0.0f, 0.0f);


    const FLOAT DeltaTime = 1.0f / 60.0f;

    while (hWindow != nullptr)
    {
        MSG Message { NULL };
        if (PeekMessage(&Message, nullptr, NULL, NULL, PM_REMOVE))
        {
            TranslateMessage(&Message);
            DispatchMessage(&Message);

            if (Message.message == WM_QUIT)
            {
                if (hWindow != nullptr)
                {
                    DestroyWindow(hWindow);
                    hWindow = nullptr;
                }
            }
        }
        else
        {
            /** Update */

#pragma region Control
            BYTE aKeyboardState[256];
            BYTE aKeyboardState2[256];
            pDIKeyboard->Acquire();

            pDIKeyboard->GetDeviceState(sizeof(aKeyboardState), reinterpret_cast<LPVOID>(&aKeyboardState));
            pDIKeyboard->GetDeviceState(sizeof(aKeyboardState2), reinterpret_cast<LPVOID>(&aKeyboardState2));

            if (aKeyboardState[DIK_W])
            {
                auto Position = GameObjects[0]->GetPosition();
                GameObjects[0]->SetPosition(Position.x, Position.y + 10.0f * DeltaTime, 0.0f);
            }
            else if(aKeyboardState[DIK_S])
            {
                auto Position = GameObjects[0]->GetPosition();
                GameObjects[0]->SetPosition(Position.x, Position.y - 10.0f * DeltaTime, 0.0f);
            }


            if (aKeyboardState2[DIK_UP])
            {
                auto Position = GameObjects[1]->GetPosition();
                GameObjects[1]->SetPosition(Position.x, Position.y + 10.0f * DeltaTime, 0.0f);
            }
            else if (aKeyboardState2[DIK_DOWN])
            {
                auto Position = GameObjects[1]->GetPosition();
                GameObjects[1]->SetPosition(Position.x, Position.y - 10.0f * DeltaTime, 0.0f);
            }
#pragma endregion


            for (UINT8 uIndex = 0; uIndex < 2; ++uIndex)
            {
                /** Get the Ball */
                if (GameObjects[2]->GetBounding().Intersects(GameObjects[uIndex]->GetBounding()))
                {
                    dynamic_cast<Ball*>(GameObjects[2].get())->SetDirection(-1.0f, -1.0f);
                }
            }

            auto BallDirectionX = dynamic_cast<Ball*>(GameObjects[2].get())->GetDirection().x;
            auto BallDirectionY = dynamic_cast<Ball*>(GameObjects[2].get())->GetDirection().y;
            auto BallSpeed = dynamic_cast<Ball*>(GameObjects[2].get())->GetSpeed();

            dynamic_cast<Ball*>(GameObjects[2].get())->Move(
                BallSpeed * BallDirectionX * DeltaTime, BallSpeed * BallDirectionY * DeltaTime);


            /** Rendering */
            D3DXCOLOR ClearColor { 0.0f, 0.0f, 0.0f, 0.0f };
            pDeviceContext->ClearRenderTargetView(pRenderTargetView.Get(), ClearColor);

            ConstantBufferChangesEveryFrame cbChangesEveryFrame
            {
                DirectX::XMMatrixOrthographicLH(static_cast<FLOAT>(g_uClientWidth), static_cast<FLOAT>(g_uClientHeight), 0.0f, 1.0f)
            };

            /** Update and set the EveryFrame ConstantBuffer */
            pDeviceContext->UpdateSubresource(pConstantBufferChangesEveryFrame.Get(), 0, nullptr, &cbChangesEveryFrame, 0, 0);
            pDeviceContext->VSSetConstantBuffers(0, 1, pConstantBufferChangesEveryFrame.GetAddressOf());

            for (auto& Object : GameObjects)
            {
                /** Render each Mesh */
                Object->Render(pDeviceContext.Get(), pConstantBufferChangesEveryPrimitive.Get());
            }

            pSwapChain->Present(0u, 0u);
        }
    }


    if (hWindow != nullptr)
    {
        DestroyWindow(hWindow);
        hWindow = nullptr;
    }

    return EXIT_SUCCESS;
}

