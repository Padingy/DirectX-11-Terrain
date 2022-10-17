//--------------------------------------------------------------------------------------
// File: main.cpp
//
// This application demonstrates animation using matrix transformations
//
// http://msdn.microsoft.com/en-us/library/windows/apps/ff729722.aspx
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#define _XM_NO_INTRINSICS_

#include "main.h"

DirectX::XMFLOAT4 g_EyePosition(0.0f, 0, 1, 1.0f);

//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
//HRESULT		InitWindow(HINSTANCE hInstance, int nCmdShow);
//HRESULT		InitDevice();
//HRESULT		InitMesh();
//HRESULT		InitWorld(int width, int height);
//void		CleanupDevice();
//void Update();
//void		Render();



//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
//HINSTANCE               g_hInst = nullptr;
//HWND                    g_hWnd = nullptr;
//D3D_DRIVER_TYPE         g_driverType = D3D_DRIVER_TYPE_NULL;
//D3D_FEATURE_LEVEL       g_featureLevel = D3D_FEATURE_LEVEL_11_0;
//ID3D11Device*           g_pd3dDevice = nullptr;
//ID3D11Device1*          g_pd3dDevice1 = nullptr;
//ID3D11DeviceContext*    g_pImmediateContext = nullptr;
//ID3D11DeviceContext1*   g_pImmediateContext1 = nullptr;
//IDXGISwapChain*         g_pSwapChain = nullptr;
//IDXGISwapChain1*        g_pSwapChain1 = nullptr;
//ID3D11RenderTargetView* g_pRenderTargetView = nullptr;
//ID3D11Texture2D*        g_pDepthStencil = nullptr;
//ID3D11DepthStencilView* g_pDepthStencilView = nullptr;
//ID3D11VertexShader*     g_pVertexShader = nullptr;
//ID3D11VertexShader*     g_pVertexShaderTes = nullptr;
//
//ID3D11ShaderResourceView* _pTextureRV = nullptr;
//
//ID3D11RasterizerState* wireFrame = nullptr;
//ID3D11RasterizerState* solidFill = nullptr;
//ID3D11RasterizerState* rsState = nullptr;
//
//ID3D11PixelShader*      g_pPixelShader = nullptr;
//
//ID3D11HullShader* g_pHullShader = nullptr;
//ID3D11DomainShader* g_pDomainShader = nullptr;
//
//ID3D11InputLayout*      g_pVertexLayout = nullptr;
//ID3D11InputLayout*      g_pVertexLayoutTes = nullptr;
//
//ID3D11Buffer*           g_pConstantBuffer = nullptr;
//
//ID3D11Buffer*           g_pLightConstantBuffer = nullptr;
//
//XMMATRIX                g_View;
//XMMATRIX                g_Projection;
//
//int						g_viewWidth;
//int						g_viewHeight;
//
//string currentView = "Camera";
//
//bool tessellationToggled = true;
//
//Camera* camera = nullptr;
//POINT currentMouseMov;
//POINT lastMouseMov;
//float moveAmountX;
//float moveAmountY;
//
//DrawableGameObject*		g_GameObject;
//
//Terrain* grid = nullptr;
//
//Skeleton* skeleton;


