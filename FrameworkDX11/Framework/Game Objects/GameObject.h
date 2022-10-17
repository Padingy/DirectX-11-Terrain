#pragma once
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxcolors.h>
#include <DirectXCollision.h>
#include "..\..\structures.h"

using namespace DirectX;

class GameObject
{
public:
	GameObject();
	~GameObject();

	virtual void Update(float t);
	virtual void Draw(ID3D11DeviceContext* pContext);

private:

};

