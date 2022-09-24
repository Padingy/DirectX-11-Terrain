#include "Terrain.h"


Terrain::Terrain(float width, float height, UINT rows, UINT cols, float smoothingFactor, ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext, int seed)
{
	m_position = XMFLOAT4(00.0f, -0.2f, 0.0f, 0.0f);

	XMStoreFloat4x4(&m_World, XMMatrixTranslation(m_position.x, m_position.y, m_position.z));
	m_Matrix = XMLoadFloat4x4(&m_World);
	XMStoreFloat4x4(&m_World, XMMatrixIdentity());

	/**********************************************
	MARKING SCHEME: LOADING TERRAIN WITH HEIGHT MAP
	Description: Using a imagine height map to create the terrain
	***********************************************/

	//LoadHeightMap("Resources/Terrain Textures/coastMountain513.raw");

	//TerrainSmoothing(smoothingFactor, rows, cols);

	//DefineGrid(width, height, rows + 1, cols + 1, pd3dDevice, pContext);
	

	/**********************************************
	MARKING SCHEME: LOADING TERRAIN WITH DIAMOND SQUARE
	Description: Using Diamond Square to create the terrain heights
	***********************************************/

	size = rows + 1;
	srand(time(0));

	InitCorners();
	DiamondSquare();

	DefineGrid(width, height, rows + 1, cols + 1);
	InitMesh(pd3dDevice, pContext);

}

Terrain::~Terrain()
{
}

void Terrain::Init()
{
}

void Terrain::DefineGrid(float width, float depth, UINT rows, UINT cols)
{
	int vertexCount = rows * cols;
	UINT triCount = (rows - 1) * (cols - 1) * 2;

	float halfWidth = width * 0.5f;
	float halfDepth = depth * 0.5f;

	float dx = width / (cols - 1);
	float dz = depth / (rows - 1);

	float du = 1.0f / (cols - 1);
	float dv = 1.0f / (rows - 1);

	float x;
	float z;

	grid.vertices.clear();
	grid.indices.clear();

	UINT k = 0;
	Vertex v;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			//v.Position.x = j * dx + (-width * 0.5);
			//v.Position.y = (float)mHeightMap[k];
			//v.Position.z = -(i * dz) + (depth * 0.5);

			//v.Position.y = map[i][j] / 255;
			//v.TexCoord.x = j * du;
			//v.TexCoord.y = i * dv;

			v.Position.x = 0 + (j * width);
			v.Position.y = (float)mHeightMap[k];
			v.Position.z = 0 + (i * depth);
			
			v.TexCoord.x = j / cols;
			v.TexCoord.y = i / rows;

			k++;
			grid.vertices.push_back(v);
		}
	}

	for (int i = 0; i < rows - 1; i++)
	{
		for (int j = 0; j < cols - 1; j++)
		{
			grid.indices.push_back(i * cols + j);

			grid.indices.push_back(i * cols + (j + 1));

			grid.indices.push_back((i + 1) * cols + j);

			grid.indices.push_back((i + 1) * cols + j);

			grid.indices.push_back(i * cols + (j + 1));

			grid.indices.push_back((i + 1) * cols + (j + 1));

		}
	}

	
}

