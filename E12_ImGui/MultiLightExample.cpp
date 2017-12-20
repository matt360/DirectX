#include "MultiLightExample.h"

MultiLightExample::MultiLightExample()
{
	shader = nullptr;

	light0_ = nullptr;
	light1_ = nullptr;
	light2_ = nullptr;
	light3_ = nullptr;
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

	if (light0_)
	{
		delete light0_;
		light0_ = 0;
	}

	if (light1_)
	{
		delete light1_;
		light1_ = 0;
	}

	if (light2_)
	{
		delete light2_;
		light2_ = 0;
	}

	if (light3_)
	{
		delete light3_;
		light3_ = 0;
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
	light0_ = new Light;
	light0_->setDiffuseColour(1.0f, 0.0f, 0.0f, 1.0f);
	light0_->setPosition(-3.0f, 0.1f, 3.0f);
	//light0_->setAmbientColour(0.2f, 0.0f, 0.0f, 1.0f); // red
	//light0_->setDirection(0.0, 0.0f, 0.0f);
	//light0_->setSpecularPower(16.f);
	//light0_->setSpecularColour(1.0f, 1.0f, 1.0f, 1.0f);
	// Light 1
	light1_ = new Light;
	light1_->setDiffuseColour(0.0f, 1.0f, 0.0f, 1.0f);
	light1_->setPosition(3.0f, 0.1f, 3.0f);
	//light1_->setAmbientColour(0.0f, 2.0f, 0.0f, 1.0f); // green
	//light1_->setDirection(0.0, 0.0f, 0.0f);
	//light1_->setSpecularPower(16.f);
	//light1_->setSpecularColour(1.0f, 1.0f, 1.0f, 1.0f);
	// Light 2
	light2_ = new Light;
	light2_->setDiffuseColour(0.0f, 0.0f, 1.0f, 1.0f);
	light2_->setPosition(-3.0f, 0.1f, -3.0f);
	//light2_->setAmbientColour(0.0f, 0.0f, 0.2f, 1.0f); // blue
	//light2_->setDirection(0.0, 0.0f, 0.0f);
	//light2_->setSpecularPower(16.f);
	//light2_->setSpecularColour(1.0f, 1.0f, 1.0f, 1.0f);
	// Light 3
	light3_ = new Light;
	light3_->setDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
	light3_->setPosition(3.0f, 0.1f, -3.0f);
	//light3_->setAmbientColour(0.2f, 0.2f, 0.0f, 1.0f); // yellow
	//light3_->setDirection(0.0, 0.0f, 0.0f);
	//light3_->setSpecularPower(16.f);
	//light3_->setSpecularColour(1.0f, 1.0f, 1.0f, 1.0f);

	// multi light example lights' colours
	light0_col = ImColor(1.0f, 0.0f, 0.0f, 1.0f);
	light1_col = ImColor(0.0f, 1.0f, 0.0f, 1.0f);
	light2_col = ImColor(0.0f, 0.0f, 1.0f, 1.0f);
	light3_col = ImColor(1.0f, 1.0f, 1.0f, 1.0f);
	// multi light example lights' positions
	light0_pos = XMFLOAT3(-3.0f, 0.1f, 3.0f);
	light1_pos = XMFLOAT3(3.0f, 0.1f, 3.0f);
	light2_pos = XMFLOAT3(-3.0f, 0.1f, -3.0f);
	light3_pos = XMFLOAT3(3.0f, 0.1f, -3.0f);
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
	lightPosition[0] = light0_pos;
	lightPosition[1] = light1_pos;
	lightPosition[2] = light2_pos;
	lightPosition[3] = light3_pos;

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	if (plane_mesh) // plane
	{
		viewMatrix = camera->getViewMatrix();
		projectionMatrix = renderer->getProjectionMatrix();
		// translation and rotation
		worldMatrix = renderer->getWorldMatrix();
		XMMATRIX matrixTranslation = XMMatrixTranslation(-40.0f, 0.0, -40.0f);
		//XMMATRIX matrixTranslation = XMMatrixTranslation(0.0f, 0.0, 0.0f);
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
		//XMMATRIX matrixTranslation = XMMatrixTranslation(0.0f, 0.0, 0.0f);
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
		//XMMATRIX matrixTranslation = XMMatrixTranslation(0.0f, 0.0, 0.0f);
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
		lightPosition
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
			light0_pos = XMFLOAT3(-3.0f, 0.1f, 3.0f);
			light1_pos = XMFLOAT3(3.0f, 0.1f, 3.0f);
			light2_pos = XMFLOAT3(-3.0f, 0.1f, -3.0f);
			light3_pos = XMFLOAT3(3.0f, 0.1f, -3.0f);
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
		// change lights' position
		ImGui::SliderFloat3("Light 0 Pos", (float*)&light0_pos, -10.0f, 10.0f);
		ImGui::SliderFloat3("Light 1 Pos", (float*)&light1_pos, -10.0f, 10.0f);
		ImGui::SliderFloat3("Light 2 Pos", (float*)&light2_pos, -10.0f, 10.0f);
		ImGui::SliderFloat3("Light 3 Pos", (float*)&light3_pos, -10.0f, 10.0f);
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