//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int WINAPI wWinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow )
{
    UNREFERENCED_PARAMETER( hPrevInstance );
    UNREFERENCED_PARAMETER( lpCmdLine );
    Application* application = new Application();

    if( FAILED( application->InitWindow( hInstance, nCmdShow ) ) )
        return 0;

    if( FAILED( application->InitDevice() ) )
    {
        application->CleanupDevice();
        return 0;
    }

    // Main message loop
    MSG msg = {0};
    while( WM_QUIT != msg.message )
    {
        
        if( PeekMessage( &msg, nullptr, 0, 0, PM_REMOVE ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
            application->Update();
            application->Render();
    }

    application->CleanupDevice();

    return ( int )msg.wParam;
}


//--------------------------------------------------------------------------------------
// Register class and create window
//--------------------------------------------------------------------------------------
HRESULT Application::InitWindow( HINSTANCE hInstance, int nCmdShow )
{
    // Register class
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof( WNDCLASSEX );
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon( hInstance, ( LPCTSTR )IDI_TUTORIAL1 );
    wcex.hCursor = LoadCursor( nullptr, IDC_ARROW );
    wcex.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = L"TutorialWindowClass";
    wcex.hIconSm = LoadIcon( wcex.hInstance, ( LPCTSTR )IDI_TUTORIAL1 );
    if( !RegisterClassEx( &wcex ) )
        return E_FAIL;

    // Create window
    g_hInst = hInstance;
    RECT rc = { 0, 0, 1280, 720 };

	g_viewWidth = 1280;
	g_viewHeight = 720;

    AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );
    g_hWnd = CreateWindow( L"TutorialWindowClass", L"Direct3D 11 Tutorial 5",
                           WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
                           CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance,
                           nullptr );
    if( !g_hWnd )
        return E_FAIL;

    ShowWindow( g_hWnd, nCmdShow );

    return S_OK;
}


//--------------------------------------------------------------------------------------
// Helper for compiling shaders with D3DCompile
//
// With VS 11, we could load up prebuilt .cso files instead...
//--------------------------------------------------------------------------------------
HRESULT CompileShaderFromFile( const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut )
{
    HRESULT hr = S_OK;

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
    // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
    // Setting this flag improves the shader debugging experience, but still allows 
    // the shaders to be optimized and to run exactly the way they will run in 
    // the release configuration of this program.
    dwShaderFlags |= D3DCOMPILE_DEBUG;

    // Disable optimizations to further improve shader debugging
    dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    ID3DBlob* pErrorBlob = nullptr;
    hr = D3DCompileFromFile( szFileName, nullptr, nullptr, szEntryPoint, szShaderModel, 
        dwShaderFlags, 0, ppBlobOut, &pErrorBlob );
    if( FAILED(hr) )
    {
        if( pErrorBlob )
        {
            OutputDebugStringA( reinterpret_cast<const char*>( pErrorBlob->GetBufferPointer() ) );
            pErrorBlob->Release();
        }
        return hr;
    }
    if( pErrorBlob ) pErrorBlob->Release();

    return S_OK;
}


//--------------------------------------------------------------------------------------
// Create Direct3D device and swap chain
//--------------------------------------------------------------------------------------
HRESULT Application::InitDevice()
{
    HRESULT hr = S_OK;

    RECT rc;
    GetClientRect( g_hWnd, &rc );
    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };
    UINT numDriverTypes = ARRAYSIZE( driverTypes );

	D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE( featureLevels );

    for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
    {
        g_driverType = driverTypes[driverTypeIndex];
        hr = D3D11CreateDevice( nullptr, g_driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
                                D3D11_SDK_VERSION, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext );

        if ( hr == E_INVALIDARG )
        {
            // DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it
            hr = D3D11CreateDevice( nullptr, g_driverType, nullptr, createDeviceFlags, &featureLevels[1], numFeatureLevels - 1,
                                    D3D11_SDK_VERSION, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext );
        }

        if( SUCCEEDED( hr ) )
            break;
    }
    if( FAILED( hr ) )
        return hr;

    // Obtain DXGI factory from device (since we used nullptr for pAdapter above)
    IDXGIFactory1* dxgiFactory = nullptr;
    {
        IDXGIDevice* dxgiDevice = nullptr;
        hr = g_pd3dDevice->QueryInterface( __uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice) );
        if (SUCCEEDED(hr))
        {
            IDXGIAdapter* adapter = nullptr;
            hr = dxgiDevice->GetAdapter(&adapter);
            if (SUCCEEDED(hr))
            {
                hr = adapter->GetParent( __uuidof(IDXGIFactory1), reinterpret_cast<void**>(&dxgiFactory) );
                adapter->Release();
            }
            dxgiDevice->Release();
        }
    }
    if (FAILED(hr))
        return hr;

    // Create swap chain
    IDXGIFactory2* dxgiFactory2 = nullptr;
    hr = dxgiFactory->QueryInterface( __uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgiFactory2) );
    if ( dxgiFactory2 )
    {
        // DirectX 11.1 or later
        hr = g_pd3dDevice->QueryInterface( __uuidof(ID3D11Device1), reinterpret_cast<void**>(&g_pd3dDevice1) );
        if (SUCCEEDED(hr))
        {
            (void) g_pImmediateContext->QueryInterface( __uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&g_pImmediateContext1) );
        }

        DXGI_SWAP_CHAIN_DESC1 sd = {};
        sd.Width = width;
        sd.Height = height;
		sd.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;//  DXGI_FORMAT_R16G16B16A16_FLOAT;////DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.BufferCount = 1;

        hr = dxgiFactory2->CreateSwapChainForHwnd( g_pd3dDevice, g_hWnd, &sd, nullptr, nullptr, &g_pSwapChain1 );
        if (SUCCEEDED(hr))
        {
            hr = g_pSwapChain1->QueryInterface( __uuidof(IDXGISwapChain), reinterpret_cast<void**>(&g_pSwapChain) );
        }

        dxgiFactory2->Release();
    }
    else
    {
        // DirectX 11.0 systems
        DXGI_SWAP_CHAIN_DESC sd = {};
        sd.BufferCount = 1;
        sd.BufferDesc.Width = width;
        sd.BufferDesc.Height = height;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferDesc.RefreshRate.Numerator = 60;
        sd.BufferDesc.RefreshRate.Denominator = 1;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.OutputWindow = g_hWnd;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.Windowed = TRUE;

        hr = dxgiFactory->CreateSwapChain( g_pd3dDevice, &sd, &g_pSwapChain );
    }

    // Note this tutorial doesn't handle full-screen swapchains so we block the ALT+ENTER shortcut
    dxgiFactory->MakeWindowAssociation( g_hWnd, DXGI_MWA_NO_ALT_ENTER );

    dxgiFactory->Release();

    if (FAILED(hr))
        return hr;

    // Create a render target view
    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = g_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), reinterpret_cast<void**>( &pBackBuffer ) );
    if( FAILED( hr ) )
        return hr;

    hr = g_pd3dDevice->CreateRenderTargetView( pBackBuffer, nullptr, &g_pRenderTargetView );
    pBackBuffer->Release();
    if( FAILED( hr ) )
        return hr;

    // Create depth stencil texture
    D3D11_TEXTURE2D_DESC descDepth = {};
    descDepth.Width = width;
    descDepth.Height = height;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;
    hr = g_pd3dDevice->CreateTexture2D( &descDepth, nullptr, &g_pDepthStencil );
    if( FAILED( hr ) )
        return hr;

    // Create the depth stencil view
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
    descDSV.Format = descDepth.Format;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0;
    hr = g_pd3dDevice->CreateDepthStencilView( g_pDepthStencil, &descDSV, &g_pDepthStencilView );
    if( FAILED( hr ) )
        return hr;

    g_pImmediateContext->OMSetRenderTargets( 1, &g_pRenderTargetView, g_pDepthStencilView );

    // Setup the viewport
    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT)width;
    vp.Height = (FLOAT)height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    g_pImmediateContext->RSSetViewports( 1, &vp );

	hr = InitMesh();
	if (FAILED(hr))
	{
		MessageBox(nullptr,
			L"Failed to initialise mesh.", L"Error", MB_OK);
		return hr;
	}

	hr = InitWorld(width, height);
	if (FAILED(hr))
	{
		MessageBox(nullptr,
			L"Failed to initialise world.", L"Error", MB_OK);
		return hr;
	}

    D3D11_RASTERIZER_DESC wfdesc;
    ZeroMemory(&wfdesc, sizeof(D3D11_RASTERIZER_DESC));
    wfdesc.FillMode = D3D11_FILL_WIREFRAME;
    wfdesc.CullMode = D3D11_CULL_NONE;
    hr = g_pd3dDevice->CreateRasterizerState(&wfdesc, &wireFrame);

    D3D11_RASTERIZER_DESC sfdesc;
    ZeroMemory(&sfdesc, sizeof(D3D11_RASTERIZER_DESC));
    sfdesc.FillMode = D3D11_FILL_SOLID;
    sfdesc.CullMode = D3D11_CULL_NONE;
    hr = g_pd3dDevice->CreateRasterizerState(&sfdesc, &solidFill);

    rsState = wireFrame;

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplWin32_Init(g_hWnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pImmediateContext);
    ImGui::StyleColorsDark();

	//hr = grid->DefineGrid(1.0f, 1.0f, 25, 25, g_pd3dDevice, g_pImmediateContext);
	/*hr = g_GameObject.initMesh(g_pd3dDevice, g_pImmediateContext);
	if (FAILED(hr))
		return hr;*/

    return S_OK;
}

