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
	mesh = new PlaneMesh(renderer->getDevice(), renderer->getDeviceContext());
	textureMgr->loadTexture("brick", L"../res/brick1.dds");
	//cubeMesh = new CubeMesh(renderer->getDevice(), renderer->getDeviceContext(), L"../res/defaultDiffuse.png");
	model = new Model(renderer->getDevice(), renderer->getDeviceContext(), "../res/teapot.obj");

	/*m_LightShader = new LightShader(renderer->getDevice(), hwnd);*/
	textureShader = new TextureShader(renderer->getDevice(), hwnd);
	depthShader = new DepthShader(renderer->getDevice(), hwnd);
	shadowShader = new ShadowShader(renderer->getDevice(), hwnd);
	
	// This is your shadow map
	renderTexture = new RenderTexture(renderer->getDevice(), 1024, 1024, SCREEN_NEAR, SCREEN_DEPTH);

	orthoMesh = new OrthoMesh(renderer->getDevice(), renderer->getDeviceContext(), 400, 300);

	light = new Light;
	light->setAmbientColour(0.0f, 0.0f, 0.0f, 1.0f);
	light->setDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
	//light->SetPosition(5.0f, 0.0f, -5.0f);
	// Shadow setup
	light->setLookAt(0.f, 0.f, 0.f);
	light->setPosition(0.f, 10.f, -10.f);
	light->generateProjectionMatrix(SCREEN_NEAR, SCREEN_DEPTH);
	//light->generateOrthoMatrix(screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);
	light->generateOrthoMatrix(100, 100, 1.0f, 100.f);

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
	// Perform depth pass
	depthPass();

	// Render scene
	renderScene();

	return true;
}

void App1::depthPass()
{
	XMMATRIX worldMatrix, lightViewMatrix, lightProjectionMatrix, scaleMatrix;

	// Set the render target to be the render to texture.
	renderTexture->setRenderTarget(renderer->getDeviceContext());

	// Clear the render to texture.
	renderTexture->clearRenderTarget(renderer->getDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);
	

	// get the world, view, and projection matrices from the camera and d3d objects.
	light->generateViewMatrix();
	lightViewMatrix = light->getViewMatrix();
	//lightProjectionMatrix = light->getProjectionMatrix();
	lightProjectionMatrix = light->getOrthoMatrix();
	worldMatrix = renderer->getWorldMatrix();

	worldMatrix = XMMatrixTranslation(-50.f, 0.f, -10.f);

	// Render floor
	mesh->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	depthShader->render(renderer->getDeviceContext(), mesh->getIndexCount());

	worldMatrix = renderer->getWorldMatrix();
	worldMatrix = XMMatrixTranslation(0.f, 7.f, 5.f);
	scaleMatrix = XMMatrixScaling(0.5f, 0.5f, 0.5f);
	worldMatrix = XMMatrixMultiply(worldMatrix, scaleMatrix);
	// Render model
	model->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	depthShader->render(renderer->getDeviceContext(), model->getIndexCount());

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	renderer->setBackBufferRenderTarget();

	// Reset the viewport back to the original.
	renderer->resetViewport();

}

void App1::renderScene()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, scaleMatrix;
	XMMATRIX baseViewMatrix, orthoMatrix;

	// Clear the scene. (default blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	// Generate the view matrix based on the camera's position.
	camera->update();

	// get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	worldMatrix = renderer->getWorldMatrix();
	viewMatrix = camera->getViewMatrix();
	projectionMatrix = renderer->getProjectionMatrix();	

	worldMatrix = XMMatrixTranslation(-50.f, 0.f, -10.f);
	// Render floor
	mesh->sendData(renderer->getDeviceContext());
	shadowShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("brick"), renderTexture->getShaderResourceView(), light);
	shadowShader->render(renderer->getDeviceContext(), mesh->getIndexCount());

	// Render model
	worldMatrix = renderer->getWorldMatrix();
	worldMatrix = XMMatrixTranslation(0.f, 7.f, 5.f);
	scaleMatrix = XMMatrixScaling(0.5f, 0.5f, 0.5f);
	worldMatrix = XMMatrixMultiply(worldMatrix, scaleMatrix);
	model->sendData(renderer->getDeviceContext());
	shadowShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("brick"), renderTexture->getShaderResourceView(), light);
	shadowShader->render(renderer->getDeviceContext(), model->getIndexCount());

	/*baseViewMatrix = camera->getOrthoViewMatrix();
	orthoMatrix = renderer->getOrthoMatrix();
	renderer->setZBuffer(false);
	orthoMesh->sendData(renderer->getDeviceContext());
	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, baseViewMatrix, orthoMatrix, renderTexture->getShaderResourceView());
	textureShader->render(renderer->getDeviceContext(), orthoMesh->getIndexCount());
	renderer->setZBuffer(true);*/

	// Present the rendered scene to the screen.
	renderer->endScene();


}


