#include "MultiLightExample.h"

MultiLightExample::MultiLightExample()
{
	shader = nullptr;

	lights_.reserve(number_of_lights_);
	for (int i = 0; i < number_of_lights_; ++i)
	{
		lights_.push_back(new Light);
	}

	light_positions_.reserve(number_of_lights_);
	for (int i = 0; i < number_of_lights_; ++i)
	{
		light_positions_.push_back(new XMFLOAT4);
	}

	light_colours_.reserve(number_of_lights_);
	for (int i = 0; i < number_of_lights_; ++i)
	{
		light_colours_.push_back(new XMFLOAT4);
	}
}

MultiLightExample::~MultiLightExample()
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

	for (Light* light : lights_)
	{
		delete light;
		light = nullptr;
	}

	for (XMFLOAT4* light_position : light_positions_)
	{
		delete light_position;
		light_position = nullptr;
	}

	for (XMFLOAT4* light_colour : light_colours_)
	{
		delete light_colour;
		light_colour = nullptr;
	}
}

void MultiLightExample::init(D3D* renderer, HWND hwnd)
{
	initShader(renderer, hwnd);
	initVariables();
	initLight();
}

void MultiLightExample::initShader(D3D* renderer, HWND hwnd)
{
	shader = new MultiLightShader(renderer->getDevice(), hwnd);
}

void MultiLightExample::initVariables()
{
	MESH_CHOICE mesh_choice = MESH_CHOICE::SPHERE;
	over_time = 0.0f;
	scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	// geomatry shader topology handler (set to triangle list by default)
	d3d11_primitive_topology_trianglelist = true;
	d3d11_primitive_topology_pointlist = false;
}

void MultiLightExample::initLight()
{
	// lights' positions
	lights_.at(0)->setPosition(-3.0f, 0.1f, 3.0f);
	lights_.at(1)->setPosition(3.0f, 0.1f, 3.0f);
	lights_.at(2)->setPosition(-3.0f, 0.1f, -3.0f);
	lights_.at(3)->setPosition(3.0f, 0.1f, -3.0f);
	
	// multi light example lights' colours
	light_colours_.at(0)->operator=(XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
	light_colours_.at(1)->operator=(XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
	light_colours_.at(2)->operator=(XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));
	light_colours_.at(3)->operator=(XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));
	// multi light example lights' positions
	light_positions_.at(0)->operator=(XMFLOAT4(-3.0f, 0.1f, 3.0f, 1.0f));
	light_positions_.at(1)->operator=(XMFLOAT4(3.0f, 0.1f, 3.0f, 1.0f));
	light_positions_.at(2)->operator=(XMFLOAT4(-3.0f, 0.1f, -3.0f, 1.0f));
	light_positions_.at(3)->operator=(XMFLOAT4(3.0f, 0.1f, -3.0f, 1.0f));
}

void MultiLightExample::render(D3D* renderer, Camera* camera, TextureManager* textureMgr)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	
	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	if (plane_mesh) // plane
	{
		viewMatrix = camera->getViewMatrix();
		projectionMatrix = renderer->getProjectionMatrix();
		// translation and rotation
		worldMatrix = renderer->getWorldMatrix();
		XMMATRIX matrixTranslation = XMMatrixTranslation(-40.0f, 0.0, -40.0f);
		XMMATRIX matrixRotation = XMMatrixRotationX(XMConvertToRadians(0.0f));
		worldMatrix = XMMatrixMultiply(matrixRotation, matrixTranslation);
	}
	else
	{
		viewMatrix = camera->getViewMatrix();
		projectionMatrix = renderer->getProjectionMatrix();
		// translation and rotation
		worldMatrix = renderer->getWorldMatrix();
		XMMATRIX matrixTranslation = XMMatrixTranslation(0.0f, 0.0, 0.0f);
		XMMATRIX matrixRotation = XMMatrixRotationX(XMConvertToRadians(0.0f));
		worldMatrix = XMMatrixMultiply(matrixRotation, matrixTranslation);
	}
	// scaling
	XMMATRIX matrixScaling = XMMatrixScaling(scale.x, scale.y, scale.z);
	worldMatrix *= matrixScaling;

	// wireframe mode
	renderer->setWireframeMode(wireframe);

	// Set primitive topology
	D3D_PRIMITIVE_TOPOLOGY d3d11_primitive_topology;
	if (d3d11_primitive_topology_trianglelist) d3d11_primitive_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	else d3d11_primitive_topology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;

	// Send geometry data (from mesh)
	mesh->sendData(renderer->getDeviceContext(), d3d11_primitive_topology);

	// Set shader parameters (matrices and texture)
	//multiLightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("default"), m_Light, camera);
	shader->setShaderParameters
	(
		renderer->getDeviceContext(),
		worldMatrix,
		viewMatrix,
		projectionMatrix,
		textureMgr->getTexture("checkerboard"), // for the default textrue pass an empty string as a name
		light_colours_,
		light_positions_
	);

	// Render object (combination of mesh geometry and shader process
	shader->render(renderer->getDeviceContext(), mesh->getIndexCount());
}

