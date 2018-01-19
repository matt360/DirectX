#include "MultiLightExample.h"

MultiLightExample::MultiLightExample()
{
	shader_ = nullptr;
}

MultiLightExample::~MultiLightExample()
{
	if (shader_)
	{
		delete shader_;
		shader_ = 0;
	}

	if (mesh_)
	{
		delete mesh_;
		mesh_ = 0;
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
	shader_ = new MultiLightShader(renderer->getDevice(), hwnd);
}

void MultiLightExample::initVariables()
{
	over_time_ = 0.0f;
	scale_ = XMFLOAT3(1.0f, 1.0f, 1.0f);
	// geomatry shader topology handler (set to triangle list by default)
	d3d11_primitive_topology_trianglelist_ = true;
	d3d11_primitive_topology_pointlist_ = false;

	lights_.reserve(shader_->number_of_lights_);
	for (int i = 0; i < shader_->number_of_lights_; ++i)
	{
		lights_.push_back(new Light);
	}

	light_positions_.reserve(shader_->number_of_lights_);
	for (int i = 0; i < shader_->number_of_lights_; ++i)
	{
		light_positions_.push_back(new XMFLOAT4);
	}

	light_colours_.reserve(shader_->number_of_lights_);
	for (int i = 0; i < shader_->number_of_lights_; ++i)
	{
		light_colours_.push_back(new XMFLOAT4);
	}

	texture = "brick";
}

void MultiLightExample::initLight()
{
	// multi light example lights' colours
	for (int i = 0; i < shader_->number_of_lights_; i += 4)
	{
		light_colours_.at(i    )->operator=(XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
		light_colours_.at(i + 1)->operator=(XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
		light_colours_.at(i + 2)->operator=(XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));
		light_colours_.at(i + 3)->operator=(XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));
	}

	// multi light example lights' positions
	for (int i = 0; i < shader_->number_of_lights_; i += 4)
	{
		float temp_pos = (float)i * 2.0f;
		light_positions_.at(i    )->operator=(XMFLOAT4(-3.0f, 0.1f, temp_pos +  3.0f, 1.0f));
		light_positions_.at(i + 1)->operator=(XMFLOAT4( 3.0f, 0.1f, temp_pos +  3.0f, 1.0f));
		light_positions_.at(i + 2)->operator=(XMFLOAT4(-3.0f, 0.1f, temp_pos + -3.0f, 1.0f));
		light_positions_.at(i + 3)->operator=(XMFLOAT4( 3.0f, 0.1f, temp_pos + -3.0f, 1.0f));
	}
}

void MultiLightExample::render(D3D* renderer, Camera* camera, TextureManager* textureMgr)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	
	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	if (plane_mesh_) // plane
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
	XMMATRIX matrixScaling = XMMatrixScaling(scale_.x, scale_.y, scale_.z);
	worldMatrix *= matrixScaling;

	// wireframe mode
	renderer->setWireframeMode(wireframe_);

	// Set primitive topology
	D3D_PRIMITIVE_TOPOLOGY d3d11_primitive_topology;
	if (d3d11_primitive_topology_trianglelist_) d3d11_primitive_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	else d3d11_primitive_topology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;

	// Send geometry data (from mesh)
	mesh_->sendData(renderer->getDeviceContext(), d3d11_primitive_topology);

	// Set shader parameters (matrices and texture)
	//multiLightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("default"), m_Light, camera);
	shader_->setShaderParameters
	(
		renderer->getDeviceContext(),
		worldMatrix,
		viewMatrix,
		projectionMatrix,
		textureMgr->getTexture(texture), // for the default textrue pass an empty string as a name
		light_colours_,
		light_positions_
	);

	// Render object (combination of mesh geometry and shader process
	shader_->render(renderer->getDeviceContext(), mesh_->getIndexCount());
}

