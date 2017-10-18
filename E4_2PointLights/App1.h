// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "../DXFramework/DXF.h"
#include "ColourShader.h"
#include "LightShader.h"

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

	void initShaders(HWND hwnd);
	void initGeometry();
	void initLight();
	void initLightSpheres();

private:
	TriangleMesh* triangleMesh;
	SphereMesh* sphereMesh;
	CubeMesh* cubeMesh;
	QuadMesh* quadMesh;
	PlaneMesh* planeMesh;

	ColourShader* colourShader;
	LightShader* lightShader;
	
	Light *light0_, *light1_, *light2_, *light3_;
	SphereMesh *lightSphere0_, *lightSphere1_, *lightSphere2_, *lightSphere3_;

	float light_y;
};

#endif

