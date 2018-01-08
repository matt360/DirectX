// texture shader.cpp
#include "MultiLightShader.h"

// When resources are being created and interfaced with,
// the device interface is used.
MultiLightShader::MultiLightShader(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd)
{
	// compiled shader object
	initShader(L"multi_light_vs.cso", L"multi_light_ps.cso");
}

MultiLightShader::~MultiLightShader()
{
	// Release the sampler state.
	if (sampleState_)
	{
		sampleState_->Release();
		sampleState_ = 0;
	}

	// Release the matrix constant buffer.
	if (matrixBuffer_)
	{
		matrixBuffer_->Release();
		matrixBuffer_ = 0;
	}

	// Release the layout.
	if (layout_)
	{
		layout_->Release();
		layout_ = 0;
	}

	if (cameraBuffer_)
	{
		cameraBuffer_->Release();
		cameraBuffer_ = 0;
	}

	// Release the light color constant buffer.
	if (lightColorBuffer_)
	{
		lightColorBuffer_->Release();
		lightColorBuffer_ = 0;
	}

	// Release the light position constant buffer.
	if (lightPositionBuffer_)
	{
		lightPositionBuffer_->Release();
		lightPositionBuffer_ = 0;
	}

	//Release base shader components
	BaseShader::~BaseShader();
}


void MultiLightShader::initShader(WCHAR* vsFilename, WCHAR* psFilename)
{
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;
	// Camera
	D3D11_BUFFER_DESC cameraBufferDesc;
	// Light
	D3D11_BUFFER_DESC lightPositionBufferDesc;
	D3D11_BUFFER_DESC lightColorBufferDesc;

	// Load (+ compile) shader files
	loadVertexShader(vsFilename);
	loadPixelShader(psFilename);

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//matrixBufferDesc.MiscFlags = D3D11_RESOURCE_MISC_FLAG::;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	// ID3D11Device interface is reposnsible for creating all memory resources
	// The created resources can then be attached to the pipeline either directly or with a resource view,
	// where they are then used during a pipeline execution event
	// The resource creation process uses a different ID3D11Device method for each type of resource,
	// but they all follow the same general pattern.

	// The creation mathods all take three parameters. The first parameter is a structure that specifies 
	// all of the various options that a resource can be created with.
	// It is referred ti as a resource description. Each resource type uses its own description structure, since they each 
	// have a different set of available properties, but the structures all serve the same purpose - 
	// to define the desired characteristics of the created resource.

	// The second parameter in the resource creation methods is a pointer to a D3D11_SUBRESOURCE_DATA structure,
	// which is used to provied the initial data to be loaded inta a resource.
	// For example, if a buffer resource will hold static verted data, this structure would be used to pass a models's
	// vertex data into the buffer. This eliminates the need to manually manipulate the buffer after it is created,
	// if its contents will not be changing. This parameter can also just be set to null if non initialization is required. 

	// The final parameter is a pointer to a pointer to the appropriate resource interface,
	// which is where the created resource pointer is stored after a successful resource creation event.

	// In each if these methods, the real configuration ocurs in the resource description structure. As mentioned above,
	// each resource type has its own structure used to define its properties. However, there are some common elements that are
	// shared across all of the structures. These include the usage flags, bind flags, CPU access flags, and miscellaneous flags.

                           // resource       resouce  a pointer to a pointer to the appropriate resource
	                       // description,   creation 
	                       //                method,
	renderer->CreateBuffer(&matrixBufferDesc, NULL, &matrixBuffer_);

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	renderer->CreateSamplerState(&samplerDesc, &sampleState_);


	// Camera buffer
	cameraBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	cameraBufferDesc.ByteWidth = sizeof(CameraBufferType);
	cameraBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cameraBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cameraBufferDesc.MiscFlags = 0;
	cameraBufferDesc.StructureByteStride = 0;

	renderer->CreateBuffer(&cameraBufferDesc, NULL, &cameraBuffer_);


	// Setup light position buffer
	// Setup the description of the light dynamic constant buffer that is in the pixel shader.
	// Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER or CreateBuffer will fail.
	lightPositionBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightPositionBufferDesc.ByteWidth = sizeof(LightColourBufferType);
	lightPositionBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightPositionBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightPositionBufferDesc.MiscFlags = 0;
	lightPositionBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	renderer->CreateBuffer(&lightPositionBufferDesc, NULL, &lightColorBuffer_);

	// Setup light color buffer
	lightColorBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightColorBufferDesc.ByteWidth = sizeof(LightPositionBufferType);
	lightColorBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightColorBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightColorBufferDesc.MiscFlags = 0;
	lightColorBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the pixel shader constant buffer from within this class.
	renderer->CreateBuffer(&lightColorBufferDesc, NULL, &lightPositionBuffer_);
}

