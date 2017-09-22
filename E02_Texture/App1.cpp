// Lab1.cpp
// Lab 1 example, simple textured quad
#include "App1.h"

App1::App1()
{
	squareMesh = nullptr;
	mesh = nullptr;
	textureShader = nullptr;
}

void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in)
{
	// Call super init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in);

	// Create Mesh object, load required textures and load shaders.
	/*mesh = new QuadMesh(renderer->getDevice(), renderer->getDeviceContext());
	textureMgr->loadTexture("brick", L"../res/bunny.png");
	textureShader = new TextureShader(renderer->getDevice(), hwnd);*/

	squareMesh = new SquareMesh(renderer->getDevice(), renderer->getDeviceContext());
	textureMgr->loadTexture("brick", L"../res/brick1.dds");
	textureMgr->loadTexture("bunny", L"../res/bunny.png");
	textureShader = new TextureShader(renderer->getDevice(), hwnd);
}


App1::~App1()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D object.
	if (mesh)
	{
		delete mesh;
		mesh = 0;
	}

	if (squareMesh)
	{
		delete squareMesh;
		squareMesh = 0;
	}

	if (textureShader)
	{
		delete textureShader;
		textureShader = 0;
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

	rotate += 0.1f;

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
	worldMatrix = renderer->getWorldMatrix();

	XMMATRIX matrix1Translation = DirectX::XMMatrixTranslation(2.0f, 0.0f, 0.0f);
	XMMATRIX matrix1Rotation = DirectX::XMMatrixRotationZ(rotate);
	// orbit
	//worldMatrix = XMMatrixMultiply(matrix1Translation, matrix1Rotation);
	// translate and rotate
	worldMatrix = XMMatrixMultiply(matrix1Rotation, matrix1Translation);

	XMMATRIX matrix1Scaling = DirectX::XMMatrixScaling(2.0f, 2.0f, 1.0f);
	worldMatrix *= matrix1Scaling;

	viewMatrix = camera->getViewMatrix();

	projectionMatrix = renderer->getProjectionMatrix();

	// wireframe mode
	//renderer->setWireframeMode(true);
	
	//// Send geometry data (from mesh)
	//mesh->sendData(renderer->getDeviceContext());
	////// Set shader parameters (matrices and texture)
	//textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("brick"));
	////// Render object (combination of mesh geometry and shader process
	//textureShader->render(renderer->getDeviceContext(), mesh->getIndexCount());

	// Send geometry data (from mesh)
	squareMesh->sendData(renderer->getDeviceContext());
	//// Set shader parameters (matrices and texture)
	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("brick"));
	//// Render object (combination of mesh geometry and shader process
	textureShader->render(renderer->getDeviceContext(), squareMesh->getIndexCount());

	// not really needed to load identity again
	worldMatrix = renderer->getWorldMatrix();

	XMMATRIX matrix2Translation = DirectX::XMMatrixTranslation(1.0f, 0.0f, 0.0f);
	XMMATRIX matrix2Scaling = DirectX::XMMatrixScaling(2.0f, 2.0f, 1.0f);
	worldMatrix = XMMatrixMultiply(matrix2Translation, matrix2Scaling);
	
	// Send geometry data (from mesh)
	squareMesh->sendData(renderer->getDeviceContext());
	//// Set shader parameters (matrices and texture)
	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("brick"), textureMgr->getTexture("bunny"));
	//// Render object (combination of mesh geometry and shader process
	textureShader->render(renderer->getDeviceContext(), squareMesh->getIndexCount());

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