// Multi Light Example GUI window
void MultiLightExample::gui(Camera* camera)
{
	// render only if the example is active
	if (example_)
	{
		ImGui::Begin("Multi Light Example", &example_);
		if (ImGui::Button("Reset Example"))
		{
			resetExample(camera);
		}
		ImGui::Checkbox("Wireframe", &wireframe_);
		// change lights' colour
		ImGui::ColorEdit4("Light 0 Col",  (float*)light_colours_.at(0));
		ImGui::ColorEdit4("Light 1 Col",  (float*)light_colours_.at(1));
		ImGui::ColorEdit4("Light 2 Col",  (float*)light_colours_.at(2));
		ImGui::ColorEdit4("Light 3 Col",  (float*)light_colours_.at(3));
		ImGui::ColorEdit4("Light 4 Col",  (float*)light_colours_.at(4));
		ImGui::ColorEdit4("Light 5 Col",  (float*)light_colours_.at(5));
		ImGui::ColorEdit4("Light 6 Col",  (float*)light_colours_.at(6));
		ImGui::ColorEdit4("Light 7 Col",  (float*)light_colours_.at(7));
		ImGui::ColorEdit4("Light 8 Col",  (float*)light_colours_.at(8));
		ImGui::ColorEdit4("Light 9 Col",  (float*)light_colours_.at(9));
		ImGui::ColorEdit4("Light 10 Col", (float*)light_colours_.at(10));
		ImGui::ColorEdit4("Light 11 Col", (float*)light_colours_.at(11));
		ImGui::ColorEdit4("Light 12 Col", (float*)light_colours_.at(12));
		ImGui::ColorEdit4("Light 13 Col", (float*)light_colours_.at(13));
		ImGui::ColorEdit4("Light 14 Col", (float*)light_colours_.at(14));
		ImGui::ColorEdit4("Light 15 Col", (float*)light_colours_.at(15));
		// change lights' position
		float pos_clamp = 40.0f;
		ImGui::SliderFloat3("Light 0 Pos",  (float*)light_positions_.at(0),  -pos_clamp, pos_clamp);
		ImGui::SliderFloat3("Light 1 Pos",  (float*)light_positions_.at(1),  -pos_clamp, pos_clamp);
		ImGui::SliderFloat3("Light 2 Pos",  (float*)light_positions_.at(2),  -pos_clamp, pos_clamp);
		ImGui::SliderFloat3("Light 3 Pos",  (float*)light_positions_.at(3),  -pos_clamp, pos_clamp);
		ImGui::SliderFloat3("Light 4 Pos",  (float*)light_positions_.at(4),  -pos_clamp, pos_clamp);
		ImGui::SliderFloat3("Light 5 Pos",  (float*)light_positions_.at(5),  -pos_clamp, pos_clamp);
		ImGui::SliderFloat3("Light 6 Pos",  (float*)light_positions_.at(6),  -pos_clamp, pos_clamp);
		ImGui::SliderFloat3("Light 7 Pos",  (float*)light_positions_.at(7),  -pos_clamp, pos_clamp);
		ImGui::SliderFloat3("Light 8 Pos",  (float*)light_positions_.at(8),  -pos_clamp, pos_clamp);
		ImGui::SliderFloat3("Light 9 Pos",  (float*)light_positions_.at(9),  -pos_clamp, pos_clamp);
		ImGui::SliderFloat3("Light 10 Pos", (float*)light_positions_.at(10), -pos_clamp, pos_clamp);
		ImGui::SliderFloat3("Light 11 Pos", (float*)light_positions_.at(11), -pos_clamp, pos_clamp);
		ImGui::SliderFloat3("Light 12 Pos", (float*)light_positions_.at(12), -pos_clamp, pos_clamp);
		ImGui::SliderFloat3("Light 13 Pos", (float*)light_positions_.at(13), -pos_clamp, pos_clamp);
		ImGui::SliderFloat3("Light 14 Pos", (float*)light_positions_.at(14), -pos_clamp, pos_clamp);
		ImGui::SliderFloat3("Light 15 Pos", (float*)light_positions_.at(15), -pos_clamp, pos_clamp);
		// scale_
		ImGui::SliderFloat3("Scale", (float*)&scale_, -40.0f, 40.0f);
		// reset scale_
		if (ImGui::Button("Reset Scale")) scale_ = XMFLOAT3(1.0f, 1.0f, 1.0f);
		// toggle topology
		if (ImGui::Checkbox("Primitive Topology Trianglelist", &d3d11_primitive_topology_trianglelist_))
			d3d11_primitive_topology_pointlist_ = false;
		if (ImGui::Checkbox("Primitive Topology Pointlist", &d3d11_primitive_topology_pointlist_))
			d3d11_primitive_topology_trianglelist_ = false;
		// what mesh to render
		if (ImGui::Checkbox("Triangle Mesh", &triangle_mesh_))
		{
			resetExample(camera);
			set_mesh_choice(MESH_CHOICE::TRIANGLE);
		}
		if (ImGui::Checkbox("Sphere Mesh", &sphere_mesh_))
		{
			// set multi light camera
			camera->setPosition(0.0f, 0.0f, -4.75f);
			camera->setRotation(0.0f, 0.f, 0.f);
			// scale_ up sphere mesh
			scale_ = XMFLOAT3(1.0f, 1.0f, 40.0f);

			set_mesh_choice(MESH_CHOICE::SPHERE);
		}
		if (ImGui::Checkbox("Cube Mesh", &cube_mesh_))
		{
			resetExample(camera);
			set_mesh_choice(MESH_CHOICE::CUBE);
		}
		if (ImGui::Checkbox("Quad Mesh", &quad_mesh_))
		{
			resetExample(camera);
			set_mesh_choice(MESH_CHOICE::QUAD);
		}
		if (ImGui::Checkbox("Plane Mesh", &plane_mesh_))
		{
			camera->setPosition(-13.5f, 5.75f, -5.0f);
			camera->setRotation(0.0f, 50.0f, 15.0f);
			scale_ = XMFLOAT3(1.0f, 1.0f, 1.0f);

			set_mesh_choice(MESH_CHOICE::PLANE);
		}
		if (ImGui::Button("Tex: brick")) texture = "brick";
		if (ImGui::Button("Tex: bunny")) texture = "bunny";
		if (ImGui::Button("Tex: height")) texture = "height";
		if (ImGui::Button("Tex: checkerboard")) texture = "checkerboard";
		if (ImGui::Button("Tex: grass")) texture = "grass";
		if (ImGui::Button("Tex: rock")) texture = "rock";
		if (ImGui::Button("Tex: slope")) texture = "slope";
		ImGui::End();
	}
}

void MultiLightExample::resetExample(Camera* camera)
{
	// set multi light camera
	camera->setPosition(-13.5f, 5.75f, -5.0f);
	camera->setRotation(0.0f, 50.0f, 15.0f);
	// scale_ up sphere mesh
	scale_ = XMFLOAT3(1.0f, 1.0f, 1.0f);
	// reset light lights' colours
	initLight();
	// render only sphere mesh
	set_mesh_choice(MESH_CHOICE::PLANE);
	// reset wireframe
	wireframe_ = false;
	// reset geometry shader primitive topology
	d3d11_primitive_topology_trianglelist_ = true;
	d3d11_primitive_topology_pointlist_ = false;
}




