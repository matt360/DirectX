// Light shader.h
// Basic single light shader setup
#pragma once

#include "../DXFramework/BaseShader.h"
#include "../DXFramework/Camera.h"

using namespace std;
using namespace DirectX;


class TessellationShader : public BaseShader
{
public:
	struct TessellationBufferType
	{
		float tessellationAmount;
		XMFLOAT3 padding;
	};

	struct CameraBufferType
	{
		XMFLOAT3 cameraPosition;
		float padding;
	};

public:

	TessellationShader(ID3D11Device* device, HWND hwnd);
	~TessellationShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix);
	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection,
		ID3D11ShaderResourceView* texture);
	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix,
		ID3D11ShaderResourceView* texture, float tessellationAmount);
	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix,
		ID3D11ShaderResourceView* texture, Camera* camera);

	void render(ID3D11DeviceContext* deviceContext, int vertexCount);

private:
	void initShader(WCHAR* vsFilename, WCHAR* psFilename);
	void initShader(WCHAR* vsFilename, WCHAR* hsFilename, WCHAR* dsFilename, WCHAR* psFilename);

private:
	ID3D11Buffer* matrixBuffer_;
	ID3D11SamplerState* sampleState_;
	ID3D11Buffer* tessellationBuffer_;
	ID3D11Buffer* cameraBuffer_;
};