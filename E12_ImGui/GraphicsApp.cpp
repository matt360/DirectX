// GraphicsApp.cpp
#include "GraphicsApp.h"

GraphicsApp::GraphicsApp()
{
	// geometry meshes
	triangleMesh_ = nullptr;
	sphereMesh_ = nullptr;
	cubeMesh_ = nullptr;
	quadMesh_ = nullptr;
	planeMesh_ = nullptr;
	terrainMesh_ = nullptr;

	specularLightExample_ = nullptr;
	tessellationExample_ = nullptr;
	terrainExample_ = nullptr;
	multiLightExample_ = nullptr;
	geometryExample_ = nullptr;
	example_ = nullptr;
}

// Release the Direct3D objects
GraphicsApp::~GraphicsApp()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();
	
	// meshes
	if (triangleMesh_)
	{
		delete triangleMesh_;
		triangleMesh_ = 0;
	}

	if (sphereMesh_)
	{
		delete sphereMesh_;
		sphereMesh_ = 0;
	}

	if (cubeMesh_)
	{
		delete cubeMesh_;
		cubeMesh_ = 0;
	}

	if (quadMesh_)
	{
		delete quadMesh_;
		quadMesh_ = 0;
	}

	if (planeMesh_)
	{
		delete planeMesh_;
		planeMesh_ = 0;
	}

	if (terrainMesh_)
	{
		delete terrainMesh_;
		terrainMesh_ = 0;
	}

	// examples
	specularLightExample_->~SpecularLightExample();
	tessellationExample_->~TessellationExample();
	terrainExample_->~TerrainExample();
	multiLightExample_->~MultiLightExample();
	geometryExample_->~GeometryExample();
}

void GraphicsApp::loadTextures()
{
	// load textures
	textureMgr->loadTexture("brick", L"../res/brick1.dds");
	textureMgr->loadTexture("bunny", L"../res/bunny.png");
	textureMgr->loadTexture("height", L"../res/height.png");
	textureMgr->loadTexture("checkerboard", L"../res/checkerboard.png");
}

// Create mesh objects
void GraphicsApp::initGeometry()
{
	triangleMesh_ = new TriangleMesh(renderer->getDevice(), renderer->getDeviceContext());
	sphereMesh_ = new SphereMesh(renderer->getDevice(), renderer->getDeviceContext());
	cubeMesh_ = new CubeMesh(renderer->getDevice(), renderer->getDeviceContext());
	quadMesh_ = new QuadMesh(renderer->getDevice(), renderer->getDeviceContext());
	planeMesh_ = new PlaneMesh(renderer->getDevice(), renderer->getDeviceContext());
	terrainMesh_ = new TerrainMesh(renderer->getDevice(), renderer->getDeviceContext(), 100, 200);
}

void GraphicsApp::initExamples()
{
	specularLightExample_ = new SpecularLightExample;
	terrainExample_ = new TerrainExample;
	tessellationExample_ = new TessellationExample;
	multiLightExample_ = new MultiLightExample;
	geometryExample_ = new GeometryExample;
}

void GraphicsApp::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in)
{
	// Call super init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in);

	loadTextures();
	initGeometry();
	initExamples();

	// initialize examples
	specularLightExample_->init(renderer, hwnd);
	terrainExample_->init(renderer, hwnd);
	tessellationExample_->init(renderer, hwnd);
	multiLightExample_->init(renderer, hwnd);
	geometryExample_->init(renderer, hwnd);

	// set default example to render
	chooseExample(EXAMPLE_CHOICE::SPECULAR_LIGHT);
}