//void MultiLightShader::setShaderParameters
//(
//	ID3D11DeviceContext* deviceContext, 
//	const XMMATRIX &worldMatrix, 
//	const XMMATRIX &viewMatrix, 
//	const XMMATRIX &projectionMatrix, 
//	ID3D11ShaderResourceView* texture, 
//	Light* light0,
//	Light* light1,
//	Light* light2,
//	Light* light3,
//	Camera* camera
//)
//{
//	HRESULT result;
//	D3D11_MAPPED_SUBRESOURCE mappedResource;
//	// vertex shader
//	MatrixBufferType* dataPtr;
//	LightPositionBuffer* lightPosPtr;
//	CameraBufferType* cameraPtr;
//	// pixel shader
//	LightColorBuffer* lightColPtr;
//
//	unsigned int bufferNumber;
//	XMMATRIX tworld, tview, tproj;
//
//	// Transpose the matrices to prepare them for the shader.
//	tworld = XMMatrixTranspose(worldMatrix);
//	tview = XMMatrixTranspose(viewMatrix);
//	tproj = XMMatrixTranspose(projectionMatrix);
//
//	// Lock the constant buffer so it can be written to.
//	result = deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
//
//	// Get a pointer to the data in the constant buffer.
//	dataPtr = (MatrixBufferType*)mappedResource.pData;
//
//	// Copy the matrices into the constant buffer.
//	dataPtr->world = tworld;// worldMatrix;
//	dataPtr->view = tview;
//	dataPtr->projection = tproj;
//
//	// Unlock the constant buffer.
//	deviceContext->Unmap(matrixBuffer, 0);
//
//	// Set the position of the constant buffer in the vertex shader.
//	bufferNumber = 0;
//
//	// Now set the constant buffer in the vertex shader with the updated values.
//	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &matrixBuffer);
//
//	// Additional //
//	// Camera
//	deviceContext->Map(cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
//	cameraPtr = (CameraBufferType*)mappedResource.pData;
//	cameraPtr->cameraPosition = camera->getPosition();
//	cameraPtr->padding = 0.0f;
//	deviceContext->Unmap(cameraBuffer, 0);
//	bufferNumber = 1;
//	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &cameraBuffer);
//
//	// Send light data to vertex shader
//	deviceContext->Map(lightPositionBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
//	lightPosPtr = (LightPositionBuffer*)mappedResource.pData;
//	lightPosPtr->ambient = light0->getAmbientColour();
//	lightPosPtr->diffuse[0] = light0->getDiffuseColour();
//	lightPosPtr->diffuse[1] = light1->getDiffuseColour();
//	lightPosPtr->diffuse[2] = light2->getDiffuseColour();
//	lightPosPtr->diffuse[3] = light3->getDiffuseColour();
//	lightPosPtr->direction = light0->getDirection();
//	lightPosPtr->specularPower = light0->getSpecularPower();
//	lightPosPtr->specular = light0->getSpecularColour();
//	lightPosPtr->position[0] = light0->getPosition();
//	lightPosPtr->position[1] = light1->getPosition();
//	lightPosPtr->position[2] = light2->getPosition();
//	lightPosPtr->position[3] = light3->getPosition();
//	//lightPtr->padding = 0.0f;
//
//	//lightPtr->padding = 0.0f;
//	deviceContext->Unmap(lightPositionBuffer, 0);
//	bufferNumber = 2;
//	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &lightPositionBuffer);
//
//	// Send light data to pixel shader
//	deviceContext->Map(lightColorBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
//	lightColPtr = (LightColorBuffer*)mappedResource.pData;
//	lightColPtr->ambient = light0->getAmbientColour();
//	lightColPtr->diffuse[0] = light0->getDiffuseColour();
//	lightColPtr->diffuse[1] = light1->getDiffuseColour();
//	lightColPtr->diffuse[2] = light2->getDiffuseColour();
//	lightColPtr->diffuse[3] = light3->getDiffuseColour();
//	lightColPtr->direction = light0->getDirection();
//	lightColPtr->specularPower = light0->getSpecularPower();
//	lightColPtr->specular = light0->getSpecularColour();
//	lightColPtr->position[0] = light0->getPosition();
//	lightColPtr->position[1] = light1->getPosition();
//	lightColPtr->position[2] = light2->getPosition();
//	lightColPtr->position[3] = light3->getPosition();
//	//lightPtr->padding = 0.0f;
//	
//	//lightPtr->padding = 0.0f;
//	deviceContext->Unmap(lightColorBuffer, 0);
//	bufferNumber = 0;
//	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &lightColorBuffer);
//
//	// Set shader texture resource in the pixel shader.
//	deviceContext->PSSetShaderResources(0, 1, &texture);
//}

