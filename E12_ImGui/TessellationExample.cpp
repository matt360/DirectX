#include "TessellationExample.h"

TessellationExample::TessellationExample()
{
	shader = nullptr;
}

TessellationExample::~TessellationExample()
{
	if (shader)
	{
		delete shader;
		shader = 0;
	}
}

void TessellationExample::init(D3D * renderer, HWND hwnd)
{
	initShader(renderer, hwnd);
	initVariables();
}

void TessellationExample::initShader(D3D * renderer, HWND hwnd)
{
	shader = new TessellationShader(renderer->getDevice(), hwnd);
}

void TessellationExample::initVariables()
{
	MESH_CHOICE mesh_choice = MESH_CHOICE::TERRAIN;
	over_time = 0.0f;
	scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	// geomatry shader topology handler (set to triangle list by default)
	d3d11_primitive_topology_trianglelist = true;
	d3d11_primitive_topology_pointlist = false;
}

void TessellationExample::render(D3D * renderer, Camera * camera, TextureManager * textureMgr)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	// WORLD MATRIX
	worldMatrix = renderer->getWorldMatrix();
	XMMATRIX matrix1Translation = DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	//XMMATRIX matrix1Rotation = DirectX::XMMatrixRotationZ(overt_time);
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

	// Set primitive topology
	/*D3D_PRIMITIVE_TOPOLOGY d3d11_primitive_topology;
	if (d3d11_primitive_topology_trianglelist) d3d11_primitive_topology = D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST;
	else d3d11_primitive_topology = D3D11_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST;*/

	// Send geometry data (from mesh)
	mesh->sendData(renderer->getDeviceContext(), D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
	
	// Set shader parameters (matrices and texture)
	shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,
		textureMgr->getTexture("brick"), camera);

	// Render object (combination of mesh geometry and shader process
	shader->render(renderer->getDeviceContext(), mesh->getIndexCount());
}

void TessellationExample::gui(Camera * camera)
{
	if (example)
	{
		ImGui::Begin("Tessellation", &example);
		if (ImGui::Button("Reset Example"))
		{
			resetExample(camera);
		}
		// wireframe
		ImGui::Checkbox("Wireframe", &wireframe);
		// scale
		ImGui::SliderFloat("Scale X", (float*)&scale.x, -15.0f, 15.0f);
		ImGui::SliderFloat("Scale Z", (float*)&scale.z, -15.0f, 15.0f);
		ImGui::SliderFloat3("Scale", (float*)&scale, -15.0f, 15.0f);
		// reset scale
		if (ImGui::Button("Reset Scale")) scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
		// toggle topology
		/*if (ImGui::Checkbox("Primitive Topology Trianglelist", &d3d11_primitive_topology_trianglelist))
		d3d11_primitive_topology_pointlist = false;
		if (ImGui::Checkbox("Primitive Topology Pointlist", &d3d11_primitive_topology_pointlist))
		d3d11_primitive_topology_trianglelist = false;*/
		ImGui::End();
	}
}

void TessellationExample::resetExample(Camera * camera)
{
	mesh_choice = MESH_CHOICE::TERRAIN;
	// set tessellation camera
	camera->setPosition(0.0f, 4.75f, -10.0f);
	camera->setRotation(0.0f, 30.0f, 0.0f);
	// reset terrain scale
	scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	// reset wireframe
	wireframe = false;
	// reset geometry shader primitive topology
	d3d11_primitive_topology_trianglelist = true;
	d3d11_primitive_topology_pointlist = false;
}
