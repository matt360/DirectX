#include "SpecularLightExample.h"



SpecularLightExample::SpecularLightExample()
{
	specularLightShader = nullptr;
}


SpecularLightExample::~SpecularLightExample()
{
}

void SpecularLightExample::init(D3D* renderer, HWND hwnd)
{
	initShaders(renderer, hwnd);
}

// create shader handlers
void SpecularLightExample::initShaders(D3D* renderer, HWND hwnd)
{
	specularLightShader = new SpecularLightShader(renderer->getDevice(), hwnd);
}

void SpecularLightExample::renderSpecularLightExample(D3D* renderer, Camera* camera, BaseMesh* sphereMesh, TextureManager* textureMgr)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	// Clear the scene. (default blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	camera->update();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	worldMatrix = renderer->getWorldMatrix();
	// Generate the view matrix based on the camera's position.
	viewMatrix = camera->getViewMatrix();
	projectionMatrix = renderer->getProjectionMatrix();

	// wireframe mode
	renderer->setWireframeMode(wireframe);


	// Send geometry data (from mesh)
	//mesh->sendData(renderer->getDeviceContext());
	sphereMesh->sendData(renderer->getDeviceContext(), D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// Set shader parameters (matrices and texture)
	specularLightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("default"), specular_light, camera);
	// Render object (combination of mesh geometry and shader process
	specularLightShader->render(renderer->getDeviceContext(), sphereMesh->getIndexCount());


	// Render GUI
	gui();
	// Present the rendered scene to the screen.
	renderer->endScene();
}

void SpecularLightExample::gui()
{

}