void MultiLightShader::setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& worldMatrix, const XMMATRIX& viewMatrix, const XMMATRIX& projectionMatrix, ID3D11ShaderResourceView* texture,
	XMFLOAT4 diffuseColor[], 
	XMFLOAT3 lightPosition[])
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	LightPositionBufferType* dataPtr2;
	LightColourBufferType* dataPtr3;
	unsigned int bufferNumber;
	XMMATRIX tworld, tview, tproj;
	
	// Transpose the matrices to prepare them for the shader.
	tworld = XMMatrixTranspose(worldMatrix);
	tview = XMMatrixTranspose(viewMatrix);
	tproj = XMMatrixTranspose(projectionMatrix);
	
	// Lock the constant buffer so it can be written to.
	result = deviceContext->Map(matrixBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	// Get a pointer to the data in the constant buffer.
	dataPtr = (MatrixBufferType*)mappedResource.pData;
	// Copy the matrices into the constant buffer.
	dataPtr->world = tworld;                // worldMatrix;
	dataPtr->view = tview;                  // viewMatrix
	dataPtr->projection = tproj;            // projectionMatrix
	// Unlock the constant buffer.
	deviceContext->Unmap(matrixBuffer_, 0);
	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;
	// Now set the constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &matrixBuffer_);
	
	//Additional

	// Lock the light position constant buffer so it can be written to.
	result = deviceContext->Map(lightPositionBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	// Get a pointer to the data in the constant buffer.
	dataPtr2 = (LightPositionBufferType*)mappedResource.pData;
	// Copy the light position variables into the constant buffer.
	dataPtr2->lightPosition[0] = lightPosition[0];
	dataPtr2->lightPosition[1] = lightPosition[1];
	dataPtr2->lightPosition[2] = lightPosition[2];
	dataPtr2->lightPosition[3] = lightPosition[3];
	// Unlock the constant buffer.
	deviceContext->Unmap(lightPositionBuffer_, 0);
	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 1;
	// Finally set the constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &lightPositionBuffer_);
	// Set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &texture);

	// Lock the light color constant buffer so it can be written to.
	result = deviceContext->Map(lightColorBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	// Get a pointer to the data in the constant buffer.
	dataPtr3 = (LightColourBufferType*)mappedResource.pData;
	// Copy the light color variables into the constant buffer.
	dataPtr3->diffuseColor[0] = diffuseColor[0];
	dataPtr3->diffuseColor[1] = diffuseColor[1];
	dataPtr3->diffuseColor[2] = diffuseColor[2];
	dataPtr3->diffuseColor[3] = diffuseColor[3];
	// Unlock the constant buffer.
	deviceContext->Unmap(lightColorBuffer_, 0);
	// Set the position of the constant buffer in the pixel shader.
	bufferNumber = 0;

	// Finally set the constant buffer in the pixel shader with the updated values.
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &lightColorBuffer_);
}

//void MultiLightShader::setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix, ID3D11ShaderResourceView* texture, Light* light)
//{
//	HRESULT result;
//	D3D11_MAPPED_SUBRESOURCE mappedResource;
//	MatrixBufferType* dataPtr;
//	LightBufferType* lightPtr;
//	unsigned int bufferNumber;
//	XMMATRIX tworld, tview, tproj;
//
//
//	// Transpose the matrices to prepare them for the shader.
//	tworld = XMMatrixTranspose(worldMatrix);
//	tview = XMMatrixTranspose(viewMatrix);
//	tproj = XMMatrixTranspose(projectionMatrix);
//
//	// Lock the constant buffer so it can be written to.
//	result = deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
//
//	// Get a pointer to the data in the constant buffer.
//	dataPtr = (MatrixBufferType*)mappedResource.pData;
//
//	// Copy the matrices into the constant buffer.
//	dataPtr->world = tworld;// worldMatrix;
//	dataPtr->view = tview;
//	dataPtr->projection = tproj;
//
//	// Unlock the constant buffer.
//	deviceContext->Unmap(matrixBuffer, 0);
//
//	// Set the position of the constant buffer in the vertex shader.
//	bufferNumber = 0;
//
//	// Now set the constant buffer in the vertex shader with the updated values.
//	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &matrixBuffer);
//
//	//Additional
//	// Send light data to pixel shader
//	deviceContext->Map(lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
//	lightPtr = (LightBufferType*)mappedResource.pData;
//	lightPtr->ambient = light->getAmbientColour();
//	lightPtr->diffuse = light->getDiffuseColour();
//	lightPtr->direction = light->getDirection();
//	lightPtr->specularPower = light->getSpecularPower();
//	lightPtr->specular = light->getSpecularColour();
//	//lightPtr->padding = 0.0f;
//	deviceContext->Unmap(lightBuffer, 0);
//	bufferNumber = 0;
//	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &lightBuffer);
//
//	// Set shader texture resource in the pixel shader.
//	deviceContext->PSSetShaderResources(0, 1, &texture);
//}

void MultiLightShader::render(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// Set the sampler state in the pixel shader.
	//deviceContext->VSSetSamplers(0, 1, &sampleState);
	deviceContext->PSSetSamplers(0, 1, &sampleState_);

	// Base render function.
	BaseShader::render(deviceContext, indexCount);
}



