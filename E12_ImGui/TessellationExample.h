#pragma once
#include "Example.h"
#include "TessellationShader.h"

class TessellationExample : public Example
{
public:
	TessellationExample();
	~TessellationExample();

	void init(D3D * renderer, HWND hwnd);
	void initShader(D3D * renderer, HWND hwnd);
	//void initLight();
	void render(D3D* renderer, Camera* camera, BaseMesh* mesh, TextureManager* textureMgr);

	// shader handler
	TessellationShader* shader;
};