// ***************************************************************************************
// InitMesh
// ***************************************************************************************

HRESULT		Application::InitMesh()
{
	// Compile the vertex shader
	ID3DBlob* pVSBlob = nullptr;
	HRESULT hr = CompileShaderFromFile(L"shader.fx", "VS", "vs_5_0", &pVSBlob);
	if (FAILED(hr))
	{
		MessageBox(nullptr,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
	}

	// Create the vertex shader
	hr = g_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &g_pVertexShader);
	if (FAILED(hr))
	{
		pVSBlob->Release();
		return hr;
	}

    ID3DBlob* pVSTESBlob = nullptr;
    hr = CompileShaderFromFile(L"shader.fx", "VSTES", "vs_5_0", &pVSTESBlob);
    if (FAILED(hr))
    {
        MessageBox(nullptr,
            L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
        return hr;
    }

    // Create the vertex shader
    hr = g_pd3dDevice->CreateVertexShader(pVSTESBlob->GetBufferPointer(), pVSTESBlob->GetBufferSize(), nullptr, &g_pVertexShaderTes);
    if (FAILED(hr))
    {
        pVSTESBlob->Release();
        return hr;
    }

	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);

	// Create the input layout
	hr = g_pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(), &g_pVertexLayout);
	pVSBlob->Release();
	if (FAILED(hr))
		return hr;

	// Set the input layout
	g_pImmediateContext->IASetInputLayout(g_pVertexLayout);

    hr = g_pd3dDevice->CreateInputLayout(layout, numElements, pVSTESBlob->GetBufferPointer(),
        pVSTESBlob->GetBufferSize(), &g_pVertexLayoutTes);
    pVSTESBlob->Release();
    if (FAILED(hr))
        return hr;

    // Set the input layout
    g_pImmediateContext->IASetInputLayout(g_pVertexLayoutTes);

	// Compile the pixel shader
	ID3DBlob* pPSBlob = nullptr;
	hr = CompileShaderFromFile(L"shader.fx", "PS", "ps_5_0", &pPSBlob);
	if (FAILED(hr))
	{
		MessageBox(nullptr,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
	}

	// Create the pixel shader
	hr = g_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &g_pPixelShader);
	pPSBlob->Release();
	if (FAILED(hr))
		return hr;

    ID3DBlob* pHSBlob = nullptr;
    hr = CompileShaderFromFile(L"shader.fx", "HSMAIN", "hs_5_0", &pHSBlob);
    if (FAILED(hr))
    {
        MessageBox(nullptr,
            L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
        return hr;
    }

    hr = g_pd3dDevice->CreateHullShader(pHSBlob->GetBufferPointer(), pHSBlob->GetBufferSize(), nullptr, &g_pHullShader);
    pHSBlob->Release();
    if (FAILED(hr))
        return hr;

    ID3DBlob* pDSBlob = nullptr;
    hr = CompileShaderFromFile(L"shader.fx", "DSMAIN", "ds_5_0", &pDSBlob);
    if (FAILED(hr))
    {
        MessageBox(nullptr,
            L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
        return hr;
    }

    hr = g_pd3dDevice->CreateDomainShader(pDSBlob->GetBufferPointer(), pDSBlob->GetBufferSize(), nullptr, &g_pDomainShader);
    pDSBlob->Release();
    if (FAILED(hr))
        return hr;

	// Create the constant buffer
    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(SimpleVertex) * 24;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(WORD) * 36;        // 36 vertices needed for 12 triangles in a triangle list
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = g_pd3dDevice->CreateBuffer(&bd, nullptr, &g_pConstantBuffer);
	if (FAILED(hr))
		return hr;



	// Create the light constant buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(LightPropertiesConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = g_pd3dDevice->CreateBuffer(&bd, nullptr, &g_pLightConstantBuffer);
	if (FAILED(hr))
		return hr;


    

	return hr;
}

// ***************************************************************************************
// InitWorld
// ***************************************************************************************
HRESULT		Application::InitWorld(int width, int height)
{
	// Initialize the view matrix
	/*XMVECTOR Eye = XMVectorSet(0.0f, 0.5f, 0.5f, 0.0f);
	XMVECTOR At = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);*/
    XMFLOAT4 Eye = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
    XMFLOAT4 At = XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f);
    XMFLOAT4 Up = XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f);
    camera = new Camera(Eye, At, Up, g_viewWidth, g_viewHeight, 0.001f, D3D11_FLOAT32_MAX, 15000.0f, LookTo);
	
    //g_View = XMMatrixLookAtLH(Eye, At, Up);
    g_View = XMLoadFloat4x4(&camera->camera._view);

	// Initialize the projection matrix
	//g_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, width / (FLOAT)height, 0.01f, 100.0f);
	g_Projection = XMLoadFloat4x4(&camera->camera._projection);

    g_GameObject = new DrawableGameObject(g_pd3dDevice, g_pImmediateContext);
    grid = new Terrain(10.0f, 10.0f, 512, 512, 0.75, g_pd3dDevice, g_pImmediateContext);
    
    //skeleton = new Skeleton(g_pd3dDevice, g_pImmediateContext);

    SetCursorPos(320, 240);

	return S_OK;
}


