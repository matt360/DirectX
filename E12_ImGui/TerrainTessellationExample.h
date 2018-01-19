#pragma once
#include "Example.h"
#include "TerrainTessellationShader.h"
#include "TextureShader.h"

class TerrainTessellationExample : public Example
{
public:
	TerrainTessellationExample();
	~TerrainTessellationExample();

	void init(D3D * renderer, HWND hwnd);
	void initShader(D3D * renderer, HWND hwnd);
	void initVariables();
	void initLight();
	void render(D3D* renderer, Camera* camera, TextureManager* textureMgr);
	void gui(Camera* camera);
	void resetExample(Camera* camera);

private:
	void renderToTexture(D3D* renderer, Camera* camera, TextureManager* textureMgr);
	void renderScene(D3D* renderer, Camera* camera, TextureManager* textureMgr);
	// shader handler
	TerrainTessellationShader* shader_;
	TextureShader* textureShader_;
	// render to texture
	RenderTexture* renderTexture;
	OrthoMesh* orthoMesh;

	// light
	Light* light_;

	float frequency_, choice_;
	string height_texture_, mapping_texture_1_, mapping_texture_2_;
	XMFLOAT3 new_light_pos_;
	bool explode_;
	float time_;
};

