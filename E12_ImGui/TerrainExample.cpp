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

	if (light)
	{
		delete light;
		light = 0;
	}
}

void TerrainExample::init(D3D* renderer, HWND hwnd)
{
	initVariables();
	initLight();
	initShader(renderer, hwnd);
	
}

void TerrainExample::initVariables()
{
	over_time = 0.0f;
	scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
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

// create shader handlers
void TerrainExample::initShader(D3D* renderer, HWND hwnd)
{
	shader = new TerrainShader(renderer->getDevice(), hwnd);
}


void TerrainExample::render(D3D* renderer, Camera* camera, BaseMesh* mesh, TextureManager* textureMgr)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	viewMatrix = camera->getViewMatrix();
	projectionMatrix = renderer->getProjectionMatrix();
	// translation and rotation
	worldMatrix = renderer->getWorldMatrix();
	XMMATRIX matrixTranslation = XMMatrixTranslation(-20.0f, 0.0, 0.0f);
	XMMATRIX matrixRotation = XMMatrixRotationX(XMConvertToRadians(180.0f));
	worldMatrix = XMMatrixMultiply(matrixRotation, matrixTranslation);
	// scaling
	XMMATRIX matrixScaling = XMMatrixScaling(scale.x, scale.y, scale.z);
	worldMatrix *= matrixScaling;

	// wave's:
	float height = 1.0f;
	float frequency = 1.0f;

	// wireframe mode
	renderer->setWireframeMode(wireframe);

	light->setPosition(0.0f, sinf(over_time * 3.0f), 0.0f);
	// Send geometry data (from mesh)
	mesh->sendData(renderer->getDeviceContext(), D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Set shader parameters (matrices and texture)
	//terrainShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("default"), m_Light);
	shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("height"), light, over_time, height, frequency);

	// Render object (combination of mesh geometry and shader process
	shader->render(renderer->getDeviceContext(), mesh->getIndexCount()); // output data from the shader programme
}
