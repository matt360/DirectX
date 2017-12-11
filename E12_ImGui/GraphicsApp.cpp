// Lab1.cpp
// Lab 1 example, simple coloured triangle mesh
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
	lightShader = nullptr;
	tessellationShader = nullptr;

	// cameras
	camera_specular_light_position_set = true;
	camera_tessellation_position_set = true;
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

	// shader handlers
	if (tessellationShader)
	{
		delete tessellationShader;
		tessellationShader = 0;
	}

	if (lightShader)
	{
		delete lightShader;
		lightShader = 0;
	}
}

void GraphicsApp::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in)
{
	// Call super init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in);

	// Create mesh objects
	triangleMesh = new TriangleMesh(renderer->getDevice(), renderer->getDeviceContext());
	sphereMesh = new SphereMesh(renderer->getDevice(), renderer->getDeviceContext());
	cubeMesh = new CubeMesh(renderer->getDevice(), renderer->getDeviceContext());
	quadMesh = new QuadMesh(renderer->getDevice(), renderer->getDeviceContext());
	planeMesh = new PlaneMesh(renderer->getDevice(), renderer->getDeviceContext());
	terrainMesh = new TerrainMesh(renderer->getDevice(), renderer->getDeviceContext(), 100, 200);

	// create shader handlers
	tessellationShader = new TessellationShader(renderer->getDevice(), hwnd);
	lightShader = new LightShader(renderer->getDevice(), hwnd);

	initLight();
	loadTextures();
	initGui();
}

void GraphicsApp::initLight()
{
	light = new Light;
	light->setAmbientColour(0.5f, 0.5f, 0.5f, 1.0f);
	light->setDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
	light->setDirection(0.5, -0.5f, 0.0f);
	light->setSpecularPower(16.f);
	light->setSpecularColour(1.0f, 1.0f, 1.0f, 1.0f);
}

void GraphicsApp::loadTextures()
{
	// load textures
	textureMgr->loadTexture("brick", L"../res/brick1.dds");
	textureMgr->loadTexture("bunny", L"../res/bunny.png");
	textureMgr->loadTexture("default", L"../res/DefaultDiffuse.png");
}

void GraphicsApp::initGui()
{
	// set colour variable for UI controls.
	clear_col = ImColor(114, 144, 154);
	specular_light_wireframe = false;
	tessellation_wireframe = false;
	triangle_colour_shader = false;
	tessellation_shader = false;
}

void GraphicsApp::renderSpecularLightExample()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	//// Clear the scene. (default blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	if (!camera_specular_light_position_set)
	{
		camera->setPosition(camera_specular_light_last_position);
		camera_specular_light_position_set = true;
	}
	camera->update();
	// remember camera's last position
	camera_specular_light_last_position = camera->getPosition();

	/// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	worldMatrix = renderer->getWorldMatrix();
	// Generate the view matrix based on the camera's position.
	viewMatrix = camera->getViewMatrix();
	projectionMatrix = renderer->getProjectionMatrix();

	// wireframe mode
	renderer->setWireframeMode(specular_light_wireframe);

	// Send geometry data (from mesh)
	//mesh->sendData(renderer->getDeviceContext());
	sphereMesh->sendData(renderer->getDeviceContext(), D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Set shader parameters (matrices and texture)
	lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("default"), light, camera);
	// Render object (combination of mesh geometry and shader process
	lightShader->render(renderer->getDeviceContext(), sphereMesh->getIndexCount());

	// Render GUI
	gui();

	// Present the rendered scene to the screen.
	renderer->endScene();
}

void GraphicsApp::renderTessellationExample()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	// Clear the scene. (default blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	if (!camera_tessellation_position_set)
	{
		camera->setPosition(camera_tessellation_last_position);
		camera_tessellation_position_set = true;
	}
	camera->update();
	// remember camera's last position
	camera_tessellation_last_position = camera->getPosition();
	// wireframe mode
	renderer->setWireframeMode(tessellation_wireframe);

	/// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	worldMatrix = renderer->getWorldMatrix();
	// Generate the view matrix based on the camera's position.
	viewMatrix = camera->getViewMatrix();
	projectionMatrix = renderer->getProjectionMatrix();

	// Send geometry data (from mesh)
	terrainMesh->sendData(renderer->getDeviceContext(), D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
	//sphereMesh->sendData(renderer->getDeviceContext(), D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
	//cubeMesh->sendData(renderer->getDeviceContext(), D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
	//quadMesh->sendData(renderer->getDeviceContext(), D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
	//planeMesh->sendData(renderer->getDeviceContext(), D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

	// Set shader parameters (matrices and texture)
	tessellationShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,
		textureMgr->getTexture("brick"), camera);

	// Render object (combination of mesh geometry and shader process
	tessellationShader->render(renderer->getDeviceContext(), terrainMesh->getIndexCount());
	//tessellationShader->render(renderer->getDeviceContext(), sphereMesh->getIndexCount());
	//tessellationShader->render(renderer->getDeviceContext(), cubeMesh->getIndexCount());
	//tessellationShader->render(renderer->getDeviceContext(), quadMesh->getIndexCount());
	//tessellationShader->render(renderer->getDeviceContext(), planeMesh->getIndexCount());

	// Render GUI
	gui();

	// Present the rendered scene to the screen.
	renderer->endScene();
}

void GraphicsApp::gui()
{
	// Force turn off on Geometry shader and force fill rendering
	renderer->getDeviceContext()->GSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->HSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->DSSetShader(NULL, NULL, 0);
	renderer->setWireframeMode(false);

	// Build UI
	//ImGui::Checkbox("Triangle Colour Shader", &triangle_colour_shader);
	ImGui::Text("FPS: %.2f", timer->getFPS());
	if (ImGui::Button("Specular Light Example"))
	{
		tessellation_shader = false;
		camera_specular_light_position_set = false;
		triangle_colour_shader ^= 1;
	}
	if (ImGui::Button("Tessellation Example"))
	{
		triangle_colour_shader = false;
		camera_tessellation_position_set = false;
		tessellation_shader ^= 1;
	}

	if (triangle_colour_shader)
	{
		ImGui::Begin("Specular Light", &triangle_colour_shader);
		ImGui::ColorEdit3("Colour", (float*)&clear_col);
		ImGui::Checkbox("Wireframe", &specular_light_wireframe);
		ImGui::End();
	}

	if (tessellation_shader)
	{
		ImGui::Begin("Tessellation", &tessellation_shader);
		ImGui::ColorEdit3("Colour", (float*)&clear_col);
		ImGui::Checkbox("Wireframe", &tessellation_wireframe);
		ImGui::End();
	}

	// Render UI
	ImGui::Render();
}

bool GraphicsApp::frame()
{
	bool result;

	result = BaseApplication::frame();
	if (!result)
	{
		return false;
	}

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
	if (triangle_colour_shader) renderSpecularLightExample();
	else if (tessellation_shader) renderTessellationExample();
	else
	{
		//// Clear the scene. (default blue colour)
		renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);
		// Render GUI
		gui();
		//// Present the rendered scene to the screen.
		renderer->endScene();
	}
	return true;
}

float GraphicsApp::clamp(float n, float lower, float upper)
{
	return std::fmax(lower, (std::fmin(n, upper)));
}

