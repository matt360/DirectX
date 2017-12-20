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

void GeometryExample::gui(Camera * camera)
{
	if (example)
	{
		ImGui::Begin("Geometry Shader Example", &example);
		if (ImGui::Button("Reset Example"))
		{
			mesh_choice = MESH_CHOICE::CUBE;
			// reset geometry shader scale
			scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
			// reset geometry shader wireframe mode
			wireframe = false;
			// reset geometry shader primitive topology
			d3d11_primitive_topology_trianglelist = true;
			d3d11_primitive_topology_pointlist = false;
			// set cube mesh
			triangle_mesh = false;
			sphere_mesh = false;
			cube_mesh = true;
			quad_mesh = false;
			plane_mesh = false;
			// set the camera
			camera->setPosition(13.0f, 4.0f, -22.0f);
			camera->setRotation(0.0f, -35.0f, 0.0f);
		}
		ImGui::Checkbox("Wireframe", &wireframe);
		ImGui::SliderFloat3("Scale", (float*)&scale, -10.0f, 10.0f);
		if (ImGui::Button("Reset Scale")) scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
		if (ImGui::Checkbox("Primitive Topology Trianglelist", &d3d11_primitive_topology_trianglelist))
			d3d11_primitive_topology_pointlist = false;
		if (ImGui::Checkbox("Primitive Topology Pointlist", &d3d11_primitive_topology_pointlist))
			d3d11_primitive_topology_trianglelist = false;

		// what mesh to render (the highest one checked will be rendered (room for improvemnet: use menu box instead)
		if (ImGui::Checkbox("Triangle Mesh", &triangle_mesh))
		{
			mesh_choice = MESH_CHOICE::TRIANGLE;
			triangle_mesh = true;
			sphere_mesh = false;
			cube_mesh = false;
			quad_mesh = false;
			plane_mesh = false;
		}
		if (ImGui::Checkbox("Sphere Mesh", &sphere_mesh))
		{
			mesh_choice = MESH_CHOICE::SPHERE;
			triangle_mesh = false;
			sphere_mesh = true;
			cube_mesh = false;
			quad_mesh = false;
			plane_mesh = false;
		}
		if (ImGui::Checkbox("Cube Mesh", &cube_mesh))
		{
			mesh_choice = MESH_CHOICE::CUBE;
			triangle_mesh = false;
			sphere_mesh = false;
			cube_mesh = true;
			quad_mesh = false;
			plane_mesh = false;
		}
		if (ImGui::Checkbox("Quad Mesh", &quad_mesh))
		{
			mesh_choice = MESH_CHOICE::QUAD;
			triangle_mesh = false;
			sphere_mesh = false;
			cube_mesh = false;
			quad_mesh = true;
			plane_mesh = false;
		}
		if (ImGui::Checkbox("Plane Mesh", &plane_mesh))
		{
			mesh_choice = MESH_CHOICE::PLANE;
			triangle_mesh = false;
			sphere_mesh = false;
			cube_mesh = false;
			quad_mesh = false;
			plane_mesh = true;
		}
		ImGui::End();
	}
}
