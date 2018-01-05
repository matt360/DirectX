#include "MultiLightExample.h"

MultiLightExample::MultiLightExample()
{
	shader = nullptr;

	number_of_lights_ = 4;
	lights_.reserve(number_of_lights_);
	for (int i = 0; i < number_of_lights_; ++i)
	{
		lights_.push_back(new Light);
	}

	for (int i = 0; i < number_of_lights_; ++i)
	{
		light_positions_.push_back(new XMFLOAT3);
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

	for (XMFLOAT3* light_position : light_positions_)
	{
		delete light_position;
		light_position = nullptr;
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
	// Light 0
	lights_.at(0)->setDiffuseColour(1.0f, 0.0f, 0.0f, 1.0f);
	lights_.at(0)->setPosition(-3.0f, 0.1f, 3.0f);
	//lights_.at(0)->setAmbientColour(0.2f, 0.0f, 0.0f, 1.0f); // red
	//lights_.at(0)->setDirection(0.0, 0.0f, 0.0f);
	//lights_.at(0)->setSpecularPower(16.f);
	//lights_.at(0)->setSpecularColour(1.0f, 1.0f, 1.0f, 1.0f);

	// Light 1
	lights_.at(1)->setDiffuseColour(0.0f, 1.0f, 0.0f, 1.0f);
	lights_.at(1)->setPosition(3.0f, 0.1f, 3.0f);
	//lights_.at(1)->setAmbientColour(0.0f, 2.0f, 0.0f, 1.0f); // green
	//lights_.at(1)->setDirection(0.0, 0.0f, 0.0f);
	//lights_.at(1)->setSpecularPower(16.f);
	//lights_.at(1)->setSpecularColour(1.0f, 1.0f, 1.0f, 1.0f);

	// Light 2
	lights_.at(2)->setDiffuseColour(0.0f, 0.0f, 1.0f, 1.0f);
	lights_.at(2)->setPosition(-3.0f, 0.1f, -3.0f);
	//lights_.at(2)->setAmbientColour(0.0f, 0.0f, 0.2f, 1.0f); // blue
	//lights_.at(2)->setDirection(0.0, 0.0f, 0.0f);
	//lights_.at(2)->setSpecularPower(16.f);
	//lights_.at(2)->setSpecularColour(1.0f, 1.0f, 1.0f, 1.0f);

	// Light 3
	lights_.at(3)->setDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
	lights_.at(3)->setPosition(3.0f, 0.1f, -3.0f);
	//lights_.at(3)->setAmbientColour(0.2f, 0.2f, 0.0f, 1.0f); // yellow
	//lights_.at(3)->setDirection(0.0, 0.0f, 0.0f);
	//lights_.at(3)->setSpecularPower(16.f);
	//lights_.at(3)->setSpecularColour(1.0f, 1.0f, 1.0f, 1.0f);

	// multi light example lights' colours
	light0_col = ImColor(1.0f, 0.0f, 0.0f, 1.0f);
	light1_col = ImColor(0.0f, 1.0f, 0.0f, 1.0f);
	light2_col = ImColor(0.0f, 0.0f, 1.0f, 1.0f);
	light3_col = ImColor(1.0f, 1.0f, 1.0f, 1.0f);
	// multi light example lights' positions
	light_positions_.at(0)->operator=(XMFLOAT3(-3.0f, 0.1f, 3.0f));
	light_positions_.at(1)->operator=(XMFLOAT3(3.0f, 0.1f, 3.0f));
	light_positions_.at(2)->operator=(XMFLOAT3(-3.0f, 0.1f, -3.0f));
	light_positions_.at(3)->operator=(XMFLOAT3(3.0f, 0.1f, -3.0f));
}

void MultiLightExample::render(D3D* renderer, Camera* camera, TextureManager* textureMgr)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	XMFLOAT4 diffuseColor[4];
	XMFLOAT3 lightPosition[4];

	// Create the diffuse color array from the four light colors.
	diffuseColor[0] = XMFLOAT4(light0_col.x, light0_col.y, light0_col.z, light0_col.w);
	diffuseColor[1] = XMFLOAT4(light1_col.x, light1_col.y, light1_col.z, light1_col.w);
	diffuseColor[2] = XMFLOAT4(light2_col.x, light2_col.y, light2_col.z, light2_col.w);
	diffuseColor[3] = XMFLOAT4(light3_col.x, light3_col.y, light3_col.z, light3_col.w);

	// Create the light position array from the four light positions.
	lightPosition[0] = (*light_positions_.at(0));
	lightPosition[1] = (*light_positions_.at(1));
	lightPosition[2] = (*light_positions_.at(2));
	lightPosition[3] = (*light_positions_.at(3));

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
	else if (sphere_mesh) // sphere
	{
		viewMatrix = camera->getViewMatrix();
		projectionMatrix = renderer->getProjectionMatrix();
		// translation and rotation
		worldMatrix = renderer->getWorldMatrix();
		XMMATRIX matrixTranslation = XMMatrixTranslation(0.0f, 0.0, 0.0f);
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
		diffuseColor,
		light_positions_
	);

	// Render object (combination of mesh geometry and shader process
	shader->render(renderer->getDeviceContext(), mesh->getIndexCount());
}

void MultiLightExample::gui(Camera * camera)
{
	if (example)
	{
		ImGui::Begin("Multi Light Example", &example);
		if (ImGui::Button("Reset Example"))
		{
			// set multi light camera
			camera->setPosition(0.0f, 0.0f, -4.75f);
			camera->setRotation(0.0f, 0.f, 0.f);
			// scale up sphere mesh
			scale = XMFLOAT3(1.0f, 1.0f, 20.0f);
			// reset light colours
			light0_col = ImColor(1.0f, 0.0f, 0.0f, 1.0f);
			light1_col = ImColor(0.0f, 1.0f, 0.0f, 1.0f);
			light2_col = ImColor(0.0f, 0.0f, 1.0f, 1.0f);
			light3_col = ImColor(1.0f, 1.0f, 1.0f, 1.0f);
			// reset light positions
			light_positions_.at(0)->operator=(XMFLOAT3(-3.0f, 0.1f, 3.0f));
			light_positions_.at(1)->operator=(XMFLOAT3(3.0f, 0.1f, 3.0f));
			light_positions_.at(2)->operator=(XMFLOAT3(-3.0f, 0.1f, -3.0f));
			light_positions_.at(3)->operator=(XMFLOAT3(3.0f, 0.1f, -3.0f));
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
		ImGui::Checkbox("Wireframe", &wireframe);
		// change lights' colour
		ImGui::ColorEdit3("Light 0 Col", (float*)&light0_col);
		ImGui::ColorEdit3("Light 1 Col", (float*)&light1_col);
		ImGui::ColorEdit3("Light 2 Col", (float*)&light2_col);
		ImGui::ColorEdit3("Light 3 Col", (float*)&light3_col);
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
		// what mesh to render (the highest one checked will be rendered (room for improvemnet: use menu box instead)
		if (ImGui::Checkbox("Triangle Mesh", &triangle_mesh))
		{
			// set multi light camera
			camera->setPosition(0.0f, 0.0f, -4.75f);

			mesh_choice = MESH_CHOICE::TRIANGLE;
			triangle_mesh = true;
			sphere_mesh = false;
			cube_mesh = false;
			quad_mesh = false;
			plane_mesh = false;
		}
		if (ImGui::Checkbox("Sphere Mesh", &sphere_mesh))
		{
			// set multi light camera
			camera->setPosition(0.0f, 0.0f, -4.75f);

			mesh_choice = MESH_CHOICE::SPHERE;
			triangle_mesh = false;
			sphere_mesh = true;
			cube_mesh = false;
			quad_mesh = false;
			plane_mesh = false;
		}
		if (ImGui::Checkbox("Cube Mesh", &cube_mesh))
		{
			// set multi light camera
			camera->setPosition(0.0f, 0.0f, -4.75f);

			mesh_choice = MESH_CHOICE::CUBE;
			triangle_mesh = false;
			sphere_mesh = false;
			cube_mesh = true;
			quad_mesh = false;
			plane_mesh = false;
		}
		if (ImGui::Checkbox("Quad Mesh", &quad_mesh))
		{
			// set multi light camera
			camera->setPosition(0.0f, 0.0f, -4.75f);

			mesh_choice = MESH_CHOICE::QUAD;
			triangle_mesh = false;
			sphere_mesh = false;
			cube_mesh = false;
			quad_mesh = true;
			plane_mesh = false;
		}
		if (ImGui::Checkbox("Plane Mesh", &plane_mesh))
		{
			camera->setPosition(0.0f, 3.0f, 0.0f);
			scale = XMFLOAT3(1.0f, 1.0f, 1.0f);

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


