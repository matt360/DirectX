#pragma once
#include "Example.h"
#include "SpecularLightShader.h"

class SpecularLightExample : public Example
{
public:
	SpecularLightExample();
	~SpecularLightExample();

	void init(D3D * renderer, HWND hwnd);
	void initShader(D3D * renderer, HWND hwnd);
	void initVariables();
	void initLight();
	void render(D3D* renderer, Camera* camera, BaseMesh* sphereMesh, TextureManager* textureMgr);
	void gui(Camera* camera);
	
	// shader handler
	SpecularLightShader* shader;
	// light
	Light* light;
};

