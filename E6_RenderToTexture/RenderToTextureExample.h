// Application.h
#pragma once

// Includes
#include "../DXFramework/DXF.h"
#include "LightShader.h"
#include "TextureShader.h"

class RenderToTextureExample : public BaseApplication
{
public:

	RenderToTextureExample();
	~RenderToTextureExample();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in);

	bool frame();

protected:
	bool render();
	void gui();

	void initLight();

	void RenderToTexture(float time);
	void RenderScene(float time);

private:
	TriangleMesh* triangleMesh;
	SphereMesh* sphereMesh;
	CubeMesh* cubeMesh;
	QuadMesh* quadMesh;
	PlaneMesh* planeMesh;
	// Shader handler
	LightShader* lightShader;
	TextureShader* textureShader;
	
	Light* light;

	RenderTexture* renderTexture;
	OrthoMesh* orthoMesh;

	float light_y;
};