HRESULT Terrain::InitMesh(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext)
{
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Vertex) * grid.vertices.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData = {};
	InitData.pSysMem = &grid.vertices[0];
	HRESULT hr = pd3dDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer);
	if (FAILED(hr))
		return hr;

	// Set vertex buffer
	stride = sizeof(Vertex);
	offset = 0;
	pContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(unsigned long) * grid.indices.size();        // 36 vertices needed for 12 triangles in a triangle list
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	InitData.pSysMem = &grid.indices[0];
	hr = pd3dDevice->CreateBuffer(&bd, &InitData, &m_pIndexBuffer);
	if (FAILED(hr))
		return hr;

	// Set index buffer
	pContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	hr = CreateDDSTextureFromFile(pd3dDevice, L"Resources\\Terrain Textures\\water.dds", nullptr, &m_pTextureResourceViewWater);
	if (FAILED(hr))
		return hr;

	hr = CreateDDSTextureFromFile(pd3dDevice, L"Resources\\Terrain Textures\\sand.dds", nullptr, &m_pTextureResourceViewSand);
	if (FAILED(hr))
		return hr;

	hr = CreateDDSTextureFromFile(pd3dDevice, L"Resources\\Terrain Textures\\grass.dds", nullptr, &m_pTextureResourceViewGrass);
	if (FAILED(hr))
		return hr;

	hr = CreateDDSTextureFromFile(pd3dDevice, L"Resources\\Terrain Textures\\lightdirt.dds", nullptr, &m_pTextureResourceViewLightDirt);
	if (FAILED(hr))
		return hr;

	hr = CreateDDSTextureFromFile(pd3dDevice, L"Resources\\Terrain Textures\\lightdirt.dds", nullptr, &m_pTextureResourceViewDarkDirt);
	if (FAILED(hr))
		return hr;

	hr = CreateDDSTextureFromFile(pd3dDevice, L"Resources\\Terrain Textures\\snow.dds", nullptr, &m_pTextureResourceViewSnow);
	if (FAILED(hr))
		return hr;


	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = pd3dDevice->CreateSamplerState(&sampDesc, &m_pSamplerLinear);

	D3D11_RASTERIZER_DESC wfd;
	ZeroMemory(&wfd, sizeof(D3D11_RASTERIZER_DESC));
	wfd.FillMode = D3D11_FILL_WIREFRAME;
	wfd.CullMode = D3D11_CULL_NONE;
	wfd.DepthClipEnable = true;
	pd3dDevice->CreateRasterizerState(&wfd, &wireFrameRS);

	m_material.Material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_material.Material.Specular = XMFLOAT4(1.0f, 0.2f, 0.2f, 1.0f);
	m_material.Material.SpecularPower = 32.0f;
	m_material.Material.UseTexture = true;

	// Create the material constant buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(MaterialPropertiesConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = pd3dDevice->CreateBuffer(&bd, nullptr, &m_pMaterialConstantBuffer);
	if (FAILED(hr))
		return hr;

	return hr;
}

void Terrain::Draw(ID3D11DeviceContext* pContext)
{
	pContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	pContext->UpdateSubresource(m_pMaterialConstantBuffer, 0, nullptr, &m_material, 0, 0);
	pContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	pContext->PSSetShaderResources(0, 1, &m_pTextureResourceViewWater);
	pContext->PSSetShaderResources(1, 1, &m_pTextureResourceViewSand);
	pContext->PSSetShaderResources(2, 1, &m_pTextureResourceViewGrass);
	pContext->PSSetShaderResources(3, 1, &m_pTextureResourceViewLightDirt);
	pContext->PSSetShaderResources(4, 1, &m_pTextureResourceViewDarkDirt);
	pContext->PSSetShaderResources(5, 1, &m_pTextureResourceViewSnow);
	pContext->PSSetSamplers(0, 1, &m_pSamplerLinear);

	pContext->DrawIndexed(grid.indices.size(), 0, 0);

}

