// Application.h
#pragma once

// Includes
#include "Example.h"
#include "LightShader.h"
#include "TextureShader.h"

class RenderToTextureExample : public Example
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
	// Shader handler
	LightShader* lightShader;
	TextureShader* textureShader;
	
	Light* light;

	RenderTexture* renderTexture;
	OrthoMesh* orthoMesh;

	float light_y;
};