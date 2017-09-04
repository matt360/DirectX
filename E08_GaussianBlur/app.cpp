// Lab1.cpp
// Lab 1 example, simple textured quad
#include "App.h"

App1::App1()
{
	mesh = nullptr;
	lightShader = nullptr;
}

void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in)
{
	// Call super init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in);

	// Create Mesh object
	mesh = new CubeMesh(renderer->getDevice(), renderer->getDeviceContext());
	textureMgr->loadTexture("brick", L"../res/brick1.dds");

	// Create light shader object
	//m_Shader = new VertexManipulationShader(m_Direct3D->GetDevice(), hwnd);
	lightShader = new LightShader(renderer->getDevice(), hwnd);
	textureShader = new TextureShader(renderer->getDevice(), hwnd);
	horizontalBlurShader = new HorizontalBlurShader(renderer->getDevice(), hwnd);
	verticalBlurShader = new VerticalBlurShader(renderer->getDevice(), hwnd);

	// Create light source
	// White light point 0.5 along the x-axis and 0.5 along the z-axis
	light = new Light;
	light->setAmbientColour(0.1f, 0.1f, 0.1f, 1.0f);
	light->setDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
	light->setDirection(0.f, 0.f, 1.0f);

	camera->setPosition(0.f, 0.f, -4.f);

	renderTexture = new RenderTexture(renderer->getDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);
	downSampleTexture = new RenderTexture(renderer->getDevice(), screenWidth / 2, screenHeight / 2, SCREEN_NEAR, SCREEN_DEPTH);
	horizontalBlurTexture = new RenderTexture(renderer->getDevice(), screenWidth / 2, screenHeight / 2, SCREEN_NEAR, SCREEN_DEPTH);
	verticalBlurTexture = new RenderTexture(renderer->getDevice(), screenWidth / 2, screenHeight / 2, SCREEN_NEAR, SCREEN_DEPTH);
	upSampleTexture = new RenderTexture(renderer->getDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);

	downSampleMesh = new OrthoMesh(renderer->getDevice(), renderer->getDeviceContext(), screenWidth / 2, screenHeight / 2, 0, 0);
	upSampleMesh = new OrthoMesh(renderer->getDevice(), renderer->getDeviceContext(), screenWidth, screenHeight, 0, 0);

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
	// Render scene to texture / render target
	renderToTexture();

	// Down sample prior to blurring, optimise
	downSample();

	// Apply horizontal blur stage
	horizontalBlur();

	// Apply vertical blur to the horizontal blur stage
	verticalBlur();

	// Up sample, return to screen size before outputting to screen
	upSample();

	// Render scene to buffer
	renderScene();

	return true;
}

void App1::renderToTexture()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	// Set the render target to be the render to texture.
	renderTexture->setRenderTarget(renderer->getDeviceContext());

	// Clear the render to texture.
	renderTexture->clearRenderTarget(renderer->getDeviceContext(), 0.0f, 0.0f, 1.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	camera->update();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	worldMatrix = renderer->getWorldMatrix();
	viewMatrix = camera->getViewMatrix();
	projectionMatrix = renderer->getProjectionMatrix();

	worldMatrix = XMMatrixRotationY(35.f);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	mesh->sendData(renderer->getDeviceContext());
	lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("brick"), light);
	// Render object (combination of mesh geometry and shader process
	lightShader->render(renderer->getDeviceContext(), mesh->getIndexCount());

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	renderer->setBackBufferRenderTarget();
}

void App1::downSample()
{
	XMMATRIX worldMatrix, baseViewMatrix, orthoMatrix;

	// Set the render target to be the render to texture.
	downSampleTexture->setRenderTarget(renderer->getDeviceContext());

	// Clear the render to texture.
	downSampleTexture->clearRenderTarget(renderer->getDeviceContext(), 0.0f, 1.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	camera->update();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	worldMatrix = renderer->getWorldMatrix();
	baseViewMatrix = camera->getOrthoViewMatrix();
	// Get the ortho matrix from the render to texture since texture has different dimensions being that it is smaller.
	orthoMatrix = downSampleTexture->getOrthoMatrix();

	// Turn off the Z buffer to begin all 2D rendering.
	renderer->setZBuffer(false);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	downSampleMesh->sendData(renderer->getDeviceContext());
	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, baseViewMatrix, orthoMatrix, renderTexture->getShaderResourceView());
	// Render object (combination of mesh geometry and shader process
	textureShader->render(renderer->getDeviceContext(), downSampleMesh->getIndexCount());

	// Turn the Z buffer back on now that all 2D rendering has completed.
	renderer->setZBuffer(true);

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	renderer->setBackBufferRenderTarget();
}

void App1::horizontalBlur()
{
	XMMATRIX worldMatrix, baseViewMatrix, orthoMatrix;
	float screenSizeX;

	// Store the screen width in a float that will be used in the horizontal blur shader.
	screenSizeX = (float)horizontalBlurTexture->getTextureWidth();

	// Set the render target to be the render to texture.
	horizontalBlurTexture->setRenderTarget(renderer->getDeviceContext());

	// Clear the render to texture.
	horizontalBlurTexture->clearRenderTarget(renderer->getDeviceContext(), 1.0f, 1.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	camera->update();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	worldMatrix = renderer->getWorldMatrix();
	baseViewMatrix = camera->getOrthoViewMatrix();
	// Get the ortho matrix from the render to texture since texture has different dimensions being that it is smaller.
	orthoMatrix = horizontalBlurTexture->getOrthoMatrix();

	// Turn off the Z buffer to begin all 2D rendering.
	renderer->setZBuffer(false);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	downSampleMesh->sendData(renderer->getDeviceContext());

	horizontalBlurShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, baseViewMatrix, orthoMatrix, downSampleTexture->getShaderResourceView(), screenSizeX);
	// Render object (combination of mesh geometry and shader process
	horizontalBlurShader->render(renderer->getDeviceContext(), downSampleMesh->getIndexCount());

	// Turn the Z buffer back on now that all 2D rendering has completed.
	renderer->setZBuffer(true);

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	renderer->setBackBufferRenderTarget();
	//renderer->ResetViewport();
}

void App1::verticalBlur()
{
	XMMATRIX worldMatrix, baseViewMatrix, orthoMatrix;
	float screenSizeY;

	// Store the screen width in a float that will be used in the horizontal blur shader.
	screenSizeY = (float)verticalBlurTexture->getTextureHeight();

	// Set the render target to be the render to texture.
	verticalBlurTexture->setRenderTarget(renderer->getDeviceContext());

	// Clear the render to texture.
	verticalBlurTexture->clearRenderTarget(renderer->getDeviceContext(), 0.0f, 1.0f, 1.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	camera->update();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	worldMatrix = renderer->getWorldMatrix();
	baseViewMatrix = camera->getOrthoViewMatrix();
	// Get the ortho matrix from the render to texture since texture has different dimensions being that it is smaller.
	orthoMatrix = verticalBlurTexture->getOrthoMatrix();

	// Turn off the Z buffer to begin all 2D rendering.
	renderer->setZBuffer(false);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	downSampleMesh->sendData(renderer->getDeviceContext());
	verticalBlurShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, baseViewMatrix, orthoMatrix, horizontalBlurTexture->getShaderResourceView(), screenSizeY);
	// Render object (combination of mesh geometry and shader process
	verticalBlurShader->render(renderer->getDeviceContext(), downSampleMesh->getIndexCount());

	// Turn the Z buffer back on now that all 2D rendering has completed.
	renderer->setZBuffer(true);

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	renderer->setBackBufferRenderTarget();
}

void App1::upSample()
{
	XMMATRIX worldMatrix, baseViewMatrix, orthoMatrix;

	// Set the render target to be the render to texture.
	upSampleTexture->setRenderTarget(renderer->getDeviceContext());

	// Clear the render to texture.
	upSampleTexture->clearRenderTarget(renderer->getDeviceContext(), 1.0f, 0.0f, 1.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	camera->update();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	worldMatrix = renderer->getWorldMatrix();
	baseViewMatrix = camera->getOrthoViewMatrix();
	// Get the ortho matrix from the render to texture since texture has different dimensions being that it is smaller.
	orthoMatrix = upSampleTexture->getOrthoMatrix();

	// Turn off the Z buffer to begin all 2D rendering.
	renderer->setZBuffer(false);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	upSampleMesh->sendData(renderer->getDeviceContext());
	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, baseViewMatrix, orthoMatrix, verticalBlurTexture->getShaderResourceView());
	// Render object (combination of mesh geometry and shader process
	textureShader->render(renderer->getDeviceContext(), upSampleMesh->getIndexCount());

	// Turn the Z buffer back on now that all 2D rendering has completed.
	renderer->setZBuffer(true);

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	renderer->setBackBufferRenderTarget();
}

void App1::renderScene()
{
	XMMATRIX worldMatrix, baseViewMatrix, orthoMatrix;

	// Clear the scene. (default blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	// Generate the view matrix based on the camera's position.
	camera->update();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	worldMatrix = renderer->getWorldMatrix();
	baseViewMatrix = camera->getOrthoViewMatrix();
	orthoMatrix = renderer->getOrthoMatrix();	// ortho matrix for 2D rendering

												// To render ortho mesh
												// Turn off the Z buffer to begin all 2D rendering.
	renderer->setZBuffer(false);

	upSampleMesh->sendData(renderer->getDeviceContext());
	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, baseViewMatrix, orthoMatrix, upSampleTexture->getShaderResourceView());
	textureShader->render(renderer->getDeviceContext(), upSampleMesh->getIndexCount());

	renderer->setZBuffer(true);

	// Present the rendered scene to the screen.
	renderer->endScene();
}