void Terrain::ToggleTessellation(bool toggled, ID3D11DeviceContext* pContext)
{
	if (toggled == true)
		pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
	else
		pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

/**********************************************
MARKING SCHEME:	LOADING HEIGHTMAP THROUGH IMAGE FILE
Description: Loading an image file to load in a height map for Terrain generation
***********************************************/
void Terrain::LoadHeightMap(string heightMapFilename)
{
	std::vector<unsigned char> in(heightmapWidth * heightmapHeight);

	// Open the file.
	std::ifstream inFile;
	inFile.open(heightMapFilename.c_str(), std::ios_base::binary);

	if (inFile)
	{
		// Read the RAW bytes.
		inFile.read((char*)&in[0], (std::streamsize)in.size());
		// Done with file.
		inFile.close();
	}

	// Copy the array data into a float array and scale it. 
	mHeightMap.resize(heightmapWidth * heightmapHeight, 0);

	for (UINT i = 0; i < heightmapHeight * heightmapWidth; ++i)
	{
		mHeightMap[i] = (in[i] / 255.0f) * heightScale;
	}

}

void Terrain::TerrainSmoothing(float k, UINT rows, UINT cols)
{
	int i, j;

	for (i = 0; i < rows; i++)
		for (j = 1; j < cols; j++)
			mHeightMap[i * cols + j] =
			mHeightMap[i * cols + j] * (1 - k) +
			mHeightMap[i * cols + j - 1] * k;

	for (i = 1; i < rows; i++)
		for (j = 0; j < cols; j++)
			mHeightMap[i * cols + j] =
			mHeightMap[i * cols + j] * (1 - k) +
			mHeightMap[(i - 1) * cols + j] * k;

	for (i = 0; i < rows; i++)
		for (j = cols - 1; j > -1; j--)
			mHeightMap[i * cols + j] =
			mHeightMap[i * cols + j] * (1 - k) +
			mHeightMap[i * cols + j + 1] * k;

	for (i = rows - 2; i < -1; i--)
		for (j = 0; j < cols; j++)
			mHeightMap[i * cols + j] =
			mHeightMap[i * cols + j] * (1 - k) +
			mHeightMap[(i + 1) * cols + j] * k;
}

int Terrain::Rnd(int min, int max)
{
	return min + (rand() % static_cast<int>(max - min + 1));
}

void Terrain::InitCorners()
{

	map[0][0] = Rnd();
	map[0][size - 1] = Rnd();
	map[size - 1][0] = Rnd();
	map[size - 1][size - 1] = Rnd();
}

/**********************************************
MARKING SCHEME: PROCEDURAL TERRAIN GENERATION DIAMOND SQUARE
Description: Creating terrain heights through a multiple step process through multiple passes over
***********************************************/

void Terrain::DiamondSquare()
{
	int sideLength = size / 2;
	DiamondStep(size);
	SquareStep(size);

	roughness /= 0.3;

	while (sideLength >= 2)
	{
		DiamondStep(sideLength + 1);
		Clamp_map();
		SquareStep(sideLength + 1);
		Clamp_map();
		sideLength /= 2;
		roughness /= 2;
	}

	DSLoadHeightMap();
}

void Terrain::FaultLine()
{
}

/**********************************************
MARKING SCHEME: PROCEDURAL TERRAIN GENERATION DIAMOND SQUARE (Diamond Step)
Description: Creating terrain heights through a multiple step process through multiple passes over
***********************************************/

void Terrain::DiamondStep(int sideLength)
{
	int halfSide = sideLength / 2;

	for (int y = 0; y < size / (sideLength - 1); y++)
	{
		for (int x = 0; x < size / (sideLength - 1); x++)
		{
			int center_x = x * (sideLength - 1) + halfSide;
			int center_y = y * (sideLength - 1) + halfSide;

			int avg = (map[x * (sideLength - 1)][y * (sideLength - 1)] +
				map[x * (sideLength - 1)][(y + 1) * (sideLength - 1)] +
				map[(x + 1) * (sideLength - 1)][y * (sideLength - 1)] +
				map[(x + 1) * (sideLength - 1)][(y + 1) * (sideLength - 1)]) / 4.0f;

			map[center_x][center_y] = avg + Rnd(-roughness, roughness);
		}
	}

}

/**********************************************
MARKING SCHEME: PROCEDURAL TERRAIN GENERATION DIAMOND SQUARE (Square Step)
Description: Creating terrain heights through a multiple step process through multiple passes over
***********************************************/

void Terrain::SquareStep(int sideLength)
{
	int halfLength = sideLength / 2;

	for (int y = 0; y < size / (sideLength - 1); y++)
	{
		for (int x = 0; x < size / (sideLength - 1); x++)
		{
			// Top
			Average(x * (sideLength - 1) + halfLength, y * (sideLength - 1), sideLength);
			// Right
			Average((x + 1) * (sideLength - 1), y * (sideLength - 1) + halfLength, sideLength);
			// Bottom
			Average(x * (sideLength - 1) + halfLength, (y + 1) * (sideLength - 1), sideLength);
			// Left
			Average(x * (sideLength - 1), y * (sideLength - 1) + halfLength, sideLength);
		}
	}
}

void Terrain::Average(int x, int y, int sideLength)
{
	float counter = 0;
	float accumulator = 0;

	int halfSide = sideLength / 2;

	if (x != 0)
	{
		counter += 1.0f;
		accumulator += map[y][x - halfSide];
	}
	if (y != 0)
	{
		counter += 1.0f;
		accumulator += map[y - halfSide][x];
	}
	if (x != size - 1)
	{
		counter += 1.0f;
		accumulator += map[y][x + halfSide];
	}
	if (y != size - 1)
	{
		counter += 1.0f;
		accumulator += map[y + halfSide][x];
	}

	map[y][x] = (accumulator / counter) + Rnd(-roughness, roughness);

}

void Terrain::DSLoadHeightMap()
{
	for (int y = 0; y < size; y++)
	{
		for (int x = 0; x < size; x++)
		{

			float height = (float)map[y][x] / 255;
			mHeightMap.push_back(height);
		}
	}
}

void Terrain::Clamp(int* val, int min, int max)
{
	if (*val < min) *val = min;
	if (*val > max) *val = max;
}

void Terrain::Clamp_map()
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			Clamp(&map[i][j], 0, 255);
		}
	}
}

void Terrain::ResetSeed()
{
	srand(time(0));
}
