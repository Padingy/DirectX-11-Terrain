#pragma once

#include <windows.h>
#include <windowsx.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include <DirectXCollision.h>
#include "Framework/DDS Texture Loader/DDSTextureLoader.h"
#include "resource.h"
#include <iostream>
#include <string>

#include "Framework/Game Objects/Drawable Game Object/DrawableGameObject.h"
#include "structures.h"
#include "Framework/Terrain/Terrain.h"
#include "Framework/Camera/Camera.h"

#include "Framework/Skeleton/Skeleton.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"

#include <algorithm>
#include <vector>

using namespace std;

typedef vector<DrawableGameObject*> vecDrawables;

class Application
{
public:
	HRESULT		InitWindow(HINSTANCE hInstance, int nCmdShow);
	HRESULT		InitDevice();
	HRESULT		InitMesh();
	HRESULT		InitWorld(int width, int height);
	void		CleanupDevice();
	void Update();
	void		Render();
	void SetupLightForRender();

private:
	HINSTANCE               g_hInst = nullptr;
	HWND                    g_hWnd = nullptr;
	D3D_DRIVER_TYPE         g_driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL       g_featureLevel = D3D_FEATURE_LEVEL_11_0;
	ID3D11Device* g_pd3dDevice = nullptr;
	ID3D11Device1* g_pd3dDevice1 = nullptr;
	ID3D11DeviceContext* g_pImmediateContext = nullptr;
	ID3D11DeviceContext1* g_pImmediateContext1 = nullptr;
	IDXGISwapChain* g_pSwapChain = nullptr;
	IDXGISwapChain1* g_pSwapChain1 = nullptr;
	ID3D11RenderTargetView* g_pRenderTargetView = nullptr;
	ID3D11Texture2D* g_pDepthStencil = nullptr;
	ID3D11DepthStencilView* g_pDepthStencilView = nullptr;
	ID3D11VertexShader* g_pVertexShader = nullptr;
	ID3D11VertexShader* g_pVertexShaderTes = nullptr;

	ID3D11ShaderResourceView* _pTextureRV = nullptr;

	ID3D11RasterizerState* wireFrame = nullptr;
	ID3D11RasterizerState* solidFill = nullptr;
	ID3D11RasterizerState* rsState = nullptr;

	ID3D11PixelShader* g_pPixelShader = nullptr;

	ID3D11HullShader* g_pHullShader = nullptr;
	ID3D11DomainShader* g_pDomainShader = nullptr;

	ID3D11InputLayout* g_pVertexLayout = nullptr;
	ID3D11InputLayout* g_pVertexLayoutTes = nullptr;

	ID3D11Buffer* g_pConstantBuffer = nullptr;

	ID3D11Buffer* g_pLightConstantBuffer = nullptr;

	XMMATRIX                g_View;
	XMMATRIX                g_Projection;

	int						g_viewWidth;
	int						g_viewHeight;

	string currentView = "Camera";

	bool tessellationToggled = true;

	Camera* camera = nullptr;
	POINT currentMouseMov;
	POINT lastMouseMov;
	float moveAmountX;
	float moveAmountY;

	DrawableGameObject* g_GameObject;

	Terrain* faultLineGrid = nullptr;
	Terrain* diamondSquareGrid = nullptr;

	Skeleton* skeleton;
};
