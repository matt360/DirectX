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

	geometryShader = nullptr;

	light = nullptr;

	renderTexture = nullptr;
	orthoMesh = nullptr;
}

void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in)
{
	// Call super/parent init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in);

	// Create Mesh object
	triangleMesh = new TriangleMesh(renderer->getDevice(), renderer->getDeviceContext());

	sphereMesh = new SphereMesh(renderer->getDevice(), renderer->getDeviceContext());

	cubeMesh = new CubeMesh(renderer->getDevice(), renderer->getDeviceContext());

	quadMesh = new QuadMesh(renderer->getDevice(), renderer->getDeviceContext());

	planeMesh = new PlaneMesh(renderer->getDevice(), renderer->getDeviceContext());

	geometryShader = new GeometryShader(renderer->getDevice(), hwnd);

	// Create light source (for normal scene rendering)
	initLight();

	textureMgr->loadTexture("brick", L"../res/brick1.dds");
	textureMgr->loadTexture("bunny", L"../res/bunny.png");
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

	if (geometryShader)
	{
		delete geometryShader;
		geometryShader = 0;
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

	// input
	if (input->isLeftMouseDown())
	{
		wireframe = !wireframe;
		input->setLeftMouse(false);
	}

	if (input->isKeyDown(VK_RETURN))
	{
		wireframe = !wireframe;
		input->SetKeyUp(VK_RETURN);
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
	worldMatrix = renderer->getWorldMatrix();

	XMMATRIX matrix1Translation = DirectX::XMMatrixTranslation(2.0f, 0.0f, 0.0f);
	//XMMATRIX matrix1Rotation = DirectX::XMMatrixRotationZ(light_y);
	XMMATRIX matrix1Rotation = DirectX::XMMatrixRotationZ(0.0f);
	// orbit
	//worldMatrix = XMMatrixMultiply(matrix1Translation, matrix1Rotation);
	// translate and rotate
	worldMatrix = XMMatrixMultiply(matrix1Rotation, matrix1Translation);

	// scaling
	XMMATRIX matrix1Scaling = DirectX::XMMatrixScaling(2.0f, 2.0f, 1.0f);
	worldMatrix *= matrix1Scaling;

	viewMatrix = camera->getViewMatrix();

	projectionMatrix = renderer->getProjectionMatrix();

	// wireframe mode
	renderer->setWireframeMode(wireframe);
	
	// ONE TRIANGLE
	//triangleMesh->sendData(renderer->getDeviceContext(), D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// THREE TRIANGLES
	//triangleMesh->sendData(renderer->getDeviceContext(), D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	// *NOTE* because we're offsetting from the normals there's going to be SIX cubes around the original cube
	cubeMesh->sendData(renderer->getDeviceContext(), D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//quadMesh->sendData(renderer->getDeviceContext()); // set input data in the shader programme
	//planeMesh->sendData(renderer->getDeviceContext()); // set input data in the shader programme

	// Set shader parameters (matrices and texture)
	geometryShader->setShaderParameters(renderer->getDeviceContext(),
		worldMatrix, viewMatrix, projectionMatrix,
		textureMgr->getTexture("brick"));
	// Render object (combination of mesh geometry and shader process
	//geometryShader->render(renderer->getDeviceContext(), triangleMesh->getIndexCount());
	geometryShader->render(renderer->getDeviceContext(), cubeMesh->getIndexCount());
	//geometryShader->render(renderer->getDeviceContext(), quadMesh->getIndexCount());
	//geometryShader->render(renderer->getDeviceContext(), planeMesh->getIndexCount());

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

