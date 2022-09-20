#pragma once
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxcolors.h>
#include <DirectXCollision.h>
#include <string>
#include <vector>
#include "..\..\structures.h"
#include "..\OBJLoader\OBJLoader.h"

using namespace DirectX;

class SceneNode
{
public:
	SceneNode();

	virtual ~SceneNode();

	virtual void Update();
	 
	void AddChild(SceneNode* node);

	XMMATRIX GetPosition() { return position; };
	void SetPosition(XMFLOAT3 setPosition);

	XMMATRIX GetScale() { return scale; };
	void SetScale(XMFLOAT3 setScale);

	XMMATRIX GetRotation() { return orientation; };
	void SetRotation(float roll, float pitch, float yaw);

	XMMATRIX GetLocalTransform() { return localTransform; };
	XMMATRIX GetWorldTransform() { return worldTransform; };

	XMFLOAT4X4 GetWorldMatrix() { return worldMat; };

	void SetTransform();

	void SetTransformTest(XMMATRIX transform) { localTransform = transform; };

	MeshData GetMesh() { return mesh; };
	void SetMesh(MeshData meshData) { mesh = meshData; };

	string GetName() { return name; };
	void SetName(string boneName) { name = boneName; };

	void DrawNode(ID3D11DeviceContext* pContext, ConstantBuffer& constantBuffer, ID3D11Buffer* constantBuffer1);
	
	SceneNode* parentNode = nullptr;
	std::vector<SceneNode*> childPtrs;

private:
	XMMATRIX position = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	XMMATRIX scale = XMMatrixScaling(0.0f, 0.0f, 0.0f);
	XMMATRIX orientation = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);

	MeshData mesh;
	string name;

	XMFLOAT4X4 worldMat;

	XMMATRIX localTransform;
	XMMATRIX worldTransform;
};

 