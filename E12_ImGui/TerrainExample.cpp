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
	initShader(renderer, hwnd);
	initVariables();
	initLight();
	
}

// create shader handlers
void TerrainExample::initShader(D3D* renderer, HWND hwnd)
{
	shader = new TerrainShader(renderer->getDevice(), hwnd);
}

void TerrainExample::initVariables()
{
	MESH_CHOICE mesh_choice = MESH_CHOICE::TERRAIN;
	over_time = 0.0f;
	scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	// geomatry shader topology handler (set to triangle list by default)
	d3d11_primitive_topology_trianglelist = true;
	d3d11_primitive_topology_pointlist = false;
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

void TerrainExample::render(D3D* renderer, Camera* camera, TextureManager* textureMgr)
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

	// Set primitive topology
	D3D_PRIMITIVE_TOPOLOGY d3d11_primitive_topology;
	if (d3d11_primitive_topology_trianglelist) d3d11_primitive_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	else d3d11_primitive_topology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;

	light->setPosition(0.0f, sinf(over_time * 3.0f), 0.0f);
	// Send geometry data (from mesh)
	mesh->sendData(renderer->getDeviceContext(), d3d11_primitive_topology);

	// Set shader parameters (matrices and texture)
	//terrainShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("default"), m_Light);
	shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("height"), light, over_time, height, frequency);

	// Render object (combination of mesh geometry and shader process
	shader->render(renderer->getDeviceContext(), mesh->getIndexCount()); // output data from the shader programme
}

// Terrain Example GUI window
void TerrainExample::gui(Camera* camera)
{
	// render only if the example is active
	if (example)
	{
		ImGui::Begin("Terrain", &example);
		if (ImGui::Button("Reset Example"))
		{
			resetExample(camera);
		}
		// wireframe
		ImGui::Checkbox("Wireframe", &wireframe);
		// scale
		ImGui::SliderFloat("Scale X", (float*)&scale.x, -15.0f, 15.0f);
		ImGui::SliderFloat("Scale Y", (float*)&scale.y, -15.0f, 15.0f);
		ImGui::SliderFloat("Scale Z", (float*)&scale.z, -15.0f, 15.0f);
		// reset scale
		if (ImGui::Button("Reset Scale")) scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
		// toggle topology
		if (ImGui::Checkbox("Primitive Topology Trianglelist", &d3d11_primitive_topology_trianglelist))
			d3d11_primitive_topology_pointlist = false;
		if (ImGui::Checkbox("Primitive Topology Pointlist", &d3d11_primitive_topology_pointlist))
			d3d11_primitive_topology_trianglelist = false;
		ImGui::End();
	}
}

void TerrainExample::resetExample(Camera * camera)
{
	mesh_choice = MESH_CHOICE::TERRAIN;
	// set terrain camera
	camera->setPosition(0.0f, 2.0f, -10.0f);
	camera->setRotation(0.0f, -200.0f, 0.0f);
	// reset terrain scale
	scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	// reset terrain wireframe mode
	wireframe = false;
	// reset geometry shader primitive topology
	d3d11_primitive_topology_trianglelist = true;
	d3d11_primitive_topology_pointlist = false;
}
