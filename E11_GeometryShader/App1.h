// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "../DXFramework/DXF.h"
#include "GeometryShader.h"

class App1 : public BaseApplication
{
public:

	App1();
	~App1();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in);

	bool frame();

protected:
	bool render();
	bool wireframe;

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
	GeometryShader* geometryShader;

	Light* light;

	RenderTexture* renderTexture;

	OrthoMesh* orthoMesh;

	float light_y;
};

#endif