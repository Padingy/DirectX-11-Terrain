#pragma once
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxcolors.h>
#include <DirectXCollision.h>
#include "..\DDS Texture Loader\DDSTextureLoader.h"
#include "..\..\Resource.h"
#include <iostream>
#include "..\..\structures.h"
#include "..\Dependencies\NlohmannJson\json.hpp"
#include "..\Game Objects\GameObject.h"
#include <vector>
#include <fstream>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <algorithm>
#include <iostream>
#include <cmath>

using namespace DirectX;
using json = nlohmann::json;

class Terrain : public GameObject
{
public:
	//struct Vertex
	//{
	//	XMFLOAT3 Position;
	//	XMFLOAT3 Normal;
	//	//XMFLOAT3 Tangent;
	//	XMFLOAT2 TexCoord;
	//};

	struct TerrainData
	{
		vector<Vertex> vertices;
		vector<unsigned long> indices;
	};

	struct TerrainTextureHeights
	{
		float textureHeight0;
		float textureHeight1;
		float textureHeight2;
		float textureHeight3;
		float textureHeight4;
		float padding[3];
	};

	Terrain(float width, float height, UINT rows, UINT cols, float smoothingFactor, ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext, int seed = time(0));
	Terrain(char* _filepath);
	~Terrain();

	virtual void Release() override;

	void Init();
	void DefineGrid(float width, float depth, UINT rows, UINT cols);
	HRESULT InitMesh(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext);
	virtual void Update(float t) override;
	virtual void Draw(ID3D11DeviceContext* pContext, XMFLOAT4X4* viewMatrix, XMFLOAT4X4* projMatrix) override;

	void ToggleTessellation(bool toggled, ID3D11DeviceContext* pContext);

	void LoadHeightMap(string heightMapFilename);

	void TerrainSmoothing(float k, UINT rows, UINT cols);

	/*void SetPosition(XMFLOAT3 newPosition) { m_position = newPosition; };
	XMFLOAT3 GetPosition() { return m_position; };*/

	void SetHeights();
	void SetTextureHeights();

	XMFLOAT4X4* GetTransform() { return &m_World; };
	ID3D11Buffer* getMaterialConstantBuffer() { return m_pMaterialConstantBuffer; }
	

	int Rnd(int min = 0, int max = 255);
	void InitCorners();
	void DiamondStep(int sideLength);
	void SquareStep(int sideLength);
	void Average(int x, int y, int sideLength);
	void DiamondSquare();
	void DiamondSquare1(int tileScale);
	vector<float> GetHeightMap() { return mHeightMap; };
	void FaultLine(int iterations, int displacement, int tileScale);
	int ConvertTo1D(int x, int y);
	void LoadFromJSON(char* _filePath);

	void Clamp(int* val, int min, int max);
	void Clamp(float* val, int min, int max);
	void Clamp_map();

	void ResetSeed();

private:
	XMFLOAT4X4 m_World;

	ID3D11Buffer* m_pConstantBuffer = nullptr;
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;
	ID3D11ShaderResourceView* m_pTextureResourceViewWater;
	ID3D11ShaderResourceView* m_pTextureResourceViewSand;
	ID3D11ShaderResourceView* m_pTextureResourceViewGrass;
	ID3D11ShaderResourceView* m_pTextureResourceViewDarkDirt;
	ID3D11ShaderResourceView* m_pTextureResourceViewLightDirt;
	ID3D11ShaderResourceView* m_pTextureResourceViewSnow;
	ID3D11SamplerState* m_pSamplerLinear;
	MaterialPropertiesConstantBuffer	m_material;
	ID3D11Buffer* m_pMaterialConstantBuffer = nullptr;
	ID3D11Buffer* m_pTerrainTextureHeights = nullptr;
	/*XMFLOAT3 m_position;*/

	UINT stride;
	UINT offset;
	ID3D11RasterizerState* wireFrameRS;

	int roughness = 196;

	int size = 17;
	
	float height;
	float width;
	UINT cols;
	UINT rows;
	int seed;

	TerrainData grid;
	TerrainTextureHeights terrainTextureHeights;

	vector<float> mHeightMap;
	int heightmapHeight = 513;
	int heightmapWidth = 513;
	float heightScale = 2.0f;

	int terrainSize = 512;
	int map[513][513];
};

