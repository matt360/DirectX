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

	textureMgr->loadTexture("brick", L"../res/brick1.dds");

	// Create Mesh object
	//triangleMesh = new TriangleMesh(renderer->getDevice(), renderer->getDeviceContext());
	sphereMesh = new SphereMesh(renderer->getDevice(), renderer->getDeviceContext());
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

// First render the scene to a render texture
/*
The first function performs the first step of the algorithm by 
rendering the scene to a full screen sized render to texture.
*/
void App1::RenderSceneToTexture(float time)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	// Set the render target to be the render to texture.
	// setRenderTarget function sets the render target view in this class as the current rendering location for all graphics to be rendered to.
	renderTexture->setRenderTarget(renderer->getDeviceContext());

	// Clear the render to texture. 
	// clearRenderTarget mimics the functionality of the D3DClass::BeginScene function 
	renderTexture->clearRenderTarget(renderer->getDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	camera->update();

	// Get the view, world, and projection matrices from the camera and d3d objects.
	viewMatrix = camera->getViewMatrix();
	worldMatrix = renderer->getWorldMatrix();
	projectionMatrix = renderer->getProjectionMatrix();

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	cubeMesh->sendData(renderer->getDeviceContext());
	lightShader->setShaderParameters
	(
		renderer->getDeviceContext(), 
		worldMatrix, viewMatrix, projectionMatrix, 
		textureMgr->getTexture("brick"), 
		light,
		time
	);
	// Render object (combination of mesh geometry and shader process
	lightShader->render(renderer->getDeviceContext(), cubeMesh->getIndexCount());

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	renderer->setBackBufferRenderTarget();

	// Reset the viewport back to the original.
	renderer->resetViewport();
}

// Next down sample the render texture to a smaller sized texture
/*
The second function performs the next step of the algorithm by 
rendering the full screen render to texture, down to
a smaller window (down sampling) which was 
defined as half the size of the screen in the Init function. 
Notice also that when we get the projection matrix it is
now an ortho matrix from the render to texture with
smaller dimensions.
*/
void App1::DownSampleTexture()
{
	XMMATRIX worldMatrix, viewMatrix, orthoMatrix;

	// Set the render target to be the render to texture.
	// setRenderTarget function sets the render target view in this class as the current rendering location for all graphics to be rendered to.
	downSampleTexture->setRenderTarget(renderer->getDeviceContext());

	// Clear the render to texture. 
	// clearRenderTarget mimics the functionality of the D3DClass::BeginScene function 
	downSampleTexture->clearRenderTarget(renderer->getDeviceContext(), 0.0f, 1.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position
	camera->update();

	// Get the view and world matrices from the camera and d3d objects.
	viewMatrix = camera->getViewMatrix();
	worldMatrix = renderer->getWorldMatrix();
	// Get the ortho matrix from the renderer to texture since texture has different dimensions being that it is smaller
	orthoMatrix = downSampleTexture->getOrthoMatrix();

	// Turn off the Z buffer to begin all the 2D rendering
	renderer->setZBuffer(false);

	// Put the small ortho window vertex and index buffers on the graphics pipeline to prepare them for drawing
	smallWindow->sendData(renderer->getDeviceContext());
	// Render the small ortho window using the texture shader and the render to texture of the scene as the texture resource
	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, orthoMatrix, renderTexture->getShaderResourceView());
	// Render object (combination of mesh geometry and shader process)
	textureShader->render(renderer->getDeviceContext(), smallWindow->getIndexCount());

	// Turn the Z buffer back on now that all the 2D rendering has completer
	renderer->setZBuffer(true);

	// Reset the render target back to the original back buffer and not the render to texture anymore
	renderer->setBackBufferRenderTarget();

	// Reset the viewport back to the original 
	renderer->resetViewport();
}

// Perform a horizontal blur on the down sampled render texture
/*
The third function performs the horizontal blur on the down sampled texture and
stores the result in separate render to texture object which
will be used as input to the vertical blur shader.
*/
void App1::RenderHorizontalBlurToTexture()
{
	XMMATRIX worldMatrix, viewMatrix, orthoMatrix;
	float screenSizeX;

	// Store the screen width in a float that will be used in the horizontal blur shader
	screenSizeX = (float)horizontalBlurTexture->getTextureWidth();

	// Set the render target to be the render to texture
	// setRenderTarget function sets the render target view in this class as the current rendering location for all graphics to be rendered to.
	horizontalBlurTexture->setRenderTarget(renderer->getDeviceContext());

	// Clear the render to texture
	// clearRenderTarget mimics the functionality of the D3DClass::BeginScene function 
	horizontalBlurTexture->clearRenderTarget(renderer->getDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position
	camera->update();

	// Get the view and world matrices from the camera and d3d objects.
	viewMatrix = camera->getViewMatrix();
	worldMatrix = renderer->getWorldMatrix();

	// Get the ortho matrix from the render to texture since texture has different dimensions
	orthoMatrix = horizontalBlurTexture->getOrthoMatrix();

	// Turn off the Z buffer to begin all 2D rendering
	renderer->setZBuffer(false);

	// Put the small ortho window vertex and index buffers on the graphics pipeline to prepare them for drawing 
	smallWindow->sendData(renderer->getDeviceContext());
	// Render the small ortho window using the horizontal blur shader and the down sampled render to texture resource
	horizontalBlurShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, orthoMatrix, 
		downSampleTexture->getShaderResourceView(), screenSizeX);
	horizontalBlurShader->render(renderer->getDeviceContext(), smallWindow->getIndexCount());

	// Turn the Z buffer back on now that all 2D rendering has completed.
	renderer->setZBuffer(true);

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	renderer->setBackBufferRenderTarget();

	// Reset the viewport back to the original.
	renderer->resetViewport();
}

