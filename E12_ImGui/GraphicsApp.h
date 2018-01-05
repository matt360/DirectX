#pragma once
#include "../DXFramework/DXF.h"
#include "GeometryExample.h"
#include "MultiLightExample.h"
#include "SpecularLightExample.h"
#include "TerrainExample.h"
#include "TessellationExample.h"

enum class EXAMPLE_CHOICE {
	SPECULAR_LIGHT,
	TESSELLATION,
	TERRAIN,
	MULTILIGHT,
	GEOMETRY
};

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
	// CONSTRUCTOR FUNCTIONS
	void initGeometry();
	void initExamples();
	void loadTextures();

	// MESHES
	TriangleMesh *triangleMesh;
	SphereMesh *sphereMesh;
	CubeMesh *cubeMesh;
	QuadMesh *quadMesh;
	PlaneMesh *planeMesh;
	TerrainMesh *terrainMesh;

	// EXAMPLES
	SpecularLightExample* specularLightExample;
	TessellationExample* tessellationExample;
	TerrainExample* terrainExample;
	MultiLightExample* multiLightExample;
	GeometryExample* geometryExample;
	Example* example;
	EXAMPLE_CHOICE example_choice;

	// FUNCTIONS
	void chooseExample();
	void chooseExample(bool& activeEg, bool& inactiveEg1, bool& inactiveEg2, bool& inactiveEg3, bool& inactiveEg4);
	BaseMesh* chooseMesh(const MESH_CHOICE& mesh_choice);
	float clamp(float n, float lower, float upper);
};
