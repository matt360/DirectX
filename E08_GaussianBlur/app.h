// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "../DXFramework/baseapplication.h"
#include "D3D.h"
#include "../DXFramework/planemesh.h"
#include "LightShader.h"
#include "TextureShader.h"
#include "../DXFramework/Light.h"
#include"../DXFramework/CubeMesh.h"
#include "../DXFramework/DXF.h"
#include "VerticalBlurShader.h"
#include "HorizontalBlurShader.h"

class App1 : public BaseApplication
{
public:

	App1();
	~App1();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in);

	bool frame();

protected:
	bool render();
	void renderToTexture();
	void downSample();
	void horizontalBlur();
	void verticalBlur();
	void upSample();
	void renderScene();

private:
	
	CubeMesh* mesh;
	OrthoMesh* orthoMesh;
	Light* light;

	RenderTexture* renderTexture;
	RenderTexture* downSampleTexture;
	RenderTexture* horizontalBlurTexture;
	RenderTexture* verticalBlurTexture;
	RenderTexture* upSampleTexture;
	OrthoMesh* downSampleMesh;
	OrthoMesh* upSampleMesh;
	TextureShader* textureShader;
	VerticalBlurShader* verticalBlurShader;
	HorizontalBlurShader* horizontalBlurShader;
	LightShader* lightShader;

};

#endif