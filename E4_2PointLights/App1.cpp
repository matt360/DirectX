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

	initVariables();

	initTextures();

	initShaders(hwnd);

	initGeometry();

	initLight();

	initLightSpheres();
}

void App1::initVariables()
{
	light_y = 0.0f;
}

void App1::initTextures() 
{
	textureMgr->loadTexture("checkerboard", L"../res/checkerboard.png");
}

void App1::initShaders(HWND hwnd)
{
	//colourShader = new ColourShader(renderer->getDevice(), hwnd);

	lightShader = new LightShader(renderer->getDevice(), hwnd);
}

void App1::initGeometry()
{
	// Create Mesh object
	//triangleMesh = new TriangleMesh(renderer->getDevice(), renderer->getDeviceContext());

	sphereMesh = new SphereMesh(renderer->getDevice(), renderer->getDeviceContext(), 5);

	cubeMesh = new CubeMesh(renderer->getDevice(), renderer->getDeviceContext());

	quadMesh = new QuadMesh(renderer->getDevice(), renderer->getDeviceContext());

	planeMesh = new PlaneMesh(renderer->getDevice(), renderer->getDeviceContext());
}

void App1::initLight()
{
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

void App1::initLightSpheres()
{
	lightSphere0_ = new SphereMesh(renderer->getDevice(), renderer->getDeviceContext(), 5);

	lightSphere1_ = new SphereMesh(renderer->getDevice(), renderer->getDeviceContext(), 5);

	lightSphere2_ = new SphereMesh(renderer->getDevice(), renderer->getDeviceContext(), 5);

	lightSphere3_ = new SphereMesh(renderer->getDevice(), renderer->getDeviceContext(), 5);
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

	if (lightSphere0_)
	{
		delete lightSphere0_;
		lightSphere0_ = 0;
	}
	
	if (lightSphere1_)
	{
		delete lightSphere1_;
		lightSphere1_ = 0;
	}

	if (lightSphere2_)
	{
		delete lightSphere2_;
		lightSphere2_ = 0;
	}

	if (lightSphere3_)
	{
		delete lightSphere3_;
		lightSphere3_ = 0;
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

	m_Light->setPosition(0.f, 100 + (100.f * sinf(a)), 0.f);
	*/

	light_y += XM_PIDIV2 * timer->getTime();
	light_y = fmodf(light_y, XM_2PI);

	light3_->setPosition(light_y, 1.0f, -3.0f);

	// Render the graphics.
	result = render();
	if (!result)
	{
		return false;
	}

	return true;
}

void App1::sendGeometryData()
{
	//triangleMesh->sendData(renderer->getDeviceContext());
	//sphereMesh->sendData(renderer->getDeviceContext());
	cubeMesh->sendData(renderer->getDeviceContext());
	//quadMesh->sendData(renderer->getDeviceContext());
	//planeMesh->sendData(renderer->getDeviceContext());


}

void App1::renderObjects()
{
	//lightShader->render(renderer->getDeviceContext(), sphereMesh->getIndexCount());
	lightShader->render(renderer->getDeviceContext(), cubeMesh->getIndexCount());
	//lightShader->render(renderer->getDeviceContext(), quadMesh->getIndexCount());
	//lightShader->render(renderer->getDeviceContext(), planeMesh->getIndexCount());
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

bool App1::render()
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
	//XMMATRIX matrixTranslation = XMMatrixTranslation(-50.0f, 0.0, -50.0f);
	//XMMATRIX matrixTranslation = XMMatrixTranslation(0.0f, 0.0, 0.0f);
	//XMMATRIX matrixRotation = XMMatrixRotationX(XMConvertToRadians(90.0f));
	//worldMatrix = XMMatrixMultiply(matrixRotation, matrixTranslation);
	// scaling
	//XMMATRIX matrixScaling = XMMatrixScaling(10.0f, 1.0f, 10.0f);
	//worldMatrix *= matrixScaling;

	// Send geometry data (from mesh)
	sendGeometryData();

	// Set shader parameters (matrices and texture)
	//lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("default"), m_Light, camera);
	lightShader->setShaderParameters
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
	renderObjects();

	// Render GUI
	gui();

	//// Present the rendered scene to the screen.
	renderer->endScene();

	return true;
}