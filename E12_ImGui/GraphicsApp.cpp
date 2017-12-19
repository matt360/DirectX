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

	// shader handlers
	geometryShader = nullptr;
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

	if (geometryShader)
	{
		delete geometryShader;
		geometryShader = 0;
	}
}

void GraphicsApp::initVariables()
{
}

void GraphicsApp::initLight()
{
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

// create shader handlers
void GraphicsApp::initShaders(D3D* renderer, HWND hwnd)
{
	geometryShader = new GeometryShader(renderer->getDevice(), hwnd);
}

void GraphicsApp::initGuiVariables()
{
	// geometry shader exmaple scale 
	gs_scale = XMFLOAT3(1.0f, 1.0f, 1.0f);

	// geomatry shader topology handler (set to triangle list by default)
	d3d11_primitive_topology_trianglelist = true;
}

void GraphicsApp::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in)
{
	// Call super init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in);

	initVariables();
	//initLight();
	loadTextures();
	initGeometry();
	initShaders(renderer, hwnd);
	specularLightExample.init(renderer, hwnd);
	terrainExample.init(renderer, hwnd);
	tessellationExample.init(renderer, hwnd);
	multiLightExample.init(renderer, hwnd);
	initGuiVariables();
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

void GraphicsApp::renderGeometryShaderExample()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	// Clear the scene. (default blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	// Generate the view matrix based on the camera's position.
	camera->update();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	worldMatrix = renderer->getWorldMatrix();

	XMMATRIX matrix1Translation = DirectX::XMMatrixTranslation(2.0f, 0.0f, 0.0f);
	//XMMATRIX matrix1Rotation = DirectX::XMMatrixRotationZ(light_y);
	XMMATRIX matrix1Rotation = DirectX::XMMatrixRotationZ(0.0f);
	// orbit
	//worldMatrix = XMMatrixMultiply(matrix1Translation, matrix1Rotation);
	// translate and rotate
	worldMatrix = XMMatrixMultiply(matrix1Rotation, matrix1Translation);

	// scaling
	XMMATRIX matrix1Scaling = DirectX::XMMatrixScaling(gs_scale.x, gs_scale.y, gs_scale.z);
	worldMatrix *= matrix1Scaling;

	viewMatrix = camera->getViewMatrix();

	projectionMatrix = renderer->getProjectionMatrix();

	// wireframe mode
	renderer->setWireframeMode(gs_wireframe);

	/*
	// ONE TRIANGLE - D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	triangleMesh->sendData(renderer->getDeviceContext(), D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// THREE TRIANGLES - D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST
	triangleMesh->sendData(renderer->getDeviceContext(), D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	// *NOTE* ecause we're offsetting from the normals there's going to be SIX cubes around the original cube
	cubeMesh->sendData(renderer->getDeviceContext(), D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	*/

	D3D_PRIMITIVE_TOPOLOGY d3d11_primitive_topology;
	//d3d11_primitive_topology_trianglelist ? d3d11_primitive_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST : d3d11_primitive_topology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
	if (d3d11_primitive_topology_trianglelist) d3d11_primitive_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	else d3d11_primitive_topology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;

	// Send geometry data (from mesh)
	if (gs_triangle_mesh) triangleMesh->sendData(renderer->getDeviceContext(), d3d11_primitive_topology);
	if (gs_sphere_mesh)   sphereMesh->sendData(renderer->getDeviceContext(), d3d11_primitive_topology);
	if (gs_cube_mesh)     cubeMesh->sendData(renderer->getDeviceContext(), d3d11_primitive_topology);
	if (gs_quad_mesh)     quadMesh->sendData(renderer->getDeviceContext(), d3d11_primitive_topology);
	if (gs_plane_mesh)    planeMesh->sendData(renderer->getDeviceContext(), d3d11_primitive_topology);

	// Set shader parameters (matrices and texture)
	geometryShader->setShaderParameters(renderer->getDeviceContext(),
		worldMatrix, viewMatrix, projectionMatrix,
		textureMgr->getTexture("checkerboard"),
		textureMgr->getTexture("bunny"));

	// Render object (combination of mesh geometry and shader process
	if (gs_triangle_mesh) geometryShader->render(renderer->getDeviceContext(), triangleMesh->getIndexCount());
	if (gs_sphere_mesh)   geometryShader->render(renderer->getDeviceContext(), sphereMesh->getIndexCount());
	if (gs_cube_mesh)     geometryShader->render(renderer->getDeviceContext(), cubeMesh->getIndexCount());
	if (gs_quad_mesh)     geometryShader->render(renderer->getDeviceContext(), quadMesh->getIndexCount());
	if (gs_plane_mesh)    geometryShader->render(renderer->getDeviceContext(), planeMesh->getIndexCount());

	// Render GUI
	gui();
	// Present the rendered scene to the screen.
	renderer->endScene();
}

bool GraphicsApp::render()
{
	// Clear the scene. (default blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	camera->update();

	if (specularLightExample.example) {
		specularLightExample.render(renderer, camera, sphereMesh, textureMgr);
		// Render GUI
		gui();
		
	}
	else if (tessellationExample.example) {
		tessellationExample.render(renderer, camera, terrainMesh, textureMgr);
		// Render GUI
		gui();
	}
	else if (terrainExample.example) {
		terrainExample.render(renderer, camera, terrainMesh, textureMgr);
		// Render GUI
		gui();
	}
	else if (multiLightExample.example) {
		BaseMesh* mesh = nullptr;
		switch (multiLightExample.mesh_choice)
		{
			case MESH::TRIANGLE :
				mesh = triangleMesh;
				break;

			case MESH::SPHERE:
				mesh = sphereMesh;
				break;

			case MESH::CUBE:
				mesh = cubeMesh;
				break;

			case MESH::QUAD:
				mesh = quadMesh;
				break;

			case MESH::PLANE:
				mesh = planeMesh;
				break;
		}

		multiLightExample.render(renderer, camera, mesh, textureMgr);
		// Render GUI
		gui();
	}
	else if (geometry_shader_example) {
		renderGeometryShaderExample();
		// Render GUI
		gui();
	}
	else
	{
		// Render GUI
		gui();
	}

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
		geometry_shader_example = false;

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
		geometry_shader_example = false;

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
		geometry_shader_example = false;

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
		geometry_shader_example = false;

		// set wireframe
		multiLightExample.wireframe = false;
		// set scale
		multiLightExample.scale = XMFLOAT3(1.0f, 1.0f, 20.0f);
		// set sphere mesh
		ml_triangle_mesh = false;
		ml_sphere_mesh = true;
		ml_cube_mesh = false;
		ml_quad_mesh = false;
		ml_plane_mesh = false;
		// set the camera and first mesh
		camera->setPosition(0.0f, 0.0f, -4.75f);
		camera->setRotation(0.0f, 0.f, 0.f);
	}
	// CHOOSE GEOMETRY SHADER EXAMPLE 
	if (ImGui::Button("Geometry Shader Example"))
	{
		specularLightExample.example = false;
		tessellationExample.example = false;
		terrainExample.example = false;
		multiLightExample.example = false;
		geometry_shader_example ^= 1;

		// set wireframe
		gs_wireframe = false;
		// reset geometry shader scale
		gs_scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
		// reset geometry shader wireframe mode
		gs_wireframe = false;
		// reset geometry shader primitive topology
		d3d11_primitive_topology_trianglelist = true;
		d3d11_primitive_topology_pointlist = false;
		// set cube mesh
		gs_triangle_mesh = false;
		gs_sphere_mesh = false;
		gs_cube_mesh = true;
		gs_quad_mesh = false;
		gs_plane_mesh = false;

		// set the camera
		camera->setPosition(13.0f, 4.0f, -22.0f);
		camera->setRotation(0.0f, -30.0f, 5.0f);
	}

	// EXAMPLE WINDOWS //
	// SPECULAR LIGHT EXAMPLE WINDOW
	if (specularLightExample.example)
	{
		ImGui::Begin("Specular Light", &specularLightExample.example);
		if (ImGui::Button("Reset Example"))
		{
			camera->resetCamera();
			specularLightExample.wireframe = false;
		}
		ImGui::Checkbox("Wireframe", &specularLightExample.wireframe);
		ImGui::End();
	}
	// TESSELLATION EXAMPLE WINDOW
	if (tessellationExample.example)
	{
		ImGui::Begin("Tessellation", &tessellationExample.example);
		if (ImGui::Button("Reset Example"))
		{
			// set tessellation camera
			camera->setPosition(0.0f, 4.75f, -10.0f);
			camera->setRotation(0.0f, 30.0f, 0.0f);
			tessellationExample.wireframe = false;
		}
		ImGui::Checkbox("Wireframe", &tessellationExample.wireframe);
		ImGui::End();
	}
	// TERRAIN EXAMPLE WINDOW
	if (terrainExample.example)
	{
		ImGui::Begin("Terrain", &terrainExample.example);
		if (ImGui::Button("Reset Example"))
		{
			// set terrain camera
			camera->setPosition(0.0f, 2.0f, -10.0f);
			camera->setRotation(0.0f, -200.0f, 0.0f);
			// reset terrain scale
			terrainExample.scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
			// reset terrain wireframe mode
			terrainExample.wireframe = false;
		}
		ImGui::Checkbox("Wireframe", &terrainExample.wireframe);
		ImGui::SliderFloat("Scale X", (float*)&terrainExample.scale.x, -15.0f, 15.0f);
		ImGui::SliderFloat("Scale Y", (float*)&terrainExample.scale.y, -15.0f, 15.0f);
		ImGui::SliderFloat("Scale Z", (float*)&terrainExample.scale.z, -15.0f, 15.0f);
		if (ImGui::Button("Reset Scale")) terrainExample.scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
		ImGui::End();
	}
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
			multiLightExample.mesh_choice = MESH::SPHERE;
			ml_triangle_mesh = false;
			ml_sphere_mesh = true;
			ml_cube_mesh = false;
			ml_quad_mesh = false;
			ml_plane_mesh = false;
			// reset wireframe
			multiLightExample.wireframe = false;
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
		// reset scale
		ImGui::SliderFloat3("Scale", (float*)&multiLightExample.scale, -20.0f, 20.0f);
		if (ImGui::Button("Reset Scale")) multiLightExample.scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
		// what mesh to render (the highest one checked will be rendered (room for improvemnet: use menu box instead)
		if (ImGui::Checkbox("Triangle Mesh", &ml_triangle_mesh))
		{
			multiLightExample.mesh_choice = MESH::TRIANGLE;
			//ml_triangle_mesh = true;
			ml_sphere_mesh = false;
			ml_cube_mesh = false;
			ml_quad_mesh = false;
			ml_plane_mesh = false;
		}
		if (ImGui::Checkbox("Sphere Mesh", &ml_sphere_mesh))
		{
			multiLightExample.mesh_choice = MESH::SPHERE;
			ml_triangle_mesh = false;
			//ml_sphere_mesh = true;
			ml_cube_mesh = false;
			ml_quad_mesh = false;
			ml_plane_mesh = false;
		}
		if (ImGui::Checkbox("Cube Mesh", &ml_cube_mesh))
		{
			multiLightExample.mesh_choice = MESH::CUBE;
			ml_triangle_mesh = false;
			ml_sphere_mesh = false;
			//ml_cube_mesh = true;
			ml_quad_mesh = false;
			ml_plane_mesh = false;
		}
		if (ImGui::Checkbox("Quad Mesh", &ml_quad_mesh))
		{
			multiLightExample.mesh_choice = MESH::QUAD;
			ml_triangle_mesh = false;
			ml_sphere_mesh = false;
			ml_cube_mesh = false;
			//ml_quad_mesh = true;
			ml_plane_mesh = false;
		}
		if (ImGui::Checkbox("Plane Mesh", &ml_plane_mesh))
		{
			multiLightExample.mesh_choice = MESH::PLANE;
			ml_triangle_mesh = false;
			ml_sphere_mesh = false;
			ml_cube_mesh = false;
			ml_quad_mesh = false;
			//ml_plane_mesh = true;
			camera->setPosition(0.0f, 3.0f, 0.0f);
			multiLightExample.scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
		}
		ImGui::End();
	}
	// GEOMETRY SHADER EXAMPLE WINDOW
	if (geometry_shader_example)
	{
		ImGui::Begin("Geometry Shader Example", &geometry_shader_example);
		if (ImGui::Button("Reset Example"))
		{
			// reset geometry shader scale
			gs_scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
			// reset geometry shader wireframe mode
			gs_wireframe = false;
			// reset geometry shader primitive topology
			d3d11_primitive_topology_trianglelist = true;
			d3d11_primitive_topology_pointlist = false;
			// set cube mesh
			gs_triangle_mesh = false;
			gs_sphere_mesh = false;
			gs_cube_mesh = true;
			gs_quad_mesh = false;
			gs_plane_mesh = false;
			// set the camera
			camera->setPosition(13.0f, 4.0f, -22.0f);
			camera->setRotation(0.0f, -35.0f, 0.0f);
		}
		ImGui::Checkbox("Wireframe", &gs_wireframe);
		ImGui::SliderFloat3("Scale", (float*)&gs_scale, -10.0f, 10.0f);
		if (ImGui::Button("Reset Scale")) gs_scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
		if (ImGui::Checkbox("Primitive Topology Trianglelist", &d3d11_primitive_topology_trianglelist))
			d3d11_primitive_topology_pointlist = false;
		if (ImGui::Checkbox("Primitive Topology Pointlist", &d3d11_primitive_topology_pointlist))
			d3d11_primitive_topology_trianglelist = false;

		// what mesh to render (the highest one checked will be rendered (room for improvemnet: use menu box instead)
		if (ImGui::Checkbox("Triangle Mesh", &gs_triangle_mesh))
		{
			//gs_triangle_mesh = true;
			gs_sphere_mesh = false;
			gs_cube_mesh = false;
			gs_quad_mesh = false;
			gs_plane_mesh = false;
		}
		if (ImGui::Checkbox("Sphere Mesh", &gs_sphere_mesh))
		{
			gs_triangle_mesh = false;
			//gs_sphere_mesh = true;
			gs_cube_mesh = false;
			gs_quad_mesh = false;
			gs_plane_mesh = false;
		}
		if (ImGui::Checkbox("Cube Mesh", &gs_cube_mesh))
		{
			gs_triangle_mesh = false;
			gs_sphere_mesh = false;
			//gs_cube_mesh = true;
			gs_quad_mesh = false;
			gs_plane_mesh = false;
		}
		if (ImGui::Checkbox("Quad Mesh", &gs_quad_mesh))
		{
			gs_triangle_mesh = false;
			gs_sphere_mesh = false;
			gs_cube_mesh = false;
			//gs_quad_mesh = true;
			gs_plane_mesh = false;
		}
		if (ImGui::Checkbox("Plane Mesh", &gs_plane_mesh))
		{
			gs_triangle_mesh = false;
			gs_sphere_mesh = false;
			gs_cube_mesh = false;
			gs_quad_mesh = false;
			//gs_plane_mesh = true;
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

