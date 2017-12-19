#include "TerrainExample.h"



TerrainExample::TerrainExample()
{
	shader = nullptr;
}


TerrainExample::~TerrainExample()
{
	if (shader)
	{
		delete shader;
		shader = 0;
	}
}



void TerrainExample::init(D3D* renderer, HWND hwnd)
{
	initLight();
	initShader(renderer, hwnd);
	over_time = 0.0f;
	tr_scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
}

// create shader handlers
void TerrainExample::initShader(D3D* renderer, HWND hwnd)
{
	shader = new TerrainShader(renderer->getDevice(), hwnd);
}

void TerrainExample::initLight()
{
	// specular light example
	light = new Light;
	light->setAmbientColour(0.5f, 0.5f, 0.5f, 1.0f);
	light->setDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
	light->setDirection(0.0, 0.0f, 0.0f);
	light->setSpecularPower(16.f);
	light->setSpecularColour(1.0f, 1.0f, 1.0f, 1.0f);
	light->setPosition(0.0f, 0.1f, 0.0f);
}

void TerrainExample::render(D3D* renderer, Camera* camera, BaseMesh* mesh, TextureManager* textureMgr)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	worldMatrix = renderer->getWorldMatrix();
	// Generate the view matrix based on the camera's position.
	viewMatrix = camera->getViewMatrix();
	projectionMatrix = renderer->getProjectionMatrix();

	// wireframe mode
	renderer->setWireframeMode(wireframe);

	// Send geometry data (from mesh)
	mesh->sendData(renderer->getDeviceContext(), D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// Set shader parameters (matrices and texture)
	shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("default"), light, camera);
	// Render object (combination of mesh geometry and shader process
	shader->render(renderer->getDeviceContext(), mesh->getIndexCount());
}
