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
	horizontalBlurShader = nullptr;
	verticalBlurShader = nullptr;

	renderTexture = nullptr;
	downSampleTexture = nullptr; 
	horizontalBlurTexture = nullptr;
	verticalBlurTexture = nullptr;
	upSampleTexture = nullptr;

	orthoMesh = nullptr;
	smallWindow = nullptr; 
	fullScreenWindow = nullptr;

	light = nullptr;
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

	// Shader handlers
	lightShader = new LightShader(renderer->getDevice(), hwnd);
	textureShader = new TextureShader(renderer->getDevice(), hwnd);

	// Create light source (for normal scene rendering)
	initLight();

	// Size of the down-sized texture to render to
	int downSampleWidth, downSampleHeight;
	downSampleWidth = screenWidth / 2;
	downSampleHeight = screenHeight / 2;

	// Blur shaders
	// Horizontal Blur Shader
	horizontalBlurShader = new HorizontalBlurShader(renderer->getDevice(), hwnd);
	// Vertical Blur Shader
	verticalBlurShader = new VerticalBlurShader(renderer->getDevice(), hwnd);

	// Render to texture
	// RenderTexture, OrthoMesh and shader set for different renderTarget
	renderTexture = new RenderTexture(renderer->getDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);
	downSampleTexture = new RenderTexture(renderer->getDevice(), downSampleWidth, downSampleHeight, SCREEN_NEAR, SCREEN_DEPTH);
	horizontalBlurTexture = new RenderTexture(renderer->getDevice(), downSampleWidth, downSampleHeight, SCREEN_NEAR, SCREEN_DEPTH);
	verticalBlurTexture = new RenderTexture(renderer->getDevice(), downSampleWidth, downSampleHeight, SCREEN_NEAR, SCREEN_DEPTH);
	upSampleTexture = new RenderTexture(renderer->getDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);

	// ortho size and position set based on window size
	// 200x200 pixels (standard would be matching window size for fullscreen mesh
	// Position default at 0x0 centre window, to offset change values (pixel)
	orthoMesh = new OrthoMesh(renderer->getDevice(), renderer->getDeviceContext(), 200, 150, -412, 225);
	smallWindow = new OrthoMesh(renderer->getDevice(), renderer->getDeviceContext(), downSampleWidth, downSampleHeight);
	fullScreenWindow = new OrthoMesh(renderer->getDevice(), renderer->getDeviceContext(), screenWidth, screenHeight);
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

	if (downSampleTexture)
	{
		delete downSampleTexture;
		downSampleTexture = 0;
	}

	if (horizontalBlurTexture)
	{
		delete horizontalBlurTexture;
		horizontalBlurTexture = 0;
	}

	if (verticalBlurTexture)
	{
		delete verticalBlurTexture;
		verticalBlurTexture = 0;
	}

	if (upSampleTexture)
	{
		delete upSampleTexture;
		upSampleTexture = 0;
	}

	if (orthoMesh)
	{
		delete orthoMesh;
		orthoMesh = 0;
	}

	if (smallWindow)
	{
		delete smallWindow;
		smallWindow = 0;
	}

	if (fullScreenWindow)
	{
		delete fullScreenWindow;
		fullScreenWindow = 0;
	}
}

// First render a scene to a render texture
void App1::RenderSceneToTexture(float time)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	// Set the render target to be the render to texture.
	renderTexture->setRenderTarget(renderer->getDeviceContext());

	// Clear the render to texture.
	renderTexture->clearRenderTarget(renderer->getDeviceContext(), 0.0f, 1.0f, 1.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	camera->update();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	worldMatrix = renderer->getWorldMatrix();
	viewMatrix = camera->getViewMatrix();
	projectionMatrix = renderer->getProjectionMatrix();

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	cubeMesh->sendData(renderer->getDeviceContext());
	lightShader->setShaderParameters
	(
		renderer->getDeviceContext(), 
		worldMatrix, viewMatrix, projectionMatrix, 
		textureMgr->getTexture("default"), 
		light, 
		time
	);
	// Render object (combination of mesh geometry and shader process
	lightShader->render(renderer->getDeviceContext(), cubeMesh->getIndexCount());

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	renderer->setBackBufferRenderTarget();
}

// Next down sample	the render texture to a smaller sized texture
void App1::DownSampleTexture()
{
}

// Perform a horizontal blur on the down sampled render texture
void App1::RenderHorizontalBlurToTexture()
{
}

// Now perform a vertical blur on the horizontal blur render texture
void App1::RenderVerticalBlurToTexture()
{
}

// Up sample the final blurred render texture to screen size again
void App1::UpSampleTexture()
{
}

// Render the blurred up sampled render texture to the screen
void App1::Render2DTextureScene(float time)
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
	lightShader->setShaderParameters
	(
		renderer->getDeviceContext(), 
		worldMatrix, viewMatrix, projectionMatrix, 
		textureMgr->getTexture("default"), 
		light, 
		time
	);
	// Render object (combination of mesh geometry and shader process
	lightShader->render(renderer->getDeviceContext(), cubeMesh->getIndexCount());


	// Render to ortho mesh
	// Turn off the Z buffer to begin all 2D rendering. //////////////////////////
	renderer->setZBuffer(false);
	// ortho matrix for 2D rendering
	orthoMatrix = renderer->getOrthoMatrix();
	orthoViewMatrix = camera->getOrthoViewMatrix();

	// // Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
	orthoMesh->sendData(renderer->getDeviceContext());
	// Once the vertex/index buffers are prepared we draw them using the texture shader. 
	// Notice we send in the orthoMatrix instead of the projectionMatrix for rendering 2D. 
	// Due note also that if your view matrix is changing you will need to create a default one for
	// 2D rendering and use it instead of the regular view matrix. 
	// In this tutorial using the regular view matrix is fine as the camera in this tutorial is stationary.
	textureShader->setShaderParameters
	(
		renderer->getDeviceContext(), 
		worldMatrix, orthoViewMatrix, orthoMatrix,
		renderTexture->getShaderResourceView()
	);
	textureShader->render(renderer->getDeviceContext(), orthoMesh->getIndexCount());
	// After all the 2D rendering is done we turn the Z buffer back on for the next round of 3D rendering.
	// Enable Z buffering after rendering //////////////////////////////////////////
	renderer->setZBuffer(true);

	// Render GUI
	gui();

	// Present the rendered scene to the screen.
	renderer->endScene();
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
	// render it normally to the texture...
	RenderToTexture(light_y);
	// ...then render it again to the back buffer
	RenderScene(light_y);

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

