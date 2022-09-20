#pragma once
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxcolors.h>
#include <DirectXCollision.h>
#include "..\DDS Texture Loader\DDSTextureLoader.h"
#include "..\..\Resource.h"
#include <iostream>
#include "..\..\structures.h"
#include <vector>
#include <fstream>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <algorithm>
#include <iostream>
#include <cmath>

using namespace DirectX;

class Terrain
{
public:
	struct Vertex
	{
		XMFLOAT3 Position;
		XMFLOAT3 Normal;
		//XMFLOAT3 Tangent;
		XMFLOAT2 TexCoord;
	};

	struct TerrainData
	{
		vector<Vertex> vertices;
		vector<unsigned long> indices;
	};

	struct HeightMapData
	{

	};

	Terrain(float width, float height, UINT rows, UINT cols, float smoothingFactor, ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext, int seed = time(0));
	~Terrain();

	void Init();
	HRESULT DefineGrid(float width, float depth, UINT rows, UINT cols, ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext);
	void Draw(ID3D11DeviceContext* pContext);

	void ToggleTessellation(bool toggled, ID3D11DeviceContext* pContext);

	void LoadHeightMap(string heightMapFilename);

	void TerrainSmoothing(float k, UINT rows, UINT cols);

	XMFLOAT4X4* GetTransform() { return &m_World; };
	ID3D11Buffer* getMaterialConstantBuffer() { return m_pMaterialConstantBuffer; }

	int Rnd(int min = 0, int max = 255);
	void InitCorners();
	void DiamondStep(int sideLength);
	void SquareStep(int sideLength);
	void Average(int x, int y, int sideLength);
	void DiamondSquare();
	void FaultLine();
	void DSLoadHeightMap();

	void Clamp(int* val, int min, int max);
	void Clamp_map();

	void ResetSeed();

	TerrainData grid;

	vector<float> mHeightMap;

	int map[513][513];
	
	int heightmapHeight = 513;
	int heightmapWidth = 513;
	float heightScale = 1.0f;
private:
	XMFLOAT4X4 m_World;
	XMMATRIX m_Matrix;

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
	XMFLOAT4 m_position;

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
};

