// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "../DXFramework/DXF.h"
#include "ColourShader.h"
#include "LightShader.h"
#include "TextureShader.h"
#include "HorizontalBlurShader.h"
#include "VerticalBlurShader.h"

class App1 : public BaseApplication
{
public:

	App1();
	~App1();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in);

	bool frame();

protected:
	bool render();
	void gui();

	void initLight();

	void RenderToTexture(float time);

	void DownSampleTexture();
	void RenderHorizontalBlurToTexture();
	void RenderVerticalBlurToTexture();
	void UpSampleTexture();
	//bool Render2DTextureScene();

	void RenderScene(float time);

private:
	TriangleMesh *triangleMesh;
	SphereMesh *sphereMesh;
	CubeMesh *cubeMesh;
	QuadMesh *quadMesh;
	PlaneMesh *planeMesh;
	// Shader handler
	ColourShader *colourShader;
	LightShader *lightShader;
	TextureShader *textureShader;
	HorizontalBlurShader *horizontalBlurShader;
	VerticalBlurShader *verticalBlurShader;
	// RenderTexture objects
	RenderTexture *renderTexture, *downSampleTexure, *horizontalBlurTexture, *verticalBlurTexture, *upSampleTexure;

	Light* light;

	OrthoMesh *orthoMesh;
	OrthoMesh *smallWindow, *fullScreenWindow;

	float light_y;
};

#endif