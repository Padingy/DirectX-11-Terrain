#include "Skeleton.h"

Skeleton::Skeleton(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext)
{
	/**********************************************
	MARKING SCHEME: BASIC LOADING OF THE BONE SKELETON STRUCTURE
	Description: Creating basic skeleton structure with bones hard coded with bones being set into a parent > child system
	***********************************************/

	MeshData mesh = OBJLoader::Load("Objects/Boat.obj", pd3dDevice);
	MeshData mesh1 = OBJLoader::Load("Objects/Rock.obj", pd3dDevice);

	root = new SceneNode();
	root->SetMesh(mesh);
	root->SetScale(XMFLOAT3(0.001f, 0.001f, 0.001f));
	root->SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
	root->SetRotation(0.0f, 0.0f, 0.0f);
	root->SetName("root");
	root->SetTransform();
	bones.push_back(root);

	SceneNode* head = new SceneNode();
	head->SetMesh(mesh);
	head->SetScale(XMFLOAT3(0.001f, 0.001f, 0.001f));
	head->SetPosition(XMFLOAT3(0.0f, 0.1f, 0.0f));
	head->SetName("head");
	head->SetTransform();
	root->AddChild(head);
	bones.push_back(head);

	SceneNode* leftArm = new SceneNode();
	leftArm->SetMesh(mesh);
	leftArm->SetScale(XMFLOAT3(0.001f, 0.001f, 0.001f));
	leftArm->SetPosition(XMFLOAT3(0.1f, 0.0f, 0.0f));
	leftArm->SetName("leftArm");
	leftArm->SetTransform();
	root->AddChild(leftArm);
	bones.push_back(leftArm);

	SceneNode* leftArm1 = new SceneNode();
	leftArm1->SetMesh(mesh);
	leftArm1->SetScale(XMFLOAT3(0.001f, 0.001f, 0.001f));
	leftArm1->SetPosition(XMFLOAT3(0.2f, 0.0f, 0.0f));
	leftArm1->SetName("leftArm1");
	leftArm1->SetTransform();
	root->AddChild(leftArm1);
	bones.push_back(leftArm1);

	SceneNode* rightArm = new SceneNode();
	rightArm->SetMesh(mesh);
	rightArm->SetScale(XMFLOAT3(0.001f, 0.001f, 0.001f));
	rightArm->SetPosition(XMFLOAT3(-0.1f, 0.0f, 0.0f));
	rightArm->SetName("rightArm");
	rightArm->SetTransform();
	root->AddChild(rightArm);
	bones.push_back(rightArm);

	SceneNode* rightArm2 = new SceneNode();
	rightArm2->SetMesh(mesh);
	rightArm2->SetScale(XMFLOAT3(0.001f, 0.001f, 0.001f));
	rightArm2->SetPosition(XMFLOAT3(-0.2f, 0.0f, 0.0f));
	rightArm2->SetName("rightArm2");
	rightArm2->SetTransform();
	root->AddChild(rightArm2);
	bones.push_back(rightArm2);

	leftLeg1 = new SceneNode();
	leftLeg1->SetMesh(mesh);
	leftLeg1->SetScale(XMFLOAT3(0.001f, 0.001f, 0.001f));
	leftLeg1->SetPosition(XMFLOAT3(0.1f, -0.1f, 0.0f));
	leftLeg1->SetName("leftLeg1");
	leftLeg1->SetTransform();
	root->AddChild(leftLeg1); 
	bones.push_back(leftLeg1);

	SceneNode* leftLeg2 = new SceneNode();
	leftLeg2->SetMesh(mesh);
	leftLeg2->SetScale(XMFLOAT3(0.001f, 0.001f, 0.001f));
	leftLeg2->SetPosition(XMFLOAT3(0.1f, -0.2f, 0.0f));
	leftLeg2->SetName("leftLeg2");
	leftLeg2->SetTransform();
	leftLeg1->AddChild(leftLeg2);
	bones.push_back(leftLeg2);
	
	rightLeg1 = new SceneNode();
	rightLeg1->SetMesh(mesh);
	rightLeg1->SetScale(XMFLOAT3(0.001f, 0.001f, 0.001f));
	rightLeg1->SetPosition(XMFLOAT3(-0.1f, -0.1f, 0.0f));
	rightLeg1->SetName("rightLeg1");
	rightLeg1->SetTransform();
	root->AddChild(rightLeg1);
	bones.push_back(rightLeg1);

	SceneNode* rightLeg2 = new SceneNode();
	rightLeg2->SetMesh(mesh);
	rightLeg2->SetScale(XMFLOAT3(0.001f, 0.001f, 0.001f));
	rightLeg2->SetPosition(XMFLOAT3(-0.1f, -0.2f, 0.0f));
	rightLeg2->SetName("rightLeg2");
	rightLeg2->SetTransform();
	rightLeg1->AddChild(rightLeg2);
	bones.push_back(rightLeg2);
}

Skeleton::~Skeleton()
{
}

void Skeleton::CreateSkeleton()
{
}

void Skeleton::Update()
{
	

	//leftLeg1->SetRotation(45, 45, 45);

	//leftLeg1->SetTransformTest(XMMatrixTranslation(0.1f, -0.2f, 0.0f));
	//rightLeg1->SetTransformTest(XMMatrixTranslation(-0.1f, -0.2f, 0.0f));

	//leftLeg1->SetTransform();

	//root->Update();
}

void Skeleton::Draw(ID3D11DeviceContext* pContext, ConstantBuffer& constantBuffer, ID3D11Buffer* constantBuffer1)
{
	/**********************************************
	MARKING SCHEME: TRAVERSING SKELETON STRUCTURE
	Description: Traversing Skeleton Structure Scene Graph using meshs and world matrix for demonstration
	***********************************************/

	root->DrawNode(pContext, constantBuffer, constantBuffer1);
}
