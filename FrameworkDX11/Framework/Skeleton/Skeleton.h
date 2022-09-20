#pragma once
#include "../Scene Graph/SceneNode.h"
#include "../OBJLoader/OBJLoader.h"

class Skeleton : public SceneNode
{
public:
	Skeleton(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext);
	~Skeleton();

	void CreateSkeleton();

	void Update();
	void Draw(ID3D11DeviceContext* pContext, ConstantBuffer& constantBuffer, ID3D11Buffer* constantBuffer1);
	SceneNode* root;
	std::vector<SceneNode*> bones;
private:
	int numOfBones = 4;

	SceneNode* leftLeg1;
	SceneNode* rightLeg1;
protected:

};