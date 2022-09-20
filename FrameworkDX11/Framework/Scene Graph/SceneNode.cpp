#include "SceneNode.h"

SceneNode::SceneNode()
{
	SetTransform();
}

SceneNode::~SceneNode()
{
}

void SceneNode::Update()
{
	if (parentNode != nullptr)
	{
		worldTransform = parentNode->worldTransform * localTransform;

		XMStoreFloat4x4(&worldMat, worldTransform);
	}
	else
	{

		worldTransform = localTransform;
		XMStoreFloat4x4(&worldMat, worldTransform);

	}

	for (std::vector<SceneNode*>::iterator it = childPtrs.begin(); it != childPtrs.end(); it++)
	{
		(*it)->Update();
	}
}

void SceneNode::AddChild(SceneNode* node)
{
	childPtrs.push_back(node);
	node->parentNode = this;
}

void SceneNode::SetPosition(XMFLOAT3 setPosition)
{
	position = XMMatrixTranslation(setPosition.x, setPosition.y, setPosition.z);
} 

void SceneNode::SetScale(XMFLOAT3 setScale)
{
	scale = XMMatrixScaling(setScale.x, setScale.y, setScale.z);
}

void SceneNode::SetRotation(float roll, float pitch, float yaw)
{
	orientation = XMMatrixRotationRollPitchYaw(roll, pitch, yaw);
}

void SceneNode::SetTransform()
{
	localTransform = scale * orientation * position;
	worldTransform = localTransform;

	XMStoreFloat4x4(&worldMat, worldTransform);
}

void SceneNode::DrawNode(ID3D11DeviceContext* pContext, ConstantBuffer& constantBuffer, ID3D11Buffer* constantBuffer1)
{
	constantBuffer.mWorld = XMMatrixTranspose(XMLoadFloat4x4(&worldMat));
	pContext->UpdateSubresource(constantBuffer1, 0, nullptr, &constantBuffer, 0, 0);

	pContext->IASetVertexBuffers(0, 1, &mesh.VertexBuffer, &mesh.VBStride, &mesh.VBOffset);
	//pContext->UpdateSubresource(m_pMaterialConstantBuffer, 0, nullptr, &m_material, 0, 0);
	pContext->IASetIndexBuffer(mesh.IndexBuffer, DXGI_FORMAT_R16_UINT, 0);


	//pContext->RSSetState(wireFrameRS);
	pContext->DrawIndexed(mesh.IndexCount, 0, 0);

	/**********************************************
	MARKING SCHEME: TRAVERSING SKELETON STRUCTURE
	Description: Traversing Skeleton Structure Scene Graph using meshs and world matrix for demonstration
	***********************************************/

	for (std::vector<SceneNode*>::iterator it = childPtrs.begin(); it != childPtrs.end(); it++)
	{
		(*it)->DrawNode(pContext, constantBuffer, constantBuffer1);
	}
}

