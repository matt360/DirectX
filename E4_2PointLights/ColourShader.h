// Colour shader.h
// Simple shader example.
#ifndef _COLOURSHADER_H_
#define _COLOURSHADER_H_

#include "../DXFramework/BaseShader.h"

using namespace std;
using namespace DirectX;


class ColourShader : public BaseShader
{
private:
	struct LightPositionBufferType
	{
		XMFLOAT3 lightPosition;
		float padding;
	};

public:

	ColourShader(ID3D11Device* device, HWND hwnd);
	~ColourShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, XMFLOAT3& lightPosition);

private:
	void initShader(WCHAR*, WCHAR*);

private:
	ID3D11Buffer* matrixBuffer;
	ID3D11Buffer* lightPositionBuffer;
};

#endif