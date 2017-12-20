// GraphicsApp.cpp
#include "GraphicsApp.h"

GraphicsApp::GraphicsApp()
{
	// geometry meshes
	triangleMesh = nullptr;
	sphereMesh = nullptr;
	cubeMesh = nullptr;
	quadMesh = nullptr;
	planeMesh = nullptr;
	terrainMesh = nullptr;
}

// Release the Direct3D objects
GraphicsApp::~GraphicsApp()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();
	
	// meshes
	if (triangleMesh)
	{
		delete triangleMesh;
		triangleMesh = 0;
	}

	if (sphereMesh)
	{
		delete sphereMesh;
		sphereMesh = 0;
	}

	if (cubeMesh)
	{
		delete cubeMesh;
		cubeMesh = 0;
	}

	if (quadMesh)
	{
		delete quadMesh;
		quadMesh = 0;
	}

	if (planeMesh)
	{
		delete planeMesh;
		planeMesh = 0;
	}

	if (terrainMesh)
	{
		delete terrainMesh;
		terrainMesh = 0;
	}

	// examples
	specularLightExample.~SpecularLightExample();
	tessellationExample.~TessellationExample();
	terrainExample.~TerrainExample();
	multiLightExample.~MultiLightExample();
	geometryExample.~GeometryExample();
}

void GraphicsApp::loadTextures()
{
	// load textures
	textureMgr->loadTexture("brick", L"../res/brick1.dds");
	textureMgr->loadTexture("bunny", L"../res/bunny.png");
	textureMgr->loadTexture("default", L"../res/DefaultDiffuse.png");
	textureMgr->loadTexture("height", L"../res/height.png");
	textureMgr->loadTexture("checkerboard", L"../res/checkerboard.png");
}

// Create mesh objects
void GraphicsApp::initGeometry()
{
	triangleMesh = new TriangleMesh(renderer->getDevice(), renderer->getDeviceContext());
	sphereMesh = new SphereMesh(renderer->getDevice(), renderer->getDeviceContext());
	cubeMesh = new CubeMesh(renderer->getDevice(), renderer->getDeviceContext());
	quadMesh = new QuadMesh(renderer->getDevice(), renderer->getDeviceContext());
	planeMesh = new PlaneMesh(renderer->getDevice(), renderer->getDeviceContext());
	terrainMesh = new TerrainMesh(renderer->getDevice(), renderer->getDeviceContext(), 100, 200);
}

void GraphicsApp::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in)
{
	// Call super init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in);

	loadTextures();
	initGeometry();
	specularLightExample.init(renderer, hwnd);
	terrainExample.init(renderer, hwnd);
	tessellationExample.init(renderer, hwnd);
	multiLightExample.init(renderer, hwnd);
	geometryExample.init(renderer, hwnd);
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
	terrainExample.over_time += XM_PIDIV2 * timer->getTime();
	terrainExample.over_time = fmodf(terrainExample.over_time, XM_2PI);

	// Render the graphics.
	result = render();
	if (!result)
	{
		return false;
	}

	return true;
}

BaseMesh* GraphicsApp::chooseMesh(const MESH_CHOICE& mesh_choice)
{
	BaseMesh* temp_mesh;
	switch (mesh_choice)
	{
	case MESH_CHOICE::TRIANGLE :
		temp_mesh = triangleMesh;
		break;

	case MESH_CHOICE::SPHERE :
		temp_mesh = sphereMesh;
		break;

	case MESH_CHOICE::CUBE :
		temp_mesh = cubeMesh;
		break;

	case MESH_CHOICE::QUAD :
		temp_mesh = quadMesh;
		break;

	case MESH_CHOICE::PLANE :
		temp_mesh = planeMesh;
		break;

	default:
		temp_mesh = triangleMesh;
		break;
	}

	return temp_mesh;
}

bool GraphicsApp::render()
{
	// Clear the scene. (default blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	camera->update();

	if (specularLightExample.example) {
		specularLightExample.render(renderer, camera, sphereMesh, textureMgr);
	}
	else if (tessellationExample.example) {
		tessellationExample.render(renderer, camera, terrainMesh, textureMgr);
	}
	else if (terrainExample.example) {
		terrainExample.render(renderer, camera, terrainMesh, textureMgr);
	}
	else if (multiLightExample.example) {
		multiLightExample.mesh = chooseMesh(multiLightExample.mesh_choice);
		multiLightExample.render(renderer, camera, textureMgr);
	}
	else if (geometryExample.example) {
		geometryExample.mesh = chooseMesh(geometryExample.mesh_choice);
		geometryExample.render(renderer, camera, textureMgr);
	}
	
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
	if (ImGui::Button("Specular Light Example"))
	{ 
		specularLightExample.example ^= 1;
		tessellationExample.example = false;
		terrainExample.example = false;
		multiLightExample.example = false;
		geometryExample.example = false;

		specularLightExample.wireframe = false;
		// set specular light camera
		camera->resetCamera();
	}
	// CHOOSE TESSELLATION EXAMPLE
	if (ImGui::Button("Tessellation Example"))
	{
		specularLightExample.example = false;
		tessellationExample.example ^= 1;
		terrainExample.example = false;
		multiLightExample.example = false;
		geometryExample.example = false;

		tessellationExample.wireframe = false;
		// set tessellation camera
		camera->setPosition(0.0f, 4.75f, -10.0f);
		camera->setRotation(0.0f, 30.0f, 0.0f);
	}
	// CHOOSE TERRAIN EXAMPLE 
	if (ImGui::Button("Terrain Example"))
	{
		specularLightExample.example = false;
		tessellationExample.example = false;
		terrainExample.example ^= 1;
		multiLightExample.example = false;
		geometryExample.example = false;

		// set terrain camera
		camera->setPosition(0.0f, 2.0f, -10.0f);
		camera->setRotation(0.0f, -200.0f, 0.0f);
		// reset terrain wireframe mode
		terrainExample.wireframe = false;
	}
	// CHOOSE MULTI LIGHT EXAMPLE
	if (ImGui::Button("Multi Light Example"))
	{
		specularLightExample.example = false;
		tessellationExample.example = false;
		terrainExample.example = false;
		multiLightExample.example ^= 1;
		geometryExample.example = false;

		// set multi light camera
		camera->setPosition(0.0f, 0.0f, -4.75f);
		camera->setRotation(0.0f, 0.f, 0.f);
		// scale up sphere mesh
		multiLightExample.scale = XMFLOAT3(1.0f, 1.0f, 20.0f);
		// reset light colours
		multiLightExample.light0_col = ImColor(1.0f, 0.0f, 0.0f, 1.0f);
		multiLightExample.light1_col = ImColor(0.0f, 1.0f, 0.0f, 1.0f);
		multiLightExample.light2_col = ImColor(0.0f, 0.0f, 1.0f, 1.0f);
		multiLightExample.light3_col = ImColor(1.0f, 1.0f, 1.0f, 1.0f);
		// reset light positions
		multiLightExample.light0_pos = XMFLOAT3(-3.0f, 0.1f, 3.0f);
		multiLightExample.light1_pos = XMFLOAT3(3.0f, 0.1f, 3.0f);
		multiLightExample.light2_pos = XMFLOAT3(-3.0f, 0.1f, -3.0f);
		multiLightExample.light3_pos = XMFLOAT3(3.0f, 0.1f, -3.0f);
		// render only sphere mesh
		multiLightExample.mesh_choice = MESH_CHOICE::SPHERE;
		multiLightExample.triangle_mesh = false;
		multiLightExample.sphere_mesh = true;
		multiLightExample.cube_mesh = false;
		multiLightExample.quad_mesh = false;
		multiLightExample.plane_mesh = false;
		// reset wireframe
		multiLightExample.wireframe = false;
	}
	// CHOOSE GEOMETRY SHADER EXAMPLE 
	if (ImGui::Button("Geometry Shader Example"))
	{
		specularLightExample.example = false;
		tessellationExample.example = false;
		terrainExample.example = false;
		multiLightExample.example = false;
		geometryExample.example ^= 1;

		// reset geometry shader scale
		geometryExample.scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
		// reset geometry shader primitive topology
		geometryExample.d3d11_primitive_topology_trianglelist = true;
		geometryExample.d3d11_primitive_topology_pointlist = false;
		// set cube mesh
		geometryExample.mesh_choice = MESH_CHOICE::CUBE;
		geometryExample.triangle_mesh = false;
		geometryExample.sphere_mesh = false;
		geometryExample.cube_mesh = true;
		geometryExample.quad_mesh = false;
		geometryExample.plane_mesh = false;
		// set wireframe
		geometryExample.wireframe = false;

		// set the camera
		camera->setPosition(13.0f, 4.0f, -22.0f);
		camera->setRotation(0.0f, -30.0f, 5.0f);
	}

	// EXAMPLE WINDOWS //
	// SPECULAR LIGHT EXAMPLE WINDOW
	specularLightExample.gui(camera);
	// TESSELLATION EXAMPLE WINDOW
	tessellationExample.gui(camera);
	// TERRAIN EXAMPLE WINDOW
	terrainExample.gui(camera);
	// MULTI LIGHT EXAMPLE WINDOW
	if (multiLightExample.example)
	{
		ImGui::Begin("Multi Light Example", &multiLightExample.example);
		if (ImGui::Button("Reset Example"))
		{
			// set multi light camera
			camera->setPosition(0.0f, 0.0f, -4.75f);
			camera->setRotation(0.0f, 0.f, 0.f);
			// scale up sphere mesh
			multiLightExample.scale = XMFLOAT3(1.0f, 1.0f, 20.0f);
			// reset light colours
			multiLightExample.light0_col = ImColor(1.0f, 0.0f, 0.0f, 1.0f);
			multiLightExample.light1_col = ImColor(0.0f, 1.0f, 0.0f, 1.0f);
			multiLightExample.light2_col = ImColor(0.0f, 0.0f, 1.0f, 1.0f);
			multiLightExample.light3_col = ImColor(1.0f, 1.0f, 1.0f, 1.0f);
			// reset light positions
			multiLightExample.light0_pos = XMFLOAT3(-3.0f, 0.1f, 3.0f);
			multiLightExample.light1_pos = XMFLOAT3(3.0f, 0.1f, 3.0f);
			multiLightExample.light2_pos = XMFLOAT3(-3.0f, 0.1f, -3.0f);
			multiLightExample.light3_pos = XMFLOAT3(3.0f, 0.1f, -3.0f);
			// render only sphere mesh
			multiLightExample.mesh_choice = MESH_CHOICE::SPHERE;
			multiLightExample.triangle_mesh = false;
			multiLightExample.sphere_mesh = true;
			multiLightExample.cube_mesh = false;
			multiLightExample.quad_mesh = false;
			multiLightExample.plane_mesh = false;
			// reset wireframe
			multiLightExample.wireframe = false;
			// reset geometry shader primitive topology
			multiLightExample.d3d11_primitive_topology_trianglelist = true;
			multiLightExample.d3d11_primitive_topology_pointlist = false;
		}
		ImGui::Checkbox("Wireframe", &multiLightExample.wireframe);
		// change lights' colour
		ImGui::ColorEdit3("Light 0 Col", (float*)&multiLightExample.light0_col);
		ImGui::ColorEdit3("Light 1 Col", (float*)&multiLightExample.light1_col);
		ImGui::ColorEdit3("Light 2 Col", (float*)&multiLightExample.light2_col);
		ImGui::ColorEdit3("Light 3 Col", (float*)&multiLightExample.light3_col);
		// change lights' position
		ImGui::SliderFloat3("Light 0 Pos", (float*)&multiLightExample.light0_pos, -10.0f, 10.0f);
		ImGui::SliderFloat3("Light 1 Pos", (float*)&multiLightExample.light1_pos, -10.0f, 10.0f);
		ImGui::SliderFloat3("Light 2 Pos", (float*)&multiLightExample.light2_pos, -10.0f, 10.0f);
		ImGui::SliderFloat3("Light 3 Pos", (float*)&multiLightExample.light3_pos, -10.0f, 10.0f);
		// scale
		ImGui::SliderFloat3("Scale", (float*)&multiLightExample.scale, -20.0f, 20.0f);
		// reset scale
		if (ImGui::Button("Reset Scale")) multiLightExample.scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
		// toggle topology
		if (ImGui::Checkbox("Primitive Topology Trianglelist", &multiLightExample.d3d11_primitive_topology_trianglelist))
			multiLightExample.d3d11_primitive_topology_pointlist = false;
		if (ImGui::Checkbox("Primitive Topology Pointlist", &multiLightExample.d3d11_primitive_topology_pointlist))
			multiLightExample.d3d11_primitive_topology_trianglelist = false;
		// what mesh to render (the highest one checked will be rendered (room for improvemnet: use menu box instead)
		if (ImGui::Checkbox("Triangle Mesh", &multiLightExample.triangle_mesh))
		{
			// set multi light camera
			camera->setPosition(0.0f, 0.0f, -4.75f);

			multiLightExample.mesh_choice = MESH_CHOICE::TRIANGLE;
			multiLightExample.triangle_mesh = true;
			multiLightExample.sphere_mesh = false;
			multiLightExample.cube_mesh = false;
			multiLightExample.quad_mesh = false;
			multiLightExample.plane_mesh = false;
		}
		if (ImGui::Checkbox("Sphere Mesh", &multiLightExample.sphere_mesh))
		{
			// set multi light camera
			camera->setPosition(0.0f, 0.0f, -4.75f);

			multiLightExample.mesh_choice = MESH_CHOICE::SPHERE;
			multiLightExample.triangle_mesh = false;
			multiLightExample.sphere_mesh = true;
			multiLightExample.cube_mesh = false;
			multiLightExample.quad_mesh = false;
			multiLightExample.plane_mesh = false;
		}
		if (ImGui::Checkbox("Cube Mesh", &multiLightExample.cube_mesh))
		{
			// set multi light camera
			camera->setPosition(0.0f, 0.0f, -4.75f);

			multiLightExample.mesh_choice = MESH_CHOICE::CUBE;
			multiLightExample.triangle_mesh = false;
			multiLightExample.sphere_mesh = false;
			multiLightExample.cube_mesh = true;
			multiLightExample.quad_mesh = false;
			multiLightExample.plane_mesh = false;
		}
		if (ImGui::Checkbox("Quad Mesh", &multiLightExample.quad_mesh))
		{
			// set multi light camera
			camera->setPosition(0.0f, 0.0f, -4.75f);

			multiLightExample.mesh_choice = MESH_CHOICE::QUAD;
			multiLightExample.triangle_mesh = false;
			multiLightExample.sphere_mesh = false;
			multiLightExample.cube_mesh = false;
			multiLightExample.quad_mesh = true;
			multiLightExample.plane_mesh = false;
		}
		if (ImGui::Checkbox("Plane Mesh", &multiLightExample.plane_mesh))
		{
			camera->setPosition(0.0f, 3.0f, 0.0f);
			multiLightExample.scale = XMFLOAT3(1.0f, 1.0f, 1.0f);

			multiLightExample.mesh_choice = MESH_CHOICE::PLANE;
			multiLightExample.triangle_mesh = false;
			multiLightExample.sphere_mesh = false;
			multiLightExample.cube_mesh = false;
			multiLightExample.quad_mesh = false;
			multiLightExample.plane_mesh = true;
		}
		ImGui::End();
	}
	// GEOMETRY SHADER EXAMPLE WINDOW
	if (geometryExample.example)
	{
		ImGui::Begin("Geometry Shader Example", &geometryExample.example);
		if (ImGui::Button("Reset Example"))
		{
			geometryExample.mesh_choice = MESH_CHOICE::CUBE;
			// reset geometry shader scale
			geometryExample.scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
			// reset geometry shader wireframe mode
			geometryExample.wireframe = false;
			// reset geometry shader primitive topology
			geometryExample.d3d11_primitive_topology_trianglelist = true;
			geometryExample.d3d11_primitive_topology_pointlist = false;
			// set cube mesh
			geometryExample.triangle_mesh = false;
			geometryExample.sphere_mesh = false;
			geometryExample.cube_mesh = true;
			geometryExample.quad_mesh = false;
			geometryExample.plane_mesh = false;
			// set the camera
			camera->setPosition(13.0f, 4.0f, -22.0f);
			camera->setRotation(0.0f, -35.0f, 0.0f);
		}
		ImGui::Checkbox("Wireframe", &geometryExample.wireframe);
		ImGui::SliderFloat3("Scale", (float*)&geometryExample.scale, -10.0f, 10.0f);
		if (ImGui::Button("Reset Scale")) geometryExample.scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
		if (ImGui::Checkbox("Primitive Topology Trianglelist", &geometryExample.d3d11_primitive_topology_trianglelist))
			geometryExample.d3d11_primitive_topology_pointlist = false;
		if (ImGui::Checkbox("Primitive Topology Pointlist", &geometryExample.d3d11_primitive_topology_pointlist))
			geometryExample.d3d11_primitive_topology_trianglelist = false;

		// what mesh to render (the highest one checked will be rendered (room for improvemnet: use menu box instead)
		if (ImGui::Checkbox("Triangle Mesh", &geometryExample.triangle_mesh))
		{
			geometryExample.mesh_choice = MESH_CHOICE::TRIANGLE;
			geometryExample.triangle_mesh = true;
			geometryExample.sphere_mesh = false;
			geometryExample.cube_mesh = false;
			geometryExample.quad_mesh = false;
			geometryExample.plane_mesh = false;
		}
		if (ImGui::Checkbox("Sphere Mesh", &geometryExample.sphere_mesh))
		{
			geometryExample.mesh_choice = MESH_CHOICE::SPHERE;
			geometryExample.triangle_mesh = false;
			geometryExample.sphere_mesh = true;
			geometryExample.cube_mesh = false;
			geometryExample.quad_mesh = false;
			geometryExample.plane_mesh = false;
		}
		if (ImGui::Checkbox("Cube Mesh", &geometryExample.cube_mesh))
		{
			geometryExample.mesh_choice = MESH_CHOICE::CUBE;
			geometryExample.triangle_mesh = false;
			geometryExample.sphere_mesh = false;
			geometryExample.cube_mesh = true;
			geometryExample.quad_mesh = false;
			geometryExample.plane_mesh = false;
		}
		if (ImGui::Checkbox("Quad Mesh", &geometryExample.quad_mesh))
		{
			geometryExample.mesh_choice = MESH_CHOICE::QUAD;
			geometryExample.triangle_mesh = false;
			geometryExample.sphere_mesh = false;
			geometryExample.cube_mesh = false;
			geometryExample.quad_mesh = true;
			geometryExample.plane_mesh = false;
		}
		if (ImGui::Checkbox("Plane Mesh", &geometryExample.plane_mesh))
		{
			geometryExample.mesh_choice = MESH_CHOICE::PLANE;
			geometryExample.triangle_mesh = false;
			geometryExample.sphere_mesh = false;
			geometryExample.cube_mesh = false;
			geometryExample.quad_mesh = false;
			geometryExample.plane_mesh = true;
		}
		ImGui::End();
	}

	// Render UI
	ImGui::Render();
}

float GraphicsApp::clamp(float n, float lower, float upper)
{
	return std::fmax(lower, (std::fmin(n, upper)));
}

