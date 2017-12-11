// Application.h
/*
NOTES
In simple terms the main change is that the programmable vertex shader has been broken into three parts.
These three parts are the vertex shader, the hull shader, and the domain shader.

Tessellation occurs between the hull and the domain shader.

Instead of sending triangle lists to the video card we now send patch lists.
A patch is a primitive made up of control points.
For example if we send a single triangle to the video card it is 
considered a single patch with three control points.

*/

#ifndef _APP9_H
#define _APP9_H

// Includes
#include "../DXFramework/DXF.h"
#include "TessellationShader.h"

class App9 : public BaseApplication
{
public:

	App9();
	~App9();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in);

	bool frame();

protected:
	bool render();
	void gui();
	float clamp(float n, float lower, float upper);
	float tessellationAmount;

	bool wireframe;
private:
	TessellationShader* tessellationShader;

	TriangleMesh *triangleMesh;
	SphereMesh *sphereMesh;
	CubeMesh *cubeMesh;
	QuadMesh *quadMesh;
	PlaneMesh *planeMesh;
	TerrainMesh *terrainMesh;
};

#endif