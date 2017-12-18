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
	//specularLightShader = nullptr;
	tessellationShader = nullptr;
	terrainShader = nullptr;
	multiLightShader = nullptr;
	geometryShader = nullptr;

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
	/*if (specularLightShader)
	{
		delete specularLightShader;
		specularLightShader = 0;
	}*/
	specularLightExample.~SpecularLightExample();

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

	if (geometryShader)
	{
		delete geometryShader;
		geometryShader = 0;
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
	//// specular light example
	//specular_light = new Light;
	//specular_light->setAmbientColour(0.5f, 0.5f, 0.5f, 1.0f);
	//specular_light->setDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
	//specular_light->setDirection(0.5, -0.5f, 0.0f);
	//specular_light->setSpecularPower(16.f);
	//specular_light->setSpecularColour(1.0f, 1.0f, 1.0f, 1.0f);

	// terrain light example
	light_terrain = new Light;
	light_terrain->setAmbientColour(0.5f, 0.5f, 0.5f, 1.0f);
	light_terrain->setDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
	light_terrain->setDirection(0.0, 0.0f, 0.0f);
	light_terrain->setSpecularPower(16.f);
	light_terrain->setSpecularColour(1.0f, 1.0f, 1.0f, 1.0f);
	light_terrain->setPosition(0.0f, 0.1f, 0.0f);

	// multiple lights example
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
void GraphicsApp::initShaders(D3D* renderer, HWND hwnd)
{
	tessellationShader = new TessellationShader(renderer->getDevice(), hwnd);
	specularLightExample.specularLightShader = new SpecularLightShader(renderer->getDevice(), hwnd);
	terrainShader = new TerrainShader(renderer->getDevice(), hwnd);
	multiLightShader = new MultiLightShader(renderer->getDevice(), hwnd);
	geometryShader = new GeometryShader(renderer->getDevice(), hwnd);
}

void GraphicsApp::initGuiVariables()
{
	// multi light example lights' colours
	light0_col = ImColor(1.0f, 0.0f, 0.0f, 1.0f);
	light1_col = ImColor(0.0f, 1.0f, 0.0f, 1.0f);
	light2_col = ImColor(0.0f, 0.0f, 1.0f, 1.0f);
	light3_col = ImColor(1.0f, 1.0f, 1.0f, 1.0f);
	// multi light example lights' positions
	light0_pos = XMFLOAT3(-3.0f, 0.1f, 3.0f);
	light1_pos = XMFLOAT3(3.0f, 0.1f, 3.0f);
	light2_pos = XMFLOAT3(-3.0f, 0.1f, -3.0f);
	light3_pos = XMFLOAT3(3.0f, 0.1f, -3.0f);

	// multi light exmaple scale
	ml_scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	// terrain exmaple scale
	tr_scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	// geometry shader exmaple scale 
	gs_scale = XMFLOAT3(1.0f, 1.0f, 1.0f);

	// geomatry shader topology handler (set to triangle list by default)
	d3d11_primitive_topology_trianglelist = true;
}

void GraphicsApp::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in)
{
	// Call super init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in);

	f_gui = &GraphicsApp::gui;

	initVariables();
	initLight();
	loadTextures();
	initGeometry();
	initShaders(renderer, hwnd);
	specularLightExample.init(renderer, hwnd);
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

//void GraphicsApp::renderSpecularLightExample()
//{
//	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
//
//	// Clear the scene. (default blue colour)
//	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);
//
//	camera->update();
//
//	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
//	worldMatrix = renderer->getWorldMatrix();
//	// Generate the view matrix based on the camera's position.
//	viewMatrix = camera->getViewMatrix();
//	projectionMatrix = renderer->getProjectionMatrix();
//
//	// wireframe mode
//	renderer->setWireframeMode(specular_light_wireframe);
//
//
//	// Send geometry data (from mesh)
//	//mesh->sendData(renderer->getDeviceContext());
//	sphereMesh->sendData(renderer->getDeviceContext(), D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//	// Set shader parameters (matrices and texture)
//	specularLightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("default"), specular_light, camera);
//	// Render object (combination of mesh geometry and shader process
//	specularLightShader->render(renderer->getDeviceContext(), sphereMesh->getIndexCount());
//
//
//	// Render GUI
//	gui();
//	// Present the rendered scene to the screen.
//	renderer->endScene();
//}

void GraphicsApp::renderTessellationExample()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	// Clear the scene. (default blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	camera->update();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
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

	// Clear the scene. (default cornflower blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	// Generate the view matrix based on the camera's position.
	camera->update();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	viewMatrix = camera->getViewMatrix();

	projectionMatrix = renderer->getProjectionMatrix();

	// translation and rotation
	worldMatrix = renderer->getWorldMatrix();
	XMMATRIX matrixTranslation = XMMatrixTranslation(-20.0f, 0.0, 0.0f);
	XMMATRIX matrixRotation = XMMatrixRotationX(XMConvertToRadians(180.0f));
	worldMatrix = XMMatrixMultiply(matrixRotation, matrixTranslation);
	// scaling
	XMMATRIX matrixScaling = XMMatrixScaling(tr_scale.x, tr_scale.y, tr_scale.z);
	worldMatrix *= matrixScaling;

	// wave's:
	float height = 1.0f;
	float frequency = 1.0f;

	// wireframe mode
	renderer->setWireframeMode(terrain_wireframe);

	//m_Light->setPosition(0.0f, sinf(light_y * 3.0f), 0.0f);
	// Send geometry data (from mesh)
	//triangleMesh->sendData(renderer->getDeviceContext());
	//sphereMesh->sendData(renderer->getDeviceContext());
	//cubeMesh->sendData(renderer->getDeviceContext());
	//quadMesh->sendData(renderer->getDeviceContext()); // set input data in the shader programme
	//planeMesh->sendData(renderer->getDeviceContext()); // set input data in the shader programme
	terrainMesh->sendData(renderer->getDeviceContext(), D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Set shader parameters (matrices and texture)
	//terrainShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("default"), m_Light);
	terrainShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("height"), light_terrain, light_terrain_y, height, frequency);

	// Render object (combination of mesh geometry and shader process
	//terrainShader->render(renderer->getDeviceContext(), triangleMesh->getIndexCount()); // output data from the shader programme
	//terrainShader->render(renderer->getDeviceContext(), sphereMesh->getIndexCount()); // output data from the shader programme
	//terrainShader->render(renderer->getDeviceContext(), cubeMesh->getIndexCount()); // output data from the shader programme
	//terrainShader->render(renderer->getDeviceContext(), quadMesh->getIndexCount()); // output data from the shader programme
	//terrainShader->render(renderer->getDeviceContext(), planeMesh->getIndexCount()); // output data from the shader programme
	terrainShader->render(renderer->getDeviceContext(), terrainMesh->getIndexCount());

	// Render GUI
	gui();

	// Present the rendered scene to the screen.
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

	// Clear the scene. (default cornflower blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	// Generate the view matrix based on the camera's position.
	camera->update();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
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
	XMMATRIX matrixScaling = XMMatrixScaling(ml_scale.x, ml_scale.y, ml_scale.z);
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

	// Present the rendered scene to the screen.
	renderer->endScene();
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
	if (specularLightExample.specular_light_example) {
		specularLightExample.renderSpecularLightExample(renderer, camera, sphereMesh, textureMgr);
		// Clear the scene. (default blue colour)
		
		// Render GUI
		gui();
		// Present the rendered scene to the screen.
		renderer->endScene();
	}
	else if (tessellation_example) renderTessellationExample();
	else if (terrain_example) renderTerrainExample();
	else if (multi_light_example) renderMultiLightExample();
	else if (geometry_shader_example) renderGeometryShaderExample();
	else
	{
		// Clear the scene. (default blue colour)
		renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);
		// Render GUI
		gui();
		// Present the rendered scene to the screen.
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
		specularLightExample.specular_light_example ^= 1;
		tessellation_example = false;
		terrain_example = false;
		multi_light_example = false;
		geometry_shader_example = false;

		specularLightExample.wireframe = false;
		// set specular light camera
		camera->resetCamera();
	}
	// CHOOSE TESSELLATION EXAMPLE
	if (ImGui::Button("Tessellation Example"))
	{
		specularLightExample.specular_light_example = false;
		tessellation_example ^= 1;
		terrain_example = false;
		multi_light_example = false;
		geometry_shader_example = false;

		tessellation_wireframe = false;
		// set tessellation camera
		camera->setPosition(0.0f, 4.75f, -10.0f);
		camera->setRotation(0.0f, 30.0f, 0.0f);
	}
	// CHOOSE TERRAIN EXAMPLE 
	if (ImGui::Button("Terrain Example"))
	{
		specularLightExample.specular_light_example = false;
		tessellation_example = false;
		terrain_example ^= 1;
		multi_light_example = false;
		geometry_shader_example = false;

		// set terrain camera
		camera->setPosition(0.0f, 2.0f, -10.0f);
		camera->setRotation(0.0f, -200.0f, 0.0f);
		// reset terrain wireframe mode
		terrain_wireframe = false;
	}
	// CHOOSE MULTI LIGHT EXAMPLE
	if (ImGui::Button("Multi Light Example"))
	{
		specularLightExample.specular_light_example = false;
		tessellation_example = false;
		terrain_example = false;
		multi_light_example ^= 1;
		geometry_shader_example = false;

		// set wireframe
		ml_wireframe = false;
		// set scale
		ml_scale = XMFLOAT3(1.0f, 1.0f, 20.0f);
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
		specularLightExample.specular_light_example = false;
		tessellation_example = false;
		terrain_example = false;
		multi_light_example = false;
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
	if (specularLightExample.specular_light_example)
	{
		ImGui::Begin("Specular Light", &specularLightExample.specular_light_example);
		if (ImGui::Button("Reset Example"))
		{
			camera->resetCamera();
			specularLightExample.wireframe = false;
		}
		ImGui::Checkbox("Wireframe", &specularLightExample.wireframe);
		ImGui::End();
	}
	// TESSELLATION EXAMPLE WINDOW
	if (tessellation_example)
	{
		ImGui::Begin("Tessellation", &tessellation_example);
		if (ImGui::Button("Reset Example"))
		{
			// set tessellation camera
			camera->setPosition(0.0f, 4.75f, -10.0f);
			camera->setRotation(0.0f, 30.0f, 0.0f);
			tessellation_wireframe = false;
		}
		ImGui::Checkbox("Wireframe", &tessellation_wireframe);
		ImGui::End();
	}
	// TERRAIN EXAMPLE WINDOW
	if (terrain_example)
	{
		ImGui::Begin("Terrain", &terrain_example);
		if (ImGui::Button("Reset Example"))
		{
			// set terrain camera
			camera->setPosition(0.0f, 2.0f, -10.0f);
			camera->setRotation(0.0f, -200.0f, 0.0f);
			// reset terrain scale
			tr_scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
			// reset terrain wireframe mode
			terrain_wireframe = false;
		}
		ImGui::Checkbox("Wireframe", &terrain_wireframe);
		ImGui::SliderFloat("Scale X", (float*)&tr_scale.x, -15.0f, 15.0f);
		ImGui::SliderFloat("Scale Y", (float*)&tr_scale.y, -15.0f, 15.0f);
		ImGui::SliderFloat("Scale Z", (float*)&tr_scale.z, -15.0f, 15.0f);
		if (ImGui::Button("Reset Scale")) tr_scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
		ImGui::End();
	}
	// MULTI LIGHT EXAMPLE WINDOW
	if (multi_light_example)
	{
		ImGui::Begin("Multi Light Example", &multi_light_example);
		if (ImGui::Button("Reset Example"))
		{
			// set multi light camera
			camera->setPosition(0.0f, 0.0f, -4.75f);
			camera->setRotation(0.0f, 0.f, 0.f);
			// scale up sphere mesh
			ml_scale = XMFLOAT3(1.0f, 1.0f, 20.0f);
			// reset light colours
			light0_col = ImColor(1.0f, 0.0f, 0.0f, 1.0f);
			light1_col = ImColor(0.0f, 1.0f, 0.0f, 1.0f);
			light2_col = ImColor(0.0f, 0.0f, 1.0f, 1.0f);
			light3_col = ImColor(1.0f, 1.0f, 1.0f, 1.0f);
			// reset light positions
			light0_pos = XMFLOAT3(-3.0f, 0.1f, 3.0f);
			light1_pos = XMFLOAT3(3.0f, 0.1f, 3.0f);
			light2_pos = XMFLOAT3(-3.0f, 0.1f, -3.0f);
			light3_pos = XMFLOAT3(3.0f, 0.1f, -3.0f);
			// render only sphere mesh
			ml_triangle_mesh = false;
			ml_sphere_mesh = true;
			ml_cube_mesh = false;
			ml_quad_mesh = false;
			ml_plane_mesh = false;
			// reset wireframe
			ml_wireframe = false;
		}
		ImGui::Checkbox("Wireframe", &ml_wireframe);
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
		// reset scale
		ImGui::SliderFloat3("Scale", (float*)&ml_scale, -20.0f, 20.0f);
		if (ImGui::Button("Reset Scale")) ml_scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
		// what mesh to render (the highest one checked will be rendered (room for improvemnet: use menu box instead)
		if (ImGui::Checkbox("Triangle Mesh", &ml_triangle_mesh))
		{
			//ml_triangle_mesh = true;
			ml_sphere_mesh = false;
			ml_cube_mesh = false;
			ml_quad_mesh = false;
			ml_plane_mesh = false;
		}
		if (ImGui::Checkbox("Sphere Mesh", &ml_sphere_mesh))
		{
			ml_triangle_mesh = false;
			//ml_sphere_mesh = true;
			ml_cube_mesh = false;
			ml_quad_mesh = false;
			ml_plane_mesh = false;
		}
		if (ImGui::Checkbox("Cube Mesh", &ml_cube_mesh))
		{
			ml_triangle_mesh = false;
			ml_sphere_mesh = false;
			//ml_cube_mesh = true;
			ml_quad_mesh = false;
			ml_plane_mesh = false;
		}
		if (ImGui::Checkbox("Quad Mesh", &ml_quad_mesh))
		{
			ml_triangle_mesh = false;
			ml_sphere_mesh = false;
			ml_cube_mesh = false;
			//ml_quad_mesh = true;
			ml_plane_mesh = false;
		}
		if (ImGui::Checkbox("Plane Mesh", &ml_plane_mesh))
		{
			ml_triangle_mesh = false;
			ml_sphere_mesh = false;
			ml_cube_mesh = false;
			ml_quad_mesh = false;
			//ml_plane_mesh = true;
			camera->setPosition(0.0f, 3.0f, 0.0f);
			ml_scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
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