//--------------------------------------------------------------------------------------
// Clean up the objects we've created
//--------------------------------------------------------------------------------------
void Application::CleanupDevice()
{
    g_GameObject->cleanup();

    // Remove any bound render target or depth/stencil buffer
    ID3D11RenderTargetView* nullViews[] = { nullptr };
    g_pImmediateContext->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);

    if( g_pImmediateContext ) g_pImmediateContext->ClearState();
    // Flush the immediate context to force cleanup
    if (g_pImmediateContext1) g_pImmediateContext1->Flush();
    g_pImmediateContext->Flush();

    if (g_pLightConstantBuffer)
        g_pLightConstantBuffer->Release();
    if (g_pVertexLayout) g_pVertexLayout->Release();
    if( g_pConstantBuffer ) g_pConstantBuffer->Release();
    if( g_pVertexShader ) g_pVertexShader->Release();
    if( g_pPixelShader ) g_pPixelShader->Release();
    if( g_pDepthStencil ) g_pDepthStencil->Release();
    if( g_pDepthStencilView ) g_pDepthStencilView->Release();
    if( g_pRenderTargetView ) g_pRenderTargetView->Release();
    if( g_pSwapChain1 ) g_pSwapChain1->Release();
    if( g_pSwapChain ) g_pSwapChain->Release();
    if( g_pImmediateContext1 ) g_pImmediateContext1->Release();
    if( g_pImmediateContext ) g_pImmediateContext->Release();


    ID3D11Debug* debugDevice = nullptr;
    g_pd3dDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&debugDevice));

    if (g_pd3dDevice1) g_pd3dDevice1->Release();
    if (g_pd3dDevice) g_pd3dDevice->Release();

    // handy for finding dx memory leaks
    debugDevice->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);

    if (debugDevice)
        debugDevice->Release();
}


