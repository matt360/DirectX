#pragma once
#include "Example.h"
#include "TerrainTessellationShader.h"

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
	// shader handler
	TerrainTessellationShader* shader_;
	// light
	Light* light_;

	float frequency_, choice_;
	string height_texture_, mapping_texture_1_, mapping_texture_2_;
	XMFLOAT3 new_light_pos_;
};

