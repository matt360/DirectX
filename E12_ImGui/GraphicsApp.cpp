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
	terrainShader = nullptr;
	multiLightShader = nullptr;
	light0_ = nullptr; 
	light1_ = nullptr;
	light2_ = nullptr;
	light3_ = nullptr;
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
	if (lightShader)
	{
		delete lightShader;
		lightShader = 0;
	}

	if (tessellationShader)
	{
		delete tessellationShader;
		tessellationShader = 0;
	}

	if (terrainShader)
	{
		delete terrainShader;
		terrainShader = 0;
	}

	// multi lights shader handler
	if (multiLightShader)
	{
		delete multiLightShader;
		multiLightShader = 0;
	}

	if (light0_)
	{
		delete light0_;
		light0_ = 0;
	}

	if (light1_)
	{
		delete light1_;
		light1_ = 0;
	}

	if (light2_)
	{
		delete light2_;
		light2_ = 0;
	}

	if (light3_)
	{
		delete light3_;
		light3_ = 0;
	}
}

void GraphicsApp::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in)
{
	// Call super init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in);

	initVariables();
	// Create mesh objects
	initGeometry();
	// create shader handlers
	initShaders(hwnd);
	initLight();
	loadTextures();
	initGuiVariables();
}

void GraphicsApp::initVariables()
{
	light_terrain_y = 0.0f;
}

void GraphicsApp::initLight()
{
	// specular light
	specular_light = new Light;
	specular_light->setAmbientColour(0.5f, 0.5f, 0.5f, 1.0f);
	specular_light->setDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
	specular_light->setDirection(0.5, -0.5f, 0.0f);
	specular_light->setSpecularPower(16.f);
	specular_light->setSpecularColour(1.0f, 1.0f, 1.0f, 1.0f);

	// terrain light
	light_terrain = new Light;
	light_terrain->setAmbientColour(0.5f, 0.5f, 0.5f, 1.0f);
	light_terrain->setDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
	light_terrain->setDirection(0.0, 0.0f, 0.0f);
	light_terrain->setSpecularPower(16.f);
	light_terrain->setSpecularColour(1.0f, 1.0f, 1.0f, 1.0f);
	light_terrain->setPosition(0.0f, 0.1f, 0.0f);

	// Multiple lights
	// Light 0
	light0_ = new Light;
	light0_->setDiffuseColour(1.0f, 0.0f, 0.0f, 1.0f);
	light0_->setPosition(-3.0f, 0.1f, 3.0f);
	//light0_->setAmbientColour(0.2f, 0.0f, 0.0f, 1.0f); // red
	//light0_->setDirection(0.0, 0.0f, 0.0f);
	//light0_->setSpecularPower(16.f);
	//light0_->setSpecularColour(1.0f, 1.0f, 1.0f, 1.0f);
	// Light 1
	light1_ = new Light;
	light1_->setDiffuseColour(0.0f, 1.0f, 0.0f, 1.0f);
	light1_->setPosition(3.0f, 0.1f, 3.0f);
	//light1_->setAmbientColour(0.0f, 2.0f, 0.0f, 1.0f); // green
	//light1_->setDirection(0.0, 0.0f, 0.0f);
	//light1_->setSpecularPower(16.f);
	//light1_->setSpecularColour(1.0f, 1.0f, 1.0f, 1.0f);
	// Light 2
	light2_ = new Light;
	light2_->setDiffuseColour(0.0f, 0.0f, 1.0f, 1.0f);
	light2_->setPosition(-3.0f, 0.1f, -3.0f);
	//light2_->setAmbientColour(0.0f, 0.0f, 0.2f, 1.0f); // blue
	//light2_->setDirection(0.0, 0.0f, 0.0f);
	//light2_->setSpecularPower(16.f);
	//light2_->setSpecularColour(1.0f, 1.0f, 1.0f, 1.0f);
	// Light 3
	light3_ = new Light;
	light3_->setDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
	light3_->setPosition(3.0f, 0.1f, -3.0f);
	//light3_->setAmbientColour(0.2f, 0.2f, 0.0f, 1.0f); // yellow
	//light3_->setDirection(0.0, 0.0f, 0.0f);
	//light3_->setSpecularPower(16.f);
	//light3_->setSpecularColour(1.0f, 1.0f, 1.0f, 1.0f);
	// light y position
}

void GraphicsApp::loadTextures()
{
	// load textures
	textureMgr->loadTexture("brick", L"../res/brick1.dds");
	textureMgr->loadTexture("bunny", L"../res/bunny.png");
	textureMgr->loadTexture("default", L"../res/DefaultDiffuse.png");
	textureMgr->loadTexture("height", L"../res/height.png");
}

void GraphicsApp::initGeometry()
{
	triangleMesh = new TriangleMesh(renderer->getDevice(), renderer->getDeviceContext());
	sphereMesh = new SphereMesh(renderer->getDevice(), renderer->getDeviceContext());
	cubeMesh = new CubeMesh(renderer->getDevice(), renderer->getDeviceContext());
	quadMesh = new QuadMesh(renderer->getDevice(), renderer->getDeviceContext());
	planeMesh = new PlaneMesh(renderer->getDevice(), renderer->getDeviceContext());
	terrainMesh = new TerrainMesh(renderer->getDevice(), renderer->getDeviceContext(), 100, 200);
}

void GraphicsApp::initShaders(HWND hwnd)
{
	tessellationShader = new TessellationShader(renderer->getDevice(), hwnd);
	lightShader = new LightShader(renderer->getDevice(), hwnd);
	terrainShader = new TerrainShader(renderer->getDevice(), hwnd);
	multiLightShader = new MultiLightShader(renderer->getDevice(), hwnd);
}

void GraphicsApp::initGuiVariables()
{
	// set colour variable for UI controls.
	clear_col = ImColor(114, 144, 154);

	// wireframe for each of the examples
	specular_light_wireframe = false;
	tessellation_wireframe = false;
	terrain_wireframe = false;
	multi_light_wireframe = false;

	// display example handlers
	specular_light_example = false;
	tessellation_example = false;
	terrain_example = false;
	multi_light_example = false;
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
	light_terrain_y += XM_PIDIV2 * timer->getTime();
	light_terrain_y = fmodf(light_terrain_y, XM_2PI);

	// Render the graphics.
	result = render();
	if (!result)
	{
		return false;
	}

	return true;
}

void GraphicsApp::renderSpecularLightExample()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	//// Clear the scene. (default blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	camera->update();

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
	lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("default"), specular_light, camera);
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

	camera->update();

	/// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	worldMatrix = renderer->getWorldMatrix();
	// Generate the view matrix based on the camera's position.
	viewMatrix = camera->getViewMatrix();
	projectionMatrix = renderer->getProjectionMatrix();

	// wireframe mode
	renderer->setWireframeMode(tessellation_wireframe);


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

void GraphicsApp::renderTerrainExample()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	//// Clear the scene. (default cornflower blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	//// Generate the view matrix based on the camera's position.
	camera->update();

	//// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	viewMatrix = camera->getViewMatrix();

	projectionMatrix = renderer->getProjectionMatrix();

	// translation and rotation
	worldMatrix = renderer->getWorldMatrix();
	XMMATRIX matrixTranslation = XMMatrixTranslation(0.0f, 0.0, 0.0f);
	XMMATRIX matrixRotation = XMMatrixRotationX(XMConvertToRadians(180.0f));
	worldMatrix = XMMatrixMultiply(matrixRotation, matrixTranslation);
	// scaling
	XMMATRIX matrixScaling = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	worldMatrix *= matrixScaling;

	// TODO remove m_Light->setPosition(0.0f, 0.0f, 0.0f);

	// wave's:
	float height = 1.0f;
	float frequency = 1.0f;

	// wireframe mode
	renderer->setWireframeMode(terrain_wireframe);

	//m_Light->setPosition(0.0f, sinf(light_y * 3.0f), 0.0f);
	//// Send geometry data (from mesh)
	//triangleMesh->sendData(renderer->getDeviceContext());
	//sphereMesh->sendData(renderer->getDeviceContext());
	//cubeMesh->sendData(renderer->getDeviceContext());
	//quadMesh->sendData(renderer->getDeviceContext()); // set input data in the shader programme
	//planeMesh->sendData(renderer->getDeviceContext()); // set input data in the shader programme
	terrainMesh->sendData(renderer->getDeviceContext(), D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//// Set shader parameters (matrices and texture)
	//lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("default"), m_Light);
	terrainShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("height"), light_terrain, light_terrain_y, height, frequency);

	//// Render object (combination of mesh geometry and shader process
	//lightShader->render(renderer->getDeviceContext(), triangleMesh->getIndexCount()); // output data from the shader programme
	//lightShader->render(renderer->getDeviceContext(), sphereMesh->getIndexCount()); // output data from the shader programme
	//lightShader->render(renderer->getDeviceContext(), cubeMesh->getIndexCount()); // output data from the shader programme
	//lightShader->render(renderer->getDeviceContext(), quadMesh->getIndexCount()); // output data from the shader programme
	//lightShader->render(renderer->getDeviceContext(), planeMesh->getIndexCount()); // output data from the shader programme
	terrainShader->render(renderer->getDeviceContext(), terrainMesh->getIndexCount());

	// Render GUI
	gui();

	//// Present the rendered scene to the screen.
	renderer->endScene();
}

void GraphicsApp::renderMultiLightExample()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	XMFLOAT4 diffuseColor[4];
	XMFLOAT3 lightPosition[4];

	// Create the diffuse color array from the four light colors.
	diffuseColor[0] = light0_->getDiffuseColour();
	diffuseColor[1] = light1_->getDiffuseColour();
	diffuseColor[2] = light2_->getDiffuseColour();
	diffuseColor[3] = light3_->getDiffuseColour();

	// Create the light position array from the four light positions.
	lightPosition[0] = light0_->getPosition();
	lightPosition[1] = light1_->getPosition();
	lightPosition[2] = light2_->getPosition();
	lightPosition[3] = light3_->getPosition();

	//// Clear the scene. (default cornflower blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	//// Generate the view matrix based on the camera's position.
	camera->update();

	//// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	viewMatrix = camera->getViewMatrix();

	projectionMatrix = renderer->getProjectionMatrix();

	// wireframe mode
	renderer->setWireframeMode(false);

	// translation and rotation
	worldMatrix = renderer->getWorldMatrix();
	//XMMATRIX matrixTranslation = XMMatrixTranslation(-100.0f, 0.0, -150.0f);
	//XMMATRIX matrixTranslation = XMMatrixTranslation(0.0f, 0.0, 0.0f);
	//XMMATRIX matrixRotation = XMMatrixRotationX(XMConvertToRadians(90.0f));
	//worldMatrix = XMMatrixMultiply(matrixRotation, matrixTranslation);
	// scaling
	//XMMATRIX matrixScaling = XMMatrixScaling(10.0f, 1.0f, 10.0f);
	//worldMatrix *= matrixScaling;

	// Send geometry data (from mesh)
	//triangleMesh->sendData(renderer->getDeviceContext());
	//sphereMesh->sendData(renderer->getDeviceContext());
	cubeMesh->sendData(renderer->getDeviceContext());
	//quadMesh->sendData(renderer->getDeviceContext());
	//planeMesh->sendData(renderer->getDeviceContext());

	// Set shader parameters (matrices and texture)
	//lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("default"), m_Light, camera);
	multiLightShader->setShaderParameters
	(
		renderer->getDeviceContext(),
		worldMatrix,
		viewMatrix,
		projectionMatrix,
		textureMgr->getTexture("checkerboard"), // for the default textrue pass an empty string as a name
		diffuseColor,
		lightPosition
	);

	// Render object (combination of mesh geometry and shader process
	//lightShader->render(renderer->getDeviceContext(), sphereMesh->getIndexCount());
	lightShader->render(renderer->getDeviceContext(), cubeMesh->getIndexCount());
	//lightShader->render(renderer->getDeviceContext(), planeMesh->getIndexCount());
	//lightShader->render(renderer->getDeviceContext(), quadMesh->getIndexCount());
	//lightShader->render(renderer->getDeviceContext(), planeMesh->getIndexCount());

	// PLANE MESH
	//planeMesh->sendData(renderer->getDeviceContext());
	//lightShader->setShaderParameters
	//(
	//	renderer->getDeviceContext(),
	//	worldMatrix,
	//	viewMatrix,
	//	projectionMatrix,
	//	textureMgr->getTexture("checkerboard"), // for the default textrue pass an empty string as a name
	//	diffuseColor,
	//	lightPosition
	//);
	//lightShader->render(renderer->getDeviceContext(), planeMesh->getIndexCount());

	// Render GUI
	gui();

	//// Present the rendered scene to the screen.
	renderer->endScene();
}

bool GraphicsApp::render()
{
	if (specular_light_example) renderSpecularLightExample();
	else if (tessellation_example) renderTessellationExample();
	else if (terrain_example) renderTerrainExample();
	else if (multi_light_example) renderMultiLightExample();
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
		tessellation_example = false;
		terrain_example = false;
		multi_light_example = false;
		specular_light_example ^= 1;
	}
	// Buttons
	if (ImGui::Button("Tessellation Example"))
	{
		specular_light_example = false;
		terrain_example = false;
		multi_light_example = false;
		tessellation_example ^= 1;
	}
	if (ImGui::Button("Terrain Example"))
	{
		specular_light_example = false;
		tessellation_example = false;
		multi_light_example = false;
		terrain_example ^= 1;
	}
	if (ImGui::Button("Multi Light Example"))
	{
		specular_light_example = false;
		tessellation_example = false;
		terrain_example = false;
		multi_light_example ^= 1;
	}

	// Handle displaying the example
	if (specular_light_example)
	{
		ImGui::Begin("Specular Light", &specular_light_example);
		//ImGui::ColorEdit3("Colour", (float*)&clear_col);
		ImGui::Checkbox("Wireframe", &specular_light_wireframe);
		ImGui::End();
	}
	if (tessellation_example)
	{
		ImGui::Begin("Tessellation", &tessellation_example);
		//ImGui::ColorEdit3("Colour", (float*)&clear_col);
		ImGui::Checkbox("Wireframe", &tessellation_wireframe);
		ImGui::End();
	}
	if (terrain_example)
	{
		ImGui::Begin("Terrain", &terrain_example);
		//ImGui::ColorEdit3("Colour", (float*)&clear_col);
		ImGui::Checkbox("Wireframe", &terrain_wireframe);
		ImGui::End();
	}
	if (multi_light_example)
	{
		ImGui::Begin("Multi Light Example", &multi_light_example);
		//ImGui::ColorEdit3("Colour", (float*)&clear_col);
		ImGui::Checkbox("Wireframe", &multi_light_example);
		ImGui::End();
	}

	// Render UI
	ImGui::Render();
}

float GraphicsApp::clamp(float n, float lower, float upper)
{
	return std::fmax(lower, (std::fmin(n, upper)));
}