// Now perform a vertical blur on the horizontal blur render texture
/*
The fourth function performs the vertical blur on 
the horizontally blurred render to texture. 
The result is stored in yet another render to texture object.
*/
void App1::RenderVerticalBlurToTexture()
{
	XMMATRIX worldMatrix, viewMatrix, orthoMatrix;
	float screenSizeY;

	// Store the screen height in a float that will be used in the vertical blur shader.
	screenSizeY = (float)verticalBlurTexture->getTextureHeight();

	// Set the render target to be render to texture
	// setRenderTarget function sets the render target view in this class as the current rendering location for all the graphics to be renderer to
	verticalBlurTexture->setRenderTarget(renderer->getDeviceContext());

	// Clear render to texture
	// clearRenderTarget mimics the functionality of the D3D::BeginScene funciton
	verticalBlurTexture->clearRenderTarget(renderer->getDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	camera->update();

	// Get the view and world matrices from the camera and d3d objects.
	viewMatrix = camera->getViewMatrix();
	worldMatrix = renderer->getWorldMatrix();

	// Get the ortho matrix from the render to texture since texture has different dimensions.
	orthoMatrix = verticalBlurTexture->getOrthoMatrix();

	// Turn off the Z buffer to begin all 2D rendering.
	renderer->setZBuffer(false);

	// Put the small ortho window vertex and index buffers on the graphics pipeline to prepare them for drawing.
	smallWindow->sendData(renderer->getDeviceContext());
	// Render the small ortho window using the vertical blur shader and the horizontal blurred render to texture resource.
	verticalBlurShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, orthoMatrix,
		horizontalBlurTexture->getShaderResourceView(), screenSizeY);
	verticalBlurShader->render(renderer->getDeviceContext(), smallWindow->getIndexCount());

	// Turn the Z buffer back on now that all 2D rendering has completed.
	renderer->setZBuffer(true);

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	renderer->setBackBufferRenderTarget();

	// Reset the viewport back to the original.
	renderer->resetViewport();
}

// Up sample the final blurred render texture to the screen size again
/*
The fifth function performs the up sampling of 
the small horizontally and vertically blurred texture. 
The up sample is done by just rendering the small blurred texture to 
a full screen 2D window model.
The result of this is rendered to another render to texture object called
upSampleTexture
*/
void App1::UpSampleTexture()
{
	XMMATRIX worldMatrix, viewMatrix, orthoMatrix;

	// Set the render target to be the render to texture.
	upSampleTexture->setRenderTarget(renderer->getDeviceContext());

	// Clear the render to texture.
	upSampleTexture->clearRenderTarget(renderer->getDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	camera->update();

	// Get the view and world matrices from the camera and d3d objects.
	viewMatrix = camera->getViewMatrix();
	worldMatrix = renderer->getWorldMatrix();

	// Get the ortho matrix from the render to texture since texture has different dimensions.
	orthoMatrix = upSampleTexture->getOrthoMatrix();

	// Turn off the Z buffer to begin all 2D rendering.
	renderer->setZBuffer(false);

	// Put the full screen ortho window vertex and index buffers on the graphics pipeline to prepare them for drawing.
	smallWindow->sendData(renderer->getDeviceContext());
	// Render the full screen ortho window using the texture shader and the small sized final blurred render to texture resource.
	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, orthoMatrix,
		verticalBlurTexture->getShaderResourceView());
	textureShader->render(renderer->getDeviceContext(), smallWindow->getIndexCount());

	// Turn the Z buffer back on now that all 2D rendering has completed.
	renderer->setZBuffer(true);

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	renderer->setBackBufferRenderTarget();

	// Reset the viewport back to the original.
	renderer->resetViewport();
}

// Render the blurred up sampled render texture to the screen
/*
The sixth and final function draws the up sampled blurred texture to
the full screen completing the full screen blur effect.
*/
void App1::Render2DTextureScene(float time)
{
	XMMATRIX worldMatrix, viewMatrix, orthoMatrix;

	// Clear the scene. (default blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	// Generate the view matrix based on the camera's position.
	camera->update();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	viewMatrix = camera->getViewMatrix();
	worldMatrix = renderer->getWorldMatrix();
	orthoMatrix = renderer->getProjectionMatrix();

	// Turn off the Z buffer to begin all 2D rendering
	renderer->setZBuffer(false);
	// ortho matrix for 2D rendering
	orthoMatrix = renderer->getOrthoMatrix();

	// Put the full screen ortho window vertex and index buffers on the graphics pipeline to prepare them for drawing.
	fullScreenWindow->sendData(renderer->getDeviceContext());
	// Render the full screen ortho window using the texture shader and the full screen sized blurred render to texture resource
	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, orthoMatrix,
		upSampleTexture->getShaderResourceView());
	textureShader->render(renderer->getDeviceContext(), fullScreenWindow->getIndexCount());

	// Turn the Z buffer back on now that all 2D rendering has completed
	renderer->setZBuffer(true);

	// Render GUI
	gui();

	// Present the rendered scene to the screen
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
	RenderSceneToTexture(light_y);
	DownSampleTexture();
	RenderHorizontalBlurToTexture();
	RenderVerticalBlurToTexture();
	UpSampleTexture();
	// ...then render it again to the back buffer
	Render2DTextureScene(light_y);

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

