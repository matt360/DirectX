// Application.h
#pragma once

// Includes
#include "Example.h"
#include "RTTLightShader.h"
#include "RTTTextureShader.h"

class RenderToTextureExample : public Example
{
public:
	RenderToTextureExample();
	~RenderToTextureExample();
	void init(D3D* renderer, HWND hwnd, int screenWidth, int screenHeight);
	void initShader(D3D* renderer, HWND hwnd, int screenWidth, int screenHeight);
	void initVariables();
	void initLight();
	void renderToTexture(D3D* renderer, Camera* camera, TextureManager* textureMgr);
	void renderScene(D3D* renderer, Camera* camera, TextureManager* textureMgr);
	void render(D3D* renderer, Camera* camera, TextureManager* textureMgr);
	void gui(Camera* camera);
	void resetExample(Camera* camera);

protected:
	void RenderToTexture(float time);
	void RenderScene(float time);
private:
	// Shader handler
	RTTLightShader* lightShader;
	RTTTextureShader* textureShader;
	
	Light* light;

	RenderTexture* renderTexture;
	OrthoMesh* orthoMesh;

	float light_y;
};