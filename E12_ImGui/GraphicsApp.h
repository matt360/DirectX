#pragma once
#include "../DXFramework/DXF.h"
#include "SpecularLightExample.h"
#include "TerrainExample.h"
#include "TessellationExample.h"
#include "MultiLightExample.h"
#include "GeometryExample.h"

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
	void initGeometry();
	void loadTextures();

	// meshes
	TriangleMesh *triangleMesh;
	SphereMesh *sphereMesh;
	CubeMesh *cubeMesh;
	QuadMesh *quadMesh;
	PlaneMesh *planeMesh;
	TerrainMesh *terrainMesh;

	// SPECULAR LIGHT EXAMPLE
	SpecularLightExample specularLightExample;
	// TESSELLATION EXAMPLE
	TessellationExample tessellationExample;
	// TERRAIN EXAMPLE
	TerrainExample terrainExample;
	// MULTI LIGHTS EXAMPLE
	MultiLightExample multiLightExample;
	// GEOMATRY SHADER EXAMPLE
	GeometryExample geometryExample;

	// math
	float clamp(float n, float lower, float upper);
};
