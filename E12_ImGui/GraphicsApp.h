// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "../DXFramework/DXF.h"
#include "ColourShader.h"
#include "TessellationShader.h"
#include "LightShader.h"

class GraphicsApp : public BaseApplication
{
public:

	GraphicsApp();
	~GraphicsApp();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input*);

	bool frame();

protected:
	bool render();
	void gui();

private:
	// CONSTRUCTOR
	void loadTextures();

	// TRIANGLE COLOUR SHADER //
	// shader handler
	LightShader* lightShader;
	// mesh
	// light
	void initLight();
	Light* m_Light;
	// render function
	void triangleColourShader();

	// TESSELLATION //
	// shader handler
	TessellationShader* tessellationShader;
	// meshes
	TriangleMesh *triangleMesh;
	SphereMesh *sphereMesh;
	CubeMesh *cubeMesh;
	QuadMesh *quadMesh;
	PlaneMesh *planeMesh;
	TerrainMesh *terrainMesh;
	// render function
	void tessellationTerrain();

	////////////// ImGUI ///////////
	void initGui();

	ImVec4 clear_col;
	bool isWireframe;
	bool triangle_colour_shader;
	bool tessellation_shader;
	////////////////////////////////

	// math
	float clamp(float n, float lower, float upper);
};

#endif