void MultiLightExample::gui(Camera* camera)
{
	if (example)
	{
		ImGui::Begin("Multi Light Example", &example);
		if (ImGui::Button("Reset Example"))
		{
			resetExample(camera);
		}
		ImGui::Checkbox("Wireframe", &wireframe);
		// change lights' colour
		ImGui::ColorEdit4("Light 0 Col", (float*)light_colours_.at(0));
		ImGui::ColorEdit4("Light 1 Col", (float*)light_colours_.at(1));
		ImGui::ColorEdit4("Light 2 Col", (float*)light_colours_.at(2));
		ImGui::ColorEdit4("Light 3 Col", (float*)light_colours_.at(3));
		// change lights' position TODO check
		ImGui::SliderFloat3("Light 0 Pos", (float*)light_positions_.at(0), -10.0f, 10.0f);
		ImGui::SliderFloat3("Light 1 Pos", (float*)light_positions_.at(1), -10.0f, 10.0f);
		ImGui::SliderFloat3("Light 2 Pos", (float*)light_positions_.at(2), -10.0f, 10.0f);
		ImGui::SliderFloat3("Light 3 Pos", (float*)light_positions_.at(3), -10.0f, 10.0f);
		// scale
		ImGui::SliderFloat3("Scale", (float*)&scale, -20.0f, 20.0f);
		// reset scale
		if (ImGui::Button("Reset Scale")) scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
		// toggle topology
		if (ImGui::Checkbox("Primitive Topology Trianglelist", &d3d11_primitive_topology_trianglelist))
			d3d11_primitive_topology_pointlist = false;
		if (ImGui::Checkbox("Primitive Topology Pointlist", &d3d11_primitive_topology_pointlist))
			d3d11_primitive_topology_trianglelist = false;
		// what mesh to render
		if (ImGui::Checkbox("Triangle Mesh", &triangle_mesh))
		{
			resetExample(camera);
			set_mesh_choice(MESH_CHOICE::TRIANGLE);
		}
		if (ImGui::Checkbox("Sphere Mesh", &sphere_mesh))
		{
			resetExample(camera);
			set_mesh_choice(MESH_CHOICE::SPHERE);
		}
		if (ImGui::Checkbox("Cube Mesh", &cube_mesh))
		{
			resetExample(camera);
			set_mesh_choice(MESH_CHOICE::CUBE);
		}
		if (ImGui::Checkbox("Quad Mesh", &quad_mesh))
		{
			resetExample(camera);
			set_mesh_choice(MESH_CHOICE::QUAD);
		}
		if (ImGui::Checkbox("Plane Mesh", &plane_mesh))
		{
			camera->setPosition(-8.75f, 3.0f, -3.5f);
			camera->setRotation(0.0f, 70.0f, 15.0f);
			scale = XMFLOAT3(1.0f, 1.0f, 1.0f);

			set_mesh_choice(MESH_CHOICE::PLANE);
		}
		ImGui::End();
	}
}

void MultiLightExample::resetExample(Camera* camera)
{
	// set multi light camera
	camera->setPosition(0.0f, 0.0f, -4.75f);
	camera->setRotation(0.0f, 0.f, 0.f);
	// scale up sphere mesh
	scale = XMFLOAT3(1.0f, 1.0f, 20.0f);
	// reset light colours
	light_colours_.at(0)->operator=(XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
	light_colours_.at(1)->operator=(XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
	light_colours_.at(2)->operator=(XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));
	light_colours_.at(3)->operator=(XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));
	// reset light positions
	light_positions_.at(0)->operator=(XMFLOAT4(-3.0f, 0.1f, 3.0f, 1.0f));
	light_positions_.at(1)->operator=(XMFLOAT4(3.0f, 0.1f, 3.0f, 1.0f));
	light_positions_.at(2)->operator=(XMFLOAT4(-3.0f, 0.1f, -3.0f, 1.0f));
	light_positions_.at(3)->operator=(XMFLOAT4(3.0f, 0.1f, -3.0f, 1.0f));
	// render only sphere mesh
	mesh_choice = MESH_CHOICE::SPHERE;
	triangle_mesh = false;
	sphere_mesh = true;
	cube_mesh = false;
	quad_mesh = false;
	plane_mesh = false;
	// reset wireframe
	wireframe = false;
	// reset geometry shader primitive topology
	d3d11_primitive_topology_trianglelist = true;
	d3d11_primitive_topology_pointlist = false;
}




