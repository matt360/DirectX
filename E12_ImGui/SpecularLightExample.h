#pragma once
#include "SpecularLightShader.h"
#include "../DXFramework/D3D.h"
#include "../DXFramework/BaseMesh.h"
#include "../DXFramework/TextureManager.h"
#include <functional>

class SpecularLightExample
{
public:
	SpecularLightExample();
	~SpecularLightExample();

	void init(D3D * renderer, HWND hwnd);
	// SPECULAR LIGHT ////////////////////
	// shader handler
	SpecularLightShader* specularLightShader;
	bool wireframe;
	bool specular_light_example;
	// mesh
	// specular_light
	Light* specular_light;
	// render
	void initShaders(D3D* renderer, HWND hwnd);
	void initLight();
	void renderSpecularLightExample(D3D* renderer, Camera* camera, BaseMesh* sphereMesh, TextureManager* textureMgr);
	//void gui();
	//////////////////////////////////////
};

