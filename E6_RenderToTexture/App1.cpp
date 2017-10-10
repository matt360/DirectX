// Lab1.cpp
// Lab 1 example, simple coloured triangle mesh
#include "App1.h"

App1::App1()
{
	//BaseApplication::BaseApplication();
	triangleMesh = nullptr;
	sphereMesh = nullptr;
	cubeMesh = nullptr;
	quadMesh = nullptr;
	planeMesh = nullptr;

	colourShader = nullptr;
	lightShader = nullptr;
	textureShader = nullptr;

	light = nullptr;

	renderTexture = nullptr;
	orthoMesh = nullptr;
}

void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in)
{
	// Call super/parent init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in);

	textureMgr->loadTexture("height", L"../res/height.png");

	// Create Mesh object
	//triangleMesh = new TriangleMesh(renderer->getDevice(), renderer->getDeviceContext());

	//sphereMesh = new SphereMesh(renderer->getDevice(), renderer->getDeviceContext());

	cubeMesh = new CubeMesh(renderer->getDevice(), renderer->getDeviceContext());

	//quadMesh = new QuadMesh(renderer->getDevice(), renderer->getDeviceContext());

	//planeMesh = new PlaneMesh(renderer->getDevice(), renderer->getDeviceContext());

	//colourShader = new ColourShader(renderer->getDevice(), hwnd);

	lightShader = new LightShader(renderer->getDevice(), hwnd);

	textureShader = new TextureShader(renderer->getDevice(), hwnd);

	// Create light source (for normal scene rendering)
	initLight();

	// RenderTexture, OrthoMesh and shader set for different renderTarget
	renderTexture = new RenderTexture(renderer->getDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);

	// ortho size and position set based on window size
	// 200x200 pixels (standard would be matching window size for fullscreen mesh
	// Position default at 0x0 centre window, to offset change values (pixel)
	orthoMesh = new OrthoMesh(renderer->getDevice(), renderer->getDeviceContext(), 200, 150, -412, 225);
}

void App1::initLight()
{
	light = new Light;
	light->setAmbientColour(0.5f, 0.5f, 0.5f, 1.0f);
	light->setDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
	light->setDirection(0.0, 0.0f, 0.0f);
	light->setSpecularPower(16.f);
	light->setSpecularColour(1.0f, 1.0f, 1.0f, 1.0f);
	light->setPosition(0.0f, 0.1f, 0.0f);
	light_y = 0.0f;
}

void App1::RenderToTexture()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	// Set the render target to be the render to texture.
	renderTexture->setRenderTarget(renderer->getDeviceContext());

	// Clear the render to texture.
	renderTexture->clearRenderTarget(renderer->getDeviceContext(), 1.0f, 1.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	camera->update();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	worldMatrix = renderer->getWorldMatrix();
	viewMatrix = camera->getViewMatrix();
	projectionMatrix = renderer->getProjectionMatrix();

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	cubeMesh->sendData(renderer->getDeviceContext());

	lightShader->setShaderParameters(renderer->getDeviceContext(), 
		worldMatrix, viewMatrix, projectionMatrix, 
		textureMgr->getTexture("default"), 
		light, timer->getTime());

	// Render object (combination of mesh geometry and shader process
	lightShader->render(renderer->getDeviceContext(), cubeMesh->getIndexCount());

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	renderer->setBackBufferRenderTarget();
}

void App1::RenderScene()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoViewMatrix, orthoMatrix;

	// Clear the scene. (default blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	// Generate the view matrix based on the camera's position.
	camera->update();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	worldMatrix = renderer->getWorldMatrix();
	viewMatrix = camera->getViewMatrix();
	projectionMatrix = renderer->getProjectionMatrix();

	// Send geometry data (from mesh)
	cubeMesh->sendData(renderer->getDeviceContext());
	// Set shader parameters (matrices and texture)

	lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix,
		projectionMatrix, textureMgr->getTexture("default"), light, timer->getTime());

	// Render object (combination of mesh geometry and shader process
	lightShader->render(renderer->getDeviceContext(), cubeMesh->getIndexCount());


	// To render ortho mesh
	// Turn off the Z buffer to begin all 2D rendering.
	renderer->setZBuffer(false);

	// ortho matrix for 2D rendering
	orthoMatrix = renderer->getOrthoMatrix();
	orthoViewMatrix = camera->getOrthoViewMatrix(); // TODO

	orthoMesh->sendData(renderer->getDeviceContext());
	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, orthoViewMatrix, orthoMatrix,
		renderTexture->getShaderResourceView());
	textureShader->render(renderer->getDeviceContext(), orthoMesh->getIndexCount());

	renderer->setZBuffer(true);

	// Render GUI
	//gui();

	gui();
	// Present the rendered scene to the screen.
	renderer->endScene();
}

App1::~App1()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D object.
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

	if (colourShader)
	{
		delete colourShader;
		colourShader = 0;
	}

	if (lightShader)
	{
		delete lightShader;
		lightShader = 0;
	}

	if (textureShader)
	{
		delete textureShader;
		textureShader = 0;
	}

	if (light)
	{
		delete light;
		light = 0;
	}

	if (renderTexture)
	{
		delete renderTexture;
		renderTexture = 0;
	}

	if (orthoMesh)
	{
		delete orthoMesh;
		orthoMesh = 0;
	}
}


bool App1::frame()
{
	bool result;

	result = BaseApplication::frame();
	if (!result)
	{
		return false;
	}

	/*
	static float a = 0.f;

	a += XM_PIDIV2 * timer->getTime();
	a = fmodf(a, XM_2PI);

	light->setPosition(0.f, 100 + (100.f * sinf(a)), 0.f);
	*/

	light_y += XM_PIDIV2 * timer->getTime();
	light_y = fmodf(light_y, XM_2PI);

	// Render the graphics.
	result = render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool App1::render()
{
	//XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	////// Clear the scene. (default cornflower blue colour)
	//renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	////// Generate the view matrix based on the camera's position.
	//camera->update();

	////// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	//viewMatrix = camera->getViewMatrix();

	//projectionMatrix = renderer->getProjectionMatrix();

	//// wireframe mode
	//renderer->setWireframeMode(true);

	//// translation and rotation
	//worldMatrix = renderer->getWorldMatrix();
	//XMMATRIX matrixTranslation = XMMatrixTranslation(0.0f, 0.0, 0.0f);
	//XMMATRIX matrixRotation = XMMatrixRotationX(XMConvertToRadians(180.0f));
	//worldMatrix = XMMatrixMultiply(matrixRotation, matrixTranslation);
	//// scaling
	//XMMATRIX matrixScaling = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	//worldMatrix *= matrixScaling;

	////light->setPosition(0.0f, sinf(light_y * 3.0f), 0.0f);
	////// Send geometry data (from mesh)
	////triangleMesh->sendData(renderer->getDeviceContext());
	////sphereMesh->sendData(renderer->getDeviceContext());
	//cubeMesh->sendData(renderer->getDeviceContext());
	////quadMesh->sendData(renderer->getDeviceContext()); // set input data in the shader programme
	////planeMesh->sendData(renderer->getDeviceContext()); // set input data in the shader programme

	//float height = 1.0f;
	//float frequency = 1.0f;

	////// Set shader parameters (matrices and texture)
	////lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("default"), light);
	//lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("default"), light, light_y, height, frequency);

	////// Render object (combination of mesh geometry and shader process
	////lightShader->render(renderer->getDeviceContext(), triangleMesh->getIndexCount()); // output data from the shader programme
	////lightShader->render(renderer->getDeviceContext(), sphereMesh->getIndexCount()); // output data from the shader programme
	//lightShader->render(renderer->getDeviceContext(), cubeMesh->getIndexCount()); // output data from the shader programme
	////lightShader->render(renderer->getDeviceContext(), quadMesh->getIndexCount()); // output data from the shader programme
	////lightShader->render(renderer->getDeviceContext(), planeMesh->getIndexCount()); // output data from the shader programme

	RenderToTexture();
	RenderScene();

	//// Render GUI
	

	//// Present the rendered scene to the screen.
	//renderer->endScene();

	return true;
}

void App1::gui()
{
	// Force turn off on Geometry shader
	renderer->getDeviceContext()->GSSetShader(NULL, NULL, 0);

	// Build UI
	ImGui::Text("FPS: %.2f", timer->getFPS());

	// Render UI
	ImGui::Render();
}

