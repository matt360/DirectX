// Light shader.h
// Geometry shader example.
#ifndef _GEOMETRYSHADER_H_
#define _GEOMETRYSHADER_H_

#include "../DXFramework/BaseShader.h"

using namespace std;
using namespace DirectX;


class GeometryShader : public BaseShader
{

public:

	GeometryShader(ID3D11Device* device, HWND hwnd);
	~GeometryShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture);
	void setShaderParameters(ID3D11DeviceContext * deviceContext, const XMMATRIX & worldMatrix, const XMMATRIX & viewMatrix, const XMMATRIX & projectionMatrix, ID3D11ShaderResourceView * texture_1, ID3D11ShaderResourceView * texture_2);
	void render(ID3D11DeviceContext* deviceContext, int vertexCount);

private:
	void initShader(WCHAR* vsFilename, WCHAR* psFilename);
	void initShader(WCHAR* vsFilename, WCHAR* gsFilename, WCHAR* psFilename);

private:
	ID3D11Buffer* matrixBuffer_;
	ID3D11SamplerState* sampleState_;
};

#endif