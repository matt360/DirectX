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
	specularLightShader = nullptr;
	tessellationShader = nullptr;
	terrainShader = nullptr;
	multiLightShader = nullptr;

	//light0_ = nullptr; 
	//light1_ = nullptr;
	//light2_ = nullptr;
	//light3_ = nullptr;
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
	if (specularLightShader)
	{
		delete specularLightShader;
		specularLightShader = 0;
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
void GraphicsApp::initShaders(HWND hwnd)
{
	tessellationShader = new TessellationShader(renderer->getDevice(), hwnd);
	specularLightShader = new SpecularLightShader(renderer->getDevice(), hwnd);
	terrainShader = new TerrainShader(renderer->getDevice(), hwnd);
	multiLightShader = new MultiLightShader(renderer->getDevice(), hwnd);
}

void GraphicsApp::initGuiVariables()
{
	// set colour variable for UI controls.
	//light0_->setDiffuseColour(1.0f, 0.0f, 0.0f, 1.0f);
	//light1_->setDiffuseColour(0.0f, 1.0f, 0.0f, 1.0f);
	//light2_->setDiffuseColour(0.0f, 0.0f, 1.0f, 1.0f);
	//light3_->setDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);

	light0_col = ImColor(1.0f, 0.0f, 0.0f, 1.0f);
	light1_col = ImColor(0.0f, 1.0f, 0.0f, 1.0f);
	light2_col = ImColor(0.0f, 0.0f, 1.0f, 1.0f);
	light3_col = ImColor(1.0f, 1.0f, 1.0f, 1.0f);

	light0_pos = XMFLOAT3(-3.0f, 0.1f, 3.0f);
	light1_pos = XMFLOAT3(3.0f, 0.1f, 3.0f);
	light2_pos = XMFLOAT3(-3.0f, 0.1f, -3.0f);
	light3_pos = XMFLOAT3(3.0f, 0.1f, -3.0f);

	scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
}

void GraphicsApp::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in)
{
	// Call super init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in);

	initVariables();
	initLight();
	loadTextures();
	initGeometry();
	initShaders(hwnd);
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
	specularLightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("default"), specular_light, camera);
	// Render object (combination of mesh geometry and shader process
	specularLightShader->render(renderer->getDeviceContext(), sphereMesh->getIndexCount());


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
	//specularLightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("default"), m_Light);
	terrainShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("height"), light_terrain, light_terrain_y, height, frequency);

	//// Render object (combination of mesh geometry and shader process
	//specularLightShader->render(renderer->getDeviceContext(), triangleMesh->getIndexCount()); // output data from the shader programme
	//specularLightShader->render(renderer->getDeviceContext(), sphereMesh->getIndexCount()); // output data from the shader programme
	//specularLightShader->render(renderer->getDeviceContext(), cubeMesh->getIndexCount()); // output data from the shader programme
	//specularLightShader->render(renderer->getDeviceContext(), quadMesh->getIndexCount()); // output data from the shader programme
	//specularLightShader->render(renderer->getDeviceContext(), planeMesh->getIndexCount()); // output data from the shader programme
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
	diffuseColor[0] = XMFLOAT4(light0_col.x, light0_col.y, light0_col.z, light0_col.w);
	diffuseColor[1] = XMFLOAT4(light1_col.x, light1_col.y, light1_col.z, light1_col.w);
	diffuseColor[2] = XMFLOAT4(light2_col.x, light2_col.y, light2_col.z, light2_col.w);
	diffuseColor[3] = XMFLOAT4(light3_col.x, light3_col.y, light3_col.z, light3_col.w);

	// Create the light position array from the four light positions.
	lightPosition[0] = light0_pos;
	lightPosition[1] = light1_pos;
	lightPosition[2] = light2_pos;
	lightPosition[3] = light3_pos;

	//// Clear the scene. (default cornflower blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	//// Generate the view matrix based on the camera's position.
	camera->update();

	//// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	if (ml_plane_mesh)
	{
		viewMatrix = camera->getViewMatrix();
		projectionMatrix = renderer->getProjectionMatrix();
		// translation and rotation
		worldMatrix = renderer->getWorldMatrix();
		XMMATRIX matrixTranslation = XMMatrixTranslation(-40.0f, 0.0, -40.0f);
		//XMMATRIX matrixTranslation = XMMatrixTranslation(0.0f, 0.0, 0.0f);
		XMMATRIX matrixRotation = XMMatrixRotationX(XMConvertToRadians(0.0f));
		worldMatrix = XMMatrixMultiply(matrixRotation, matrixTranslation);
	}
	else if (ml_sphere_mesh)
	{
		viewMatrix = camera->getViewMatrix();
		projectionMatrix = renderer->getProjectionMatrix();
		// translation and rotation
		worldMatrix = renderer->getWorldMatrix();
		XMMATRIX matrixTranslation = XMMatrixTranslation(0.0f, 0.0, 0.0f);
		//XMMATRIX matrixTranslation = XMMatrixTranslation(0.0f, 0.0, 0.0f);
		XMMATRIX matrixRotation = XMMatrixRotationX(XMConvertToRadians(0.0f));
		worldMatrix = XMMatrixMultiply(matrixRotation, matrixTranslation);

		scale = XMFLOAT3(1.0f, 1.0f, 20.0f);
	}
	else
	{
		viewMatrix = camera->getViewMatrix();
		projectionMatrix = renderer->getProjectionMatrix();
		// translation and rotation
		worldMatrix = renderer->getWorldMatrix();
		XMMATRIX matrixTranslation = XMMatrixTranslation(0.0f, 0.0, 0.0f);
		//XMMATRIX matrixTranslation = XMMatrixTranslation(0.0f, 0.0, 0.0f);
		XMMATRIX matrixRotation = XMMatrixRotationX(XMConvertToRadians(0.0f));
		worldMatrix = XMMatrixMultiply(matrixRotation, matrixTranslation);
	}
	// scaling
	XMMATRIX matrixScaling = XMMatrixScaling(scale.x, scale.y, scale.z);
	worldMatrix *= matrixScaling;

	// wireframe mode
	renderer->setWireframeMode(ml_wireframe);

	// Send geometry data (from mesh)
	if (ml_triangle_mesh) triangleMesh->sendData(renderer->getDeviceContext(), D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	if (ml_sphere_mesh)   sphereMesh->sendData(renderer->getDeviceContext(), D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	if (ml_cube_mesh)     cubeMesh->sendData(renderer->getDeviceContext(), D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	if (ml_quad_mesh)     quadMesh->sendData(renderer->getDeviceContext(), D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	if (ml_plane_mesh)    planeMesh->sendData(renderer->getDeviceContext(), D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Set shader parameters (matrices and texture)
	//multiLightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("default"), m_Light, camera);
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
	if (ml_triangle_mesh) multiLightShader->render(renderer->getDeviceContext(), sphereMesh->getIndexCount());
	if (ml_sphere_mesh)   multiLightShader->render(renderer->getDeviceContext(), cubeMesh->getIndexCount());
	if (ml_cube_mesh)     multiLightShader->render(renderer->getDeviceContext(), planeMesh->getIndexCount());
	if (ml_quad_mesh)     multiLightShader->render(renderer->getDeviceContext(), quadMesh->getIndexCount());
	if (ml_plane_mesh)    multiLightShader->render(renderer->getDeviceContext(), planeMesh->getIndexCount());

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
	ImGui::Text("Camera: x: %.2f y: %.2f z: %.2f", camera->getPosition().x, camera->getPosition().y, camera->getPosition().z);
	if (ImGui::Button("Reset camera"))
	{
		camera->setPosition(0.0f, 0.0f, -10.0f);
		camera->setRotation(0.0f, 0.0f, 0.0f);
	}
	if (ImGui::Button("Specular Light Example"))
	{ 
		specular_light_example ^= 1;
		tessellation_example = false;
		terrain_example = false;
		multi_light_example = false;
	}
	// Buttons
	if (ImGui::Button("Tessellation Example"))
	{
		specular_light_example = false;
		tessellation_example ^= 1;
		terrain_example = false;
		multi_light_example = false;
	}
	if (ImGui::Button("Terrain Example"))
	{
		specular_light_example = false;
		tessellation_example = false;
		terrain_example ^= 1;
		multi_light_example = false;
	}
	if (ImGui::Button("Multi Light Example"))
	{
		specular_light_example = false;
		tessellation_example = false;
		terrain_example = false;
		multi_light_example ^= 1;
		camera->setPosition(0.0f, 0.0f, -4.75f);
		ml_sphere_mesh = true;
	}

	// Handle displaying the example
	if (specular_light_example)
	{
		ImGui::Begin("Specular Light", &specular_light_example);
		ImGui::Checkbox("Wireframe", &specular_light_wireframe);
		ImGui::End();
	}
	if (tessellation_example)
	{
		ImGui::Begin("Tessellation", &tessellation_example);
		ImGui::Checkbox("Wireframe", &tessellation_wireframe);
		ImGui::End();
	}
	if (terrain_example)
	{
		ImGui::Begin("Terrain", &terrain_example);
		ImGui::Checkbox("Wireframe", &terrain_wireframe);
		ImGui::End();
	}
	if (multi_light_example)
	{
		ImGui::Begin("Multi Light Example", &multi_light_example);
		// change lights' colour
		ImGui::ColorEdit3("Light 0 Col", (float*)&light0_col);
		ImGui::ColorEdit3("Light 1 Col", (float*)&light1_col);
		ImGui::ColorEdit3("Light 2 Col", (float*)&light2_col);
		ImGui::ColorEdit3("Light 3 Col", (float*)&light3_col);
		// change lights' position
		ImGui::SliderFloat3("Light 0 Pos", (float*)&light0_pos, -10.0f, 10.0f);
		ImGui::SliderFloat3("Light 1 Pos", (float*)&light1_pos, -10.0f, 10.0f);
		ImGui::SliderFloat3("Light 2 Pos", (float*)&light2_pos, -10.0f, 10.0f);
		ImGui::SliderFloat3("Light 3 Pos", (float*)&light3_pos, -10.0f, 10.0f);
		
		ImGui::SliderFloat3("Scale", (float*)&scale, -20.0f, 20.0f);
		ImGui::Checkbox("Triangle Mesh", &ml_triangle_mesh);
		ImGui::Checkbox("Sphere Mesh", &ml_sphere_mesh);
		ImGui::Checkbox("Cube Mesh", &ml_cube_mesh);
		ImGui::Checkbox("Quad Mesh", &ml_quad_mesh);
		ImGui::Checkbox("Plane Mesh", &ml_plane_mesh);
		ImGui::Checkbox("Wireframe", &ml_wireframe);
		ImGui::End();
	}

	// Render UI
	ImGui::Render();
}

float GraphicsApp::clamp(float n, float lower, float upper)
{
	return std::fmax(lower, (std::fmin(n, upper)));
}

