#include "GameObject.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
	Release();
}

void GameObject::Release()
{
}

void GameObject::Update(float t)
{

}

void GameObject::Draw(ID3D11DeviceContext* pContext, XMFLOAT4X4* viewMatrix, XMFLOAT4X4* projMatrix)
{

}

