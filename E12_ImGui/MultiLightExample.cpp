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
	initLight();
	initShader(renderer, hwnd);
	over_time = 0.0f;
	scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
}

void MultiLightExample::initShader(D3D* renderer, HWND hwnd)
{
	shader = new MultiLightShader(renderer->getDevice(), hwnd);
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

void MultiLightExample::render(D3D* renderer, Camera* camera, BaseMesh* mesh, TextureManager* textureMgr)
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
	if (mesh_choice == MESH::PLANE) // plane
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
	else if (mesh_choice == MESH::SPHERE) // sphere
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

	// Send geometry data (from mesh)
	mesh->sendData(renderer->getDeviceContext(), D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

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


