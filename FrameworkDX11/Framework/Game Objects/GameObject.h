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
	struct Vertex
	{
		XMFLOAT3 Position;
		XMFLOAT3 Normal;
		//XMFLOAT3 Tangent;
		XMFLOAT2 TexCoord;
	};

	GameObject();
	~GameObject();

	virtual void Release();
	
	XMFLOAT3 m_position;

	virtual void Update(float t);
	virtual void Draw(ID3D11DeviceContext* pContext, XMFLOAT4X4* viewMatrix, XMFLOAT4X4* projMatrix);


	void SetPosition(XMFLOAT3 newPosition) { m_position = newPosition; };
	XMFLOAT3 GetPosition() { return m_position; };


protected:
	


};

