// Light shader.h
// Basic single light shader setup
#pragma once

#include "../DXFramework/BaseShader.h"
#include "../DXFramework/Light.h"
#include "../DXFramework/Camera.h"

using namespace std;
using namespace DirectX;

const int NUM_LIGHTS = 4;

class MultiLightShader : public BaseShader
{
private:
	struct CameraBufferType
	{
		XMFLOAT3 cameraPosition;
		float padding;
	};

	struct LightColorBufferType
	{
		XMFLOAT4 diffuseColor[NUM_LIGHTS];
	};

	struct LightPositionBufferType
	{
		XMFLOAT3 lightPosition[NUM_LIGHTS];
	};

public:
	// When resources are being created and interfaced with,
	// the 'device' interface is used.
	MultiLightShader(ID3D11Device* device, HWND hwnd);
	~MultiLightShader();

	// When the pipeline or a resource is being manipulated,
	// the 'device context' is used.
	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, 
		ID3D11ShaderResourceView* texture, XMFLOAT4 diffuseColor[],
		XMFLOAT3 lightPosition[]);
	void render(ID3D11DeviceContext* deviceContext, int vertexCount);

private:
	void initShader(WCHAR*, WCHAR*);


private:
	ID3D11SamplerState* m_sampleState;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_cameraBuffer;
	ID3D11Buffer* m_lightColorBuffer;
	ID3D11Buffer* m_lightPositionBuffer;
};