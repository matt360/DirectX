// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
//#include "../DXFramework/baseapplication.h"
//#include "D3D.h"
//#include "../DXFramework/planemesh.h"
#include "../DXFramework/DXF.h"
#include "LightShader.h"
#include "TextureShader.h"
//#include "../DXFramework/Light.h"
//#include"../DXFramework/CubeMesh.h"
#include "ShadowShader.h"
#include "DepthShader.h"

class App1 : public BaseApplication
{
public:

	App1();
	~App1();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in);

	bool frame();

protected:
	bool render();
	void depthPass();
	void renderScene();

private:
	LightShader* lightShader;
	TextureShader* textureShader;
	PlaneMesh* mesh;
	CubeMesh* cubeMesh;
	OrthoMesh* orthoMesh;
	
	Light* light;
	Model* model;
	ShadowShader* shadowShader;
	DepthShader* depthShader;

	RenderTexture* renderTexture;

};

#endif