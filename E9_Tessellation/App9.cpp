// Lab1.cpp
// Lab 1 example, simple coloured triangle mesh
#include "App9.h"

App9::App9()
{
	//BaseApplication::BaseApplication();

	// shader handlers
	colourShader = nullptr;
	tessellationShader = nullptr;

	// geometry meshes
	triangleMesh = nullptr;
	sphereMesh = nullptr;
	cubeMesh = nullptr;
	quadMesh = nullptr;
	planeMesh = nullptr;
}

void App9::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in)
{
	// Call super/parent init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in);

	wireframe = false;
	tessellationAmount = 4.0f;

	// Create Mesh object
	//triangleMesh = new TriangleMesh(renderer->getDevice(), renderer->getDeviceContext());
	sphereMesh = new SphereMesh(renderer->getDevice(), renderer->getDeviceContext());
	cubeMesh = new CubeMesh(renderer->getDevice(), renderer->getDeviceContext());
	//quadMesh = new QuadMesh(renderer->getDevice(), renderer->getDeviceContext());
	//planeMesh = new PlaneMesh(renderer->getDevice(), renderer->getDeviceContext());
	//colourShader = new ColourShader(renderer->getDevice(), hwnd);

	// shader handlers
	colourShader = new ColourShader(renderer->getDevice(), hwnd);
	tessellationShader = new TessellationShader(renderer->getDevice(), hwnd);

	// load textures
	textureMgr->loadTexture("brick", L"../res/brick1.dds");
	textureMgr->loadTexture("bunny", L"../res/bunny.png");
}


App9::~App9()
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

	if (tessellationShader)
	{
		delete tessellationShader;
		tessellationShader = 0;
	}
}


bool App9::frame()
{
	bool result;

	result = BaseApplication::frame();
	if (!result)
	{
		return false;
	}

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

	if (input->isKeyDown(VK_LEFT))
	{
		tessellationAmount = clamp(tessellationAmount -= 1.0f, 1.0f, 64.0f);
		input->SetKeyUp(VK_LEFT);
	}

	if (input->isKeyDown(VK_RIGHT))
	{
		tessellationAmount = clamp(tessellationAmount += 1.0f, 1.0f, 64.0f);
		input->SetKeyUp(VK_RIGHT);
	}

	// Render the graphics.
	result = render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool App9::render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	//// Clear the scene. (default blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	//// Generate the view matrix based on the camera's position.
	camera->update();

	// wireframe mode
	renderer->setWireframeMode(wireframe);

	//// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	worldMatrix = renderer->getWorldMatrix();
	viewMatrix = camera->getViewMatrix();
	projectionMatrix = renderer->getProjectionMatrix();

	//// Send geometry data (from mesh)
	sphereMesh->sendData(renderer->getDeviceContext(), D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
	//// Set shader parameters (matrices and texture)
	tessellationShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,
		textureMgr->getTexture("brick"), tessellationAmount);
	//// Render object (combination of mesh geometry and shader process
	tessellationShader->render(renderer->getDeviceContext(), sphereMesh->getIndexCount());
	
	// Render GUI
	gui();

	//// Present the rendered scene to the screen.
	renderer->endScene();

	return true;
}

void App9::gui()
{
	// Force turn off on Geometry shader
	renderer->getDeviceContext()->GSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->HSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->DSSetShader(NULL, NULL, 0);

	// Build UI
	ImGui::Text("FPS: %.2f", timer->getFPS());

	// Render UI
	ImGui::Render();
}

float App9::clamp(float n, float lower, float upper)
{
	return std::fmax(lower, (std::fmin(n, upper)));
}

