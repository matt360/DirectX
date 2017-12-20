#pragma once
#include "../DXFramework/DXF.h"
#include "GeometryExample.h"
#include "MultiLightExample.h"
#include "SpecularLightExample.h"
#include "TerrainExample.h"
#include "TessellationExample.h"

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

	// FUNCTIONS
	float clamp(float n, float lower, float upper);
	BaseMesh* chooseMesh(const MESH_CHOICE& mesh_choice);

	// MESHES
	TriangleMesh *triangleMesh;
	SphereMesh *sphereMesh;
	CubeMesh *cubeMesh;
	QuadMesh *quadMesh;
	PlaneMesh *planeMesh;
	TerrainMesh *terrainMesh;

	// EXAMPLES
	SpecularLightExample specularLightExample;
	TessellationExample tessellationExample;
	TerrainExample terrainExample;
	MultiLightExample multiLightExample;
	GeometryExample geometryExample;
};
