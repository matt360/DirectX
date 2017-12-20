#include "GeometryExample.h"

GeometryExample::GeometryExample()
{
	shader = nullptr;
}

GeometryExample::~GeometryExample()
{
	if (shader)
	{
		delete shader;
		shader = 0;
	}

	if (mesh)
	{
		delete mesh;
		mesh = 0;
	}
}

void GeometryExample::init(D3D * renderer, HWND hwnd)
{
	initShader(renderer, hwnd);
	initVariables();
}

void GeometryExample::initShader(D3D * renderer, HWND hwnd)
{
	shader = new GeometryShader(renderer->getDevice(), hwnd);
}

void GeometryExample::initVariables()
{
	over_time = 0.0f;
	scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	// geomatry shader topology handler (set to triangle list by default)
	d3d11_primitive_topology_trianglelist = true;
	d3d11_primitive_topology_pointlist = false;
}

void GeometryExample::render(D3D * renderer, Camera * camera, TextureManager * textureMgr)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	// WORLD MATRIX
	worldMatrix = renderer->getWorldMatrix();
	XMMATRIX matrix1Translation = DirectX::XMMatrixTranslation(2.0f, 0.0f, 0.0f);
	//XMMATRIX matrix1Rotation = DirectX::XMMatrixRotationZ(light_y);
	XMMATRIX matrix1Rotation = DirectX::XMMatrixRotationZ(0.0f);
	// orbit
	//worldMatrix = XMMatrixMultiply(matrix1Translation, matrix1Rotation);
	// translate and rotate
	worldMatrix = XMMatrixMultiply(matrix1Rotation, matrix1Translation);
	// scaling
	XMMATRIX matrix1Scaling = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	worldMatrix *= matrix1Scaling;
	// VIEW MATRIX
	viewMatrix = camera->getViewMatrix();
	// PROJECTION MATRIX
	projectionMatrix = renderer->getProjectionMatrix();

	// wireframe mode
	renderer->setWireframeMode(wireframe);

	/*
	// ONE TRIANGLE - D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	triangleMesh->sendData(renderer->getDeviceContext(), D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// THREE TRIANGLES - D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST
	triangleMesh->sendData(renderer->getDeviceContext(), D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	// *NOTE* ecause we're offsetting from the normals there's going to be SIX cubes around the original cube
	cubeMesh->sendData(renderer->getDeviceContext(), D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	*/

	// Set primitive topology
	D3D_PRIMITIVE_TOPOLOGY d3d11_primitive_topology;
	if (d3d11_primitive_topology_trianglelist) d3d11_primitive_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	else d3d11_primitive_topology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;

	// Send geometry data (from mesh)
	mesh->sendData(renderer->getDeviceContext(), d3d11_primitive_topology);

	// Set shader parameters (matrices and texture)
	shader->setShaderParameters(renderer->getDeviceContext(),
		worldMatrix, viewMatrix, projectionMatrix,
		textureMgr->getTexture("checkerboard"),
		textureMgr->getTexture("bunny"));

	// Render object (combination of mesh geometry and shader process
	shader->render(renderer->getDeviceContext(), mesh->getIndexCount());
}
