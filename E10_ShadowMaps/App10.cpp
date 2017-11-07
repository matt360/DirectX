// Lab1.cpp
// Lab 1 example, simple coloured triangle mesh
#include "App10.h"

App10::App10()
{
	//BaseApplication::BaseApplication();
	terrainMesh = nullptr;
}

void App10::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in)
{
	// Call super/parent init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in);

	// Create Mesh object
	terrainMesh = new TerrainMesh(renderer->getDevice(), renderer->getDeviceContext());

	// Shader handlers
	depthShader = new DepthShader(renderer->getDevice(), hwnd);
	shadowShader = new ShadowShader(renderer->getDevice(), hwnd);

	// Light
	initLight();
}

void App10::initLight()
{
	light = new Light;
	light->setAmbientColour(0.5f, 0.5f, 0.5f, 1.0f);
	light->setDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
	light->setDirection(0.5, -0.5f, 0.0f);
	light->setSpecularPower(16.f);
	light->setSpecularColour(1.0f, 1.0f, 1.0f, 1.0f);
}

App10::~App10()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D object.
	if (terrainMesh)
	{
		delete terrainMesh;
		terrainMesh = 0;
	}

	if (depthShader)
	{
		delete depthShader;
		depthShader = 0;
	}

	if (shadowShader)
	{
		delete shadowShader;
		shadowShader = 0;
	}
}


bool App10::frame()
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

bool App10::render()
{
	XMMATRIX worldMatrix, lightViewMatrix, lightProjectionMatrix;

	// Clear the scene. (default blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	// Generate the view matrix based on the camera's position.
	camera->update();

	// Get the world, view and projection matrices from the camera and Direct3D objects.
	//light->generateViewMatrix
	//worldMatrix = renderer->getWorldMatrix();
	//viewMatrix = camera->getViewMatrix();
	//projectionMatrix = renderer->getProjectionMatrix();
	light->generateViewMatrix();
	worldMatrix = renderer->getWorldMatrix();
	lightViewMatrix = light->getViewMatrix();
	lightProjectionMatrix = light->getProjectionMatrix();

	// Render floor
	// Send geometry data (from mesh)
	terrainMesh->sendData(renderer->getDeviceContext());
	// Set shader parameters (matrices and texture)
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	// Render object (combination of mesh geometry and shader process
	depthShader->render(renderer->getDeviceContext(), terrainMesh->getIndexCount());

	// Render GUI
	gui();

	// Present the rendered scene to the screen.
	renderer->endScene();

	return true;
}

void App10::gui()
{
	// Force turn off on Geometry shader
	renderer->getDeviceContext()->GSSetShader(NULL, NULL, 0);

	// Build UI
	ImGui::Text("FPS: %.2f", timer->getFPS());

	// Render UI
	ImGui::Render();
}

