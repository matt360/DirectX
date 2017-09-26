// Lab1.cpp
// Lab 1 example, simple coloured triangle mesh
#include "App1.h"

App1::App1()
{
	//BaseApplication::BaseApplication();
	triangleMesh = nullptr;
	sphereMesh = nullptr;
	quadMesh = nullptr;
	lightShader = nullptr;
	colourShader = nullptr;
}

void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in)
{
	// Call super/parent init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in);

	textureMgr->loadTexture("default", L"../res/DefaultDiffuse.png");

	// Create Mesh object
	triangleMesh = new TriangleMesh(renderer->getDevice(), renderer->getDeviceContext());

	sphereMesh = new SphereMesh(renderer->getDevice(), renderer->getDeviceContext());

	quadMesh = new QuadMesh(renderer->getDevice(), renderer->getDeviceContext());

	//colourShader = new ColourShader(renderer->getDevice(), hwnd);

	lightShader = new LightShader(renderer->getDevice(), hwnd);

	initLight();
}

void App1::initLight()
{
	m_Light = new Light;
	m_Light->setAmbientColour(0.0f, 0.0f, 0.0f, 1.0f);
	m_Light->setDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->setDirection(0.0, 0.0f, 0.0f);
	m_Light->setSpecularPower(16.f);
	m_Light->setSpecularColour(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->setPosition(0.0f, 1.0f, 0.0f);
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

	if (quadMesh)
	{
		delete quadMesh;
		quadMesh = 0;
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
}


bool App1::frame()
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

bool App1::render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	//// Clear the scene. (default blue colour)
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
	XMMATRIX matrixTranslation = XMMatrixTranslation(0.0f, 0.0, 0.0f);
	XMMATRIX matrixRotation = XMMatrixRotationX(XMConvertToRadians(90.0f));
	worldMatrix = XMMatrixMultiply(matrixRotation, matrixTranslation);
	// scaling
	XMMATRIX matrixScaling = XMMatrixScaling(3.0f, 1.0f, 3.0f);
	worldMatrix *= matrixScaling;

	//// Send geometry data (from mesh)
	//triangleMesh->sendData(renderer->getDeviceContext());
	//sphereMesh->sendData(renderer->getDeviceContext());
	quadMesh->sendData(renderer->getDeviceContext());

	//// Set shader parameters (matrices and texture)
	//lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("default"), m_Light, camera);
	lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("default"), m_Light, camera);
	//// Render object (combination of mesh geometry and shader process
	//lightShader->render(renderer->getDeviceContext(), sphereMesh->getIndexCount());
	lightShader->render(renderer->getDeviceContext(), quadMesh->getIndexCount());

	// Render GUI
	gui();

	//// Present the rendered scene to the screen.
	renderer->endScene();

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