//--------------------------------------------------------------------------------------
// Called every time the application receives a message
//--------------------------------------------------------------------------------------
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
        return true;

    PAINTSTRUCT ps;
    HDC hdc;

    switch( message )
    {
	case WM_LBUTTONDOWN:
	{
		int xPos = GET_X_LPARAM(lParam);
		int yPos = GET_Y_LPARAM(lParam);
		break;
	}
    case WM_PAINT:
        hdc = BeginPaint( hWnd, &ps );
        EndPaint( hWnd, &ps );
        break;

    case WM_DESTROY:
        PostQuitMessage( 0 );
        break;

        // Note that this tutorial does not handle resizing (WM_SIZE) requests,
        // so we created the window without the resize border.

    default:
        return DefWindowProc( hWnd, message, wParam, lParam );
    }

    return 0;
}

void Application::SetupLightForRender()
{
    Light light;
    light.Enabled = static_cast<int>(true);
    light.LightType = PointLight;
    light.Color = XMFLOAT4(Colors::White);
    light.SpotAngle = XMConvertToRadians(45.0f);
    light.ConstantAttenuation = 1.0f;
    light.LinearAttenuation = 1;
    light.QuadraticAttenuation = 1;


    // set up the light
    XMFLOAT4 LightPosition(g_EyePosition);
    light.Position = LightPosition;
    XMVECTOR LightDirection = XMVectorSet(-LightPosition.x, -LightPosition.y, -LightPosition.z, 0.0f);
    LightDirection = XMVector3Normalize(LightDirection);
    XMStoreFloat4(&light.Direction, LightDirection);

    LightPropertiesConstantBuffer lightProperties;
    lightProperties.EyePosition = LightPosition;
    lightProperties.Lights[0] = light;
    g_pImmediateContext->UpdateSubresource(g_pLightConstantBuffer, 0, nullptr, &lightProperties, 0, 0);
}

