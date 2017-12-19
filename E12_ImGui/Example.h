#pragma once
#include "../DXFramework/D3D.h"
#include "../DXFramework/BaseMesh.h"
#include "../DXFramework/BaseShader.h"
#include "../DXFramework/Camera.h"
#include "../DXFramework/TextureManager.h"

class Example
{
public:
	Example() {}
	~Example() {}

	virtual void initShader(D3D * renderer, HWND hwnd) = 0;
	
	float over_time;
	bool wireframe;
	bool example;
	XMFLOAT3 scale;
};

