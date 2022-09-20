#pragma once
using namespace std;
using namespace DirectX;

//--------------------------------------------------------------------------------------
// Structures
//--------------------------------------------------------------------------------------


struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
	XMFLOAT4 vOutputColor;
};

struct _Material
{
	_Material()
		: Emissive(0.0f, 0.0f, 0.0f, 1.0f)
		, Ambient(0.1f, 0.1f, 0.1f, 1.0f)
		, Diffuse(1.0f, 1.0f, 1.0f, 1.0f)
		, Specular(1.0f, 1.0f, 1.0f, 1.0f)
		, SpecularPower(128.0f)
		, UseTexture(false)
	{}

	DirectX::XMFLOAT4   Emissive;
	//----------------------------------- (16 byte boundary)
	DirectX::XMFLOAT4   Ambient;
	//----------------------------------- (16 byte boundary)
	DirectX::XMFLOAT4   Diffuse;
	//----------------------------------- (16 byte boundary)
	DirectX::XMFLOAT4   Specular;
	//----------------------------------- (16 byte boundary)
	float               SpecularPower;
	// Add some padding complete the 16 byte boundary.
	int                 UseTexture;
	// Add some padding to complete the 16 byte boundary.
	float               Padding[2];
	//----------------------------------- (16 byte boundary)
}; // Total:                                80 bytes (5 * 16)

struct MaterialPropertiesConstantBuffer
{
	_Material   Material;
};

enum LightType
{
	DirectionalLight = 0,
	PointLight = 1,
	SpotLight = 2
};

#define MAX_LIGHTS 1

struct Light
{
	Light()
		: Position(0.0f, 0.0f, 0.0f, 1.0f)
		, Direction(0.0f, 0.0f, 1.0f, 0.0f)
		, Color(1.0f, 1.0f, 1.0f, 1.0f)
		, SpotAngle(DirectX::XM_PIDIV2)
		, ConstantAttenuation(1.0f)
		, LinearAttenuation(0.0f)
		, QuadraticAttenuation(0.0f)
		, LightType(DirectionalLight)
		, Enabled(0)
	{}

	DirectX::XMFLOAT4    Position;
	//----------------------------------- (16 byte boundary)
	DirectX::XMFLOAT4    Direction;
	//----------------------------------- (16 byte boundary)
	DirectX::XMFLOAT4    Color;
	//----------------------------------- (16 byte boundary)
	float       SpotAngle;
	float       ConstantAttenuation;
	float       LinearAttenuation;
	float       QuadraticAttenuation;
	//----------------------------------- (16 byte boundary)
	int         LightType;
	int         Enabled;
	// Add some padding to make this struct size a multiple of 16 bytes.
	int         Padding[2];
	//----------------------------------- (16 byte boundary)
};  // Total:                              80 bytes ( 5 * 16 )


struct LightPropertiesConstantBuffer
{
	LightPropertiesConstantBuffer()
		: EyePosition(0, 0, 0, 1)
		, GlobalAmbient(0.2f, 0.2f, 0.8f, 1.0f)
	{}

	DirectX::XMFLOAT4   EyePosition;
	//----------------------------------- (16 byte boundary)
	DirectX::XMFLOAT4   GlobalAmbient;
	//----------------------------------- (16 byte boundary)
	Light               Lights[MAX_LIGHTS]; // 80 * 8 bytes
};  // Total:                 672 bytes (42 * 16)

struct CameraS
{
	XMMATRIX camRotationMatrix;
	XMVECTOR DefaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR DefaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR camForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR camRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	XMFLOAT4 _eye;
	XMVECTOR _eyeVector;
	XMFLOAT4 _at;
	XMVECTOR _atVector;
	XMFLOAT4 _up;
	XMVECTOR _upVector;

	float movementSpeed;
	int type;

	UINT _windowWidth;
	UINT _windowHeight;
	FLOAT _nearDepth;
	FLOAT _farDepth;

	float rotationX;
	float rotationY;

	XMFLOAT4X4 _view;
	XMFLOAT4X4 _projection;
};

enum CameraType
{
	LookAt = 0,
	LookTo = 1,
};

enum Direction
{
	Forward = 0,
	Backward = 1,
	Right = 2,
	Left = 3,
};

struct MeshData
{
	ID3D11Buffer* VertexBuffer;
	ID3D11Buffer* IndexBuffer;
	UINT VBStride;
	UINT VBOffset;
	UINT IndexCount;
	float scale;
};

struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
	XMFLOAT2 TexC;

	bool operator<(const SimpleVertex other) const
	{
		return memcmp((void*)this, (void*)&other, sizeof(SimpleVertex)) > 0;
	};
};