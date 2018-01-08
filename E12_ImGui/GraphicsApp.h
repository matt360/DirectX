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
	TriangleMesh *triangleMesh_;
	SphereMesh *sphereMesh_;
	CubeMesh *cubeMesh_;
	QuadMesh *quadMesh_;
	PlaneMesh *planeMesh_;
	TerrainMesh *terrainMesh_;

	// EXAMPLES
	SpecularLightExample* specularLightExample_;
	TessellationExample* tessellationExample_;
	TerrainExample* terrainExample_;
	MultiLightExample* multiLightExample_;
	GeometryExample* geometryExample_;
	Example* example_;
	EXAMPLE_CHOICE example_choice_;

	// FUNCTIONS
	void setActiveExample(bool& activeEg, bool& inactiveEg1, bool& inactiveEg2, bool& inactiveEg3, bool& inactiveEg4);
	void chooseExample(EXAMPLE_CHOICE eg);
	BaseMesh* chooseMesh(const MESH_CHOICE& mesh_choice);
	float clamp(float n, float lower, float upper);
};
