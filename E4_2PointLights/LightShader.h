// Light shader.h
// Basic single light shader setup
#ifndef _LIGHTSHADER_H_
#define _LIGHTSHADER_H_

#include "../DXFramework/BaseShader.h"
#include "../DXFramework/Light.h"
#include "../DXFramework/Camera.h"

using namespace std;
using namespace DirectX;

const int NUM_LIGHTS = 4;

class LightShader : public BaseShader
{
private:
	struct LightBufferType
	{
		XMFLOAT4 ambient;
		XMFLOAT4 diffuse[NUM_LIGHTS];
		XMFLOAT3 direction;
		float specularPower;
		XMFLOAT4 specular;
		XMFLOAT4 position[NUM_LIGHTS];
		//float padding;
	};

	struct CameraBufferType
	{
		XMFLOAT3 cameraPosition;
		float padding;
	};

public:
	// When resources are being created and interfaced with,
	// the 'device' interface is used.
	LightShader(ID3D11Device* device, HWND hwnd);
	~LightShader();

	// When the pipeline or a resource is being manipulated,
	// the 'device context' is used.
	void setShaderParameters
	(
		ID3D11DeviceContext* deviceContext, 
		const XMMATRIX& worldMatrix, 
		const XMMATRIX& viewMatrix, 
		const XMMATRIX& projectionMatrix, 
		ID3D11ShaderResourceView* texture, 
		Light* light0,
		Light* light1,
		Light* light2,
		Light* light3,
		Camera* camera
	);
	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, Light* light);
	void render(ID3D11DeviceContext* deviceContext, int vertexCount);

private:
	void initShader(WCHAR*, WCHAR*);


private:
	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11Buffer* lightBuffer;
	ID3D11Buffer* cameraBuffer;
};

#endif