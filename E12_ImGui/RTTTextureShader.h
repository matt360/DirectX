// texture shader.h
#pragma once

#include "../DXFramework/BaseShader.h"

using namespace std;
using namespace DirectX;


class RTTTextureShader : public BaseShader
{

public:

	RTTTextureShader(ID3D11Device* device, HWND hwnd);
	~RTTTextureShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture);
	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture_1, ID3D11ShaderResourceView* texture_2);
	void render(ID3D11DeviceContext* deviceContext, int vertexCount);

private:
	void initShader(WCHAR*, WCHAR*);

private:
	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* sampleState;
};