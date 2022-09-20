#pragma once
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxcolors.h>
#include <DirectXCollision.h>
#include <vector>

using namespace DirectX;

class Bone
{
public:
	Bone();
	~Bone();
private:
	std::vector<Bone*> childPtrs;
	XMFLOAT4X4 tranform;
};