bool GraphicsApp::frame()
{
	bool result;

	result = BaseApplication::frame();
	if (!result)
	{
		return false;
	}

	// calculate new y position of light_terrain
	terrainExample_->over_time += XM_PIDIV2 * timer->getTime();
	terrainExample_->over_time = fmodf(terrainExample_->over_time, XM_2PI);

	// Render the graphics.
	result = render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool GraphicsApp::render()
{
	// Clear the scene. (default blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	// update camera
	camera->update();

	// set example's mesh
	example_->setMesh(chooseMesh(example_->getMeshChoice()));
	// render example
	example_->render(renderer, camera, textureMgr);
	
	// Render GUI
	gui();

	// Present the rendered scene to the screen.
	renderer->endScene();

	return true;
}

void GraphicsApp::gui()
{
	// Force turn off on Geometry shader and force fill rendering
	renderer->getDeviceContext()->GSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->HSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->DSSetShader(NULL, NULL, 0);
	renderer->setWireframeMode(false);

	// MAIN WINDOW DISPLAY INFO
	ImGui::Text("FPS: %.2f", timer->getFPS());
	ImGui::Text("Camera Position: x: %.2f y: %.2f z: %.2f", camera->getPosition().x, camera->getPosition().y, camera->getPosition().z);
	// MAIN WINDOW BUTTONS
	if (ImGui::Button("Reset camera"))
	{
		camera->resetCamera();
	}
	// CHOOSE SPECULAR LIGHT EXAMPLE
	else if (ImGui::Button("Specular Light Example"))
	{ 
		chooseExample(EXAMPLE_CHOICE::SPECULAR_LIGHT);
	}
	// CHOOSE TESSELLATION EXAMPLE
	else if (ImGui::Button("Tessellation Example"))
	{
		chooseExample(EXAMPLE_CHOICE::TESSELLATION);
	}
	// CHOOSE TERRAIN EXAMPLE 
	else if (ImGui::Button("Terrain Example"))
	{
		chooseExample(EXAMPLE_CHOICE::TERRAIN);
	}
	// CHOOSE MULTI LIGHT EXAMPLE
	else if (ImGui::Button("Multi Light Example"))
	{
		chooseExample(EXAMPLE_CHOICE::MULTILIGHT);
	}
	// CHOOSE GEOMETRY SHADER EXAMPLE 
	else if (ImGui::Button("Geometry Shader Example"))
	{
		chooseExample(EXAMPLE_CHOICE::GEOMETRY);
	}

	// EXAMPLES' GUI
	specularLightExample_->gui(camera);
	tessellationExample_->gui(camera);
	terrainExample_->gui(camera);
	multiLightExample_->gui(camera);
	geometryExample_->gui(camera);

	// Render UI
	ImGui::Render();
}

void GraphicsApp::setActiveExample(bool& activeEg, bool& inactiveEg1, bool& inactiveEg2, bool& inactiveEg3, bool& inactiveEg4)
{
	activeEg = !activeEg;
	inactiveEg1 = false;
	inactiveEg2 = false;
	inactiveEg3 = false;
	inactiveEg4 = false;
}

void GraphicsApp::chooseExample(EXAMPLE_CHOICE eg)
{
	switch (eg)
	{
	case EXAMPLE_CHOICE::SPECULAR_LIGHT:
		example_ = specularLightExample_;
		example_choice_ = EXAMPLE_CHOICE::SPECULAR_LIGHT;
		setActiveExample(specularLightExample_->example, tessellationExample_->example, terrainExample_->example,
			multiLightExample_->example, geometryExample_->example);

		specularLightExample_->resetExample(camera);
		break;

	case EXAMPLE_CHOICE::TESSELLATION:
		example_ = tessellationExample_;
		example_choice_ = EXAMPLE_CHOICE::TESSELLATION;
		setActiveExample(tessellationExample_->example, specularLightExample_->example, terrainExample_->example,
			multiLightExample_->example, geometryExample_->example);

		tessellationExample_->resetExample(camera);
		break;

	case EXAMPLE_CHOICE::TERRAIN:
		example_ = terrainExample_;
		// set choice
		example_choice_ = EXAMPLE_CHOICE::TERRAIN;
		setActiveExample(terrainExample_->example, specularLightExample_->example, tessellationExample_->example,
			multiLightExample_->example, geometryExample_->example);

		terrainExample_->resetExample(camera);
		break;

	case EXAMPLE_CHOICE::MULTILIGHT:
		example_ = multiLightExample_;
		// set choice
		example_choice_ = EXAMPLE_CHOICE::MULTILIGHT;
		setActiveExample(multiLightExample_->example, specularLightExample_->example, tessellationExample_->example,
			terrainExample_->example, geometryExample_->example);

		multiLightExample_->resetExample(camera);
		break;

	case EXAMPLE_CHOICE::GEOMETRY:
		example_ = geometryExample_;
		// set choice
		example_choice_ = EXAMPLE_CHOICE::GEOMETRY;
		setActiveExample(geometryExample_->example, specularLightExample_->example, tessellationExample_->example,
			terrainExample_->example, multiLightExample_->example);

		geometryExample_->resetExample(camera);
		break;

	default:
		example_ = specularLightExample_;
		example_choice_ = EXAMPLE_CHOICE::SPECULAR_LIGHT;
		setActiveExample(specularLightExample_->example, tessellationExample_->example, terrainExample_->example,
			multiLightExample_->example, geometryExample_->example);

		specularLightExample_->resetExample(camera);
		break;
	}
}

BaseMesh* GraphicsApp::chooseMesh(const MESH_CHOICE& mesh_choice)
{
	switch (mesh_choice)
	{
	case MESH_CHOICE::TRIANGLE:
		return triangleMesh_;
		break;

	case MESH_CHOICE::SPHERE:
		return sphereMesh_;
		break;

	case MESH_CHOICE::CUBE:
		return cubeMesh_;
		break;

	case MESH_CHOICE::QUAD:
		return quadMesh_;
		break;

	case MESH_CHOICE::PLANE:
		return planeMesh_;
		break;

	case MESH_CHOICE::TERRAIN:
		return terrainMesh_;
		break;

	default:
		return triangleMesh_;
		break;
	}
}

float GraphicsApp::clamp(float n, float lower, float upper)
{
	return std::fmax(lower, (std::fmin(n, upper)));
}

