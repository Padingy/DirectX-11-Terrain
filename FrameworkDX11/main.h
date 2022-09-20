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