float calculateDeltaTime()
{
    // Update our time
    static float deltaTime = 0.0f;
    static ULONGLONG timeStart = 0;
    ULONGLONG timeCur = GetTickCount64();
    if (timeStart == 0)
        timeStart = timeCur;
    deltaTime = (timeCur - timeStart) / 1000.0f;
    timeStart = timeCur;

    float FPS60 = 1.0f / 60.0f;
    static float cummulativeTime = 0;

    // cap the framerate at 60 fps 
    cummulativeTime += deltaTime;
    if (cummulativeTime >= FPS60) {
        cummulativeTime = cummulativeTime - FPS60;
    }
    else {
        return 0;
    }

    return deltaTime;
}

static float dTime = 0.0f;

void Application::Update()
{
    

    float t = calculateDeltaTime(); // capped at 60 fps
    if (t == 0.0f)
        return;

    if (GetAsyncKeyState(0x20) & 1) // Space
    {
        if (currentView == "Object")
            currentView = "Camera";
        else if (currentView == "Camera")
            currentView = "Object";
    }

    if (GetAsyncKeyState(0x77) & 1) // F8
    {
        if (tessellationToggled == false)
            tessellationToggled = true;
        else if (tessellationToggled == true)
            tessellationToggled = false;
    }

    if (GetAsyncKeyState(0x78) & 1) // F9
    {
        if (rsState == solidFill)
            rsState = wireFrame;
        else if (rsState == wireFrame)
            rsState = solidFill;
    }
    if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
    {
        grid->ResetSeed();

    }

    GetCursorPos(&currentMouseMov);

    ScreenToClient(g_hWnd, &currentMouseMov);

    if (currentView == "Camera")
    {
        if (GetAsyncKeyState(0x57)) //W
        {
            camera->Move(5000.0f * t, Forward);
            //_View = XMLoadFloat4x4(&camera->GetView());
        }
        else if (GetAsyncKeyState(0x53))//S
        {
            camera->Move(5000.0f * t, Backward);
            //_View = XMLoadFloat4x4(&camera->GetView());
        }
        if (GetAsyncKeyState(0x41))//D
        {
            camera->Move(5000.0f * t, Right);
        }
        else if (GetAsyncKeyState(0x44))//A
        {
            camera->Move(5000.0f * t, Left);
        }

        /*if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
        {*/
        if (currentMouseMov.x != lastMouseMov.x || currentMouseMov.y != lastMouseMov.y)
        {
            moveAmountX += lastMouseMov.y;
            moveAmountY += currentMouseMov.x;
            camera->Rotate((moveAmountX * 0.01), (moveAmountY * 0.01));
            moveAmountX = 0.0f;
            moveAmountY = 0.0f;
            lastMouseMov = currentMouseMov;
            //SetCursorPos(320, 240);

        }
        //}

        g_View = XMLoadFloat4x4(&camera->camera._view);
    }

    grid->Update(t);
}

