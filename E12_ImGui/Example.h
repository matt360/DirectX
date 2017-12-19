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
	// SPECULAR LIGHT ////////////////////
	// shader handler
	//BaseShader* shader;
	bool wireframe;
	bool example;
	// render
	//void initLight();
	//virtual void initLight() = 0;
	//void renderSpecularLightExample(D3D* renderer, Camera* camera, BaseMesh* sphereMesh, TextureManager* textureMgr);
};