//--------------------------------------------------------------------------------------
// Render a frame
//--------------------------------------------------------------------------------------
void Application::Render()
{
    

    // Clear the back buffer
    g_pImmediateContext->ClearRenderTargetView( g_pRenderTargetView, Colors::MidnightBlue );

    // Clear the depth buffer to 1.0 (max depth)
    g_pImmediateContext->ClearDepthStencilView( g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );

	// Update the cube transform, material etc. 
	//g_GameObject.update(t, g_pImmediateContext);
    //skeleton->Update();

    // get the game object world transform
	//XMMATRIX mGO = XMLoadFloat4x4(g_GameObject.getTransform());
	XMMATRIX mGO = XMLoadFloat4x4(g_GameObject->getTransform());

    // store this and the view / projection in a constant buffer for the vertex shader to use
    ConstantBuffer cb1;
	cb1.mWorld = XMMatrixTranspose( mGO);
	cb1.mView = XMMatrixTranspose( g_View );
	cb1.mProjection = XMMatrixTranspose( g_Projection );
	cb1.vOutputColor = XMFLOAT4(0, 0, 0, 0);
	g_pImmediateContext->UpdateSubresource( g_pConstantBuffer, 0, nullptr, &cb1, 0, 0 );

    
    SetupLightForRender();

    // Render the cube
    if (tessellationToggled == false)
    {
        g_pImmediateContext->VSSetShader(g_pVertexShader, nullptr, 0);
        g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
        grid->ToggleTessellation(false, g_pImmediateContext);
    }
    else if (tessellationToggled == true)
    {
        g_pImmediateContext->VSSetShader(g_pVertexShaderTes, nullptr, 0);
        g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);

        g_pImmediateContext->HSSetShader(g_pHullShader, nullptr, 0);
        g_pImmediateContext->DSSetShader(g_pDomainShader, nullptr, 0);
        g_pImmediateContext->DSSetConstantBuffers(0, 1, &g_pConstantBuffer);

        grid->ToggleTessellation(true, g_pImmediateContext);
    }

	/*g_pImmediateContext->VSSetShader( g_pVertexShader, nullptr, 0 );
	g_pImmediateContext->VSSetConstantBuffers( 0, 1, &g_pConstantBuffer );

    g_pImmediateContext->HSSetShader(g_pHullShader, nullptr, 0);
    g_pImmediateContext->DSSetShader(g_pDomainShader, nullptr, 0);
    g_pImmediateContext->DSSetConstantBuffers(0, 1, &g_pConstantBuffer);*/

    g_pImmediateContext->PSSetShader( g_pPixelShader, nullptr, 0 );
	g_pImmediateContext->PSSetConstantBuffers(2, 1, &g_pLightConstantBuffer);

    //ID3D11Buffer* materialCB = g_GameObject->getMaterialConstantBuffer();
    ID3D11Buffer* materialCB = grid->getMaterialConstantBuffer();
    g_pImmediateContext->PSSetConstantBuffers(1, 1, &materialCB);

    g_pImmediateContext->RSSetState(rsState);

    
   // g_GameObject->draw(g_pImmediateContext);

    mGO = XMLoadFloat4x4(grid->GetTransform());
    cb1.mWorld = XMMatrixTranspose(mGO);
    g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, nullptr, &cb1, 0, 0);

    grid->Draw(g_pImmediateContext);
    //skeleton->Draw(g_pImmediateContext, cb1, g_pConstantBuffer);

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::Text("Current View: (%.5f)(%.5f)(%.5f)", camera->GetPos().x, camera->GetPos().y, camera->GetPos().z);
    ImGui::Text("camera at: (%.5f)(%.5f)(%.5f)", camera->GetAt().x, camera->GetAt().y, camera->GetAt().z);
    ImGui::Text("camera up: (%.5f)(%.5f)(%.5f)", camera->GetUp().x, camera->GetUp().y, camera->GetUp().z);

    ImGui::Text("Movement Control Type: %s", currentView.c_str());
    ImGui::Checkbox("Tessellation", &tessellationToggled);

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    // Present our back buffer to our front buffer
    g_pSwapChain->Present( 0, 0 );
}



