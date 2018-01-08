// texture shader.cpp
#include "TerrainShader.h"

// When resources are being created and interfaced with,
// the device interface is used.
TerrainShader::TerrainShader(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd)
{
	// compiled shader object
	initShader(L"terrain_vs.cso", L"terrain_ps.cso");
}


TerrainShader::~TerrainShader()
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

	// Release the light constant buffer.
	if (lightBuffer_)
	{
		lightBuffer_->Release();
		lightBuffer_ = 0;
	}

	if (cameraBuffer_)
	{
		cameraBuffer_->Release();
		cameraBuffer_ = 0;
	}

	if (timeBuffer_)
	{
		timeBuffer_->Release();
		timeBuffer_ = 0;
	}
	//Release base shader components
	BaseShader::~BaseShader();
}


void TerrainShader::initShader(WCHAR* vsFilename, WCHAR* psFilename)
{
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC lightBufferDesc;
	// Time
	D3D11_BUFFER_DESC timeBufferDesc;
	// Camera
	D3D11_BUFFER_DESC cameraBufferDesc;

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

	// Setup light buffer
	// Setup the description of the light dynamic constant buffer that is in the pixel shader.
	// Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER or CreateBuffer will fail.
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	renderer->CreateBuffer(&lightBufferDesc, NULL, &lightBuffer_);

	// Camera buffer
	cameraBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	cameraBufferDesc.ByteWidth = sizeof(CameraBufferType);
	cameraBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cameraBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cameraBufferDesc.MiscFlags = 0;
	cameraBufferDesc.StructureByteStride = 0;

//	renderer->CreateBuffer(&cameraBufferDesc, NULL, &cameraBuffer);

	// Time buffer
	timeBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	timeBufferDesc.ByteWidth = sizeof(TimeBufferType);
	timeBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	timeBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	timeBufferDesc.MiscFlags = 0;
	timeBufferDesc.StructureByteStride = 0;

	renderer->CreateBuffer(&timeBufferDesc, NULL, &timeBuffer_);
}

void TerrainShader::setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix, ID3D11ShaderResourceView* texture, Light* light, Camera* camera)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	LightBufferType* lightPtr;
	CameraBufferType* cameraPtr;
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
	dataPtr->world = tworld;// worldMatrix;
	dataPtr->view = tview;
	dataPtr->projection = tproj;

	// Unlock the constant buffer.
	deviceContext->Unmap(matrixBuffer_, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Now set the constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &matrixBuffer_);

	// Additional //
	// Camera
	result = deviceContext->Map(cameraBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	cameraPtr = (CameraBufferType*)mappedResource.pData;
	cameraPtr->cameraPosition = camera->getPosition();
	cameraPtr->padding = 0.0f;
	deviceContext->Unmap(cameraBuffer_, 0);
	bufferNumber = 1;
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &cameraBuffer_);

	// Send light data to pixel shader
	result = deviceContext->Map(lightBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	lightPtr = (LightBufferType*)mappedResource.pData;
	lightPtr->ambient = light->getAmbientColour();
	lightPtr->diffuse = light->getDiffuseColour();
	lightPtr->direction = light->getDirection();
	lightPtr->specularPower = light->getSpecularPower();
	lightPtr->specular = light->getSpecularColour();
	lightPtr->position = light->getPosition();
	lightPtr->padding = 0.0f;
	
	//lightPtr->padding = 0.0f;
	deviceContext->Unmap(lightBuffer_, 0);
	bufferNumber = 0;
	//deviceContext->VSSetConstantBuffers(bufferNumber, 1, &lightBuffer);
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &lightBuffer_);

	// Set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &texture);
	deviceContext->VSSetShaderResources(0, 1, &texture);
}

void TerrainShader::setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix, ID3D11ShaderResourceView* texture, Light* light)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	LightBufferType* lightPtr;
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
	dataPtr->world = tworld;// worldMatrix;
	dataPtr->view = tview;
	dataPtr->projection = tproj;

	// Unlock the constant buffer.
	deviceContext->Unmap(matrixBuffer_, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Now set the constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &matrixBuffer_);

	//Additional
	// Send light data to pixel shader
	deviceContext->Map(lightBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	lightPtr = (LightBufferType*)mappedResource.pData;
	lightPtr->ambient = light->getAmbientColour();
	lightPtr->diffuse = light->getDiffuseColour();
	lightPtr->direction = light->getDirection();
	lightPtr->specularPower = light->getSpecularPower();
	lightPtr->specular = light->getSpecularColour();
	//lightPtr->padding = 0.0f;
	deviceContext->Unmap(lightBuffer_, 0);
	bufferNumber = 0;
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &lightBuffer_);

	// Set shader texture resource in the pixel and vertex shader.
	deviceContext->PSSetShaderResources(0, 1, &texture);
	deviceContext->VSSetShaderResources(0, 1, &texture);
}

void TerrainShader::setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix, ID3D11ShaderResourceView* texture, Light* light, float time, float height, float frequency)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	TimeBufferType* timePtr;
	LightBufferType* lightPtr;
	unsigned int bufferNumber;
	XMMATRIX tworld, tview, tproj;


	// Transpose the matrices to prepare them for the shader.
	tworld = XMMatrixTranspose(worldMatrix);
	tview = XMMatrixTranspose(viewMatrix);
	tproj = XMMatrixTranspose(projectionMatrix);

	// Lock the constant buffer so it can be written to.
	deviceContext->Map(matrixBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// Get a pointer to the data in the constant buffer.
	dataPtr = (MatrixBufferType*)mappedResource.pData;
	// Copy the matrices into the constant buffer.
	dataPtr->world = tworld;// worldMatrix;
	dataPtr->view = tview;
	dataPtr->projection = tproj;
	// Unlock the constant buffer.
	deviceContext->Unmap(matrixBuffer_, 0);
	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;
	// Now set the constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &matrixBuffer_);

	// Time
	// Send time data to vertex shader
	deviceContext->Map(timeBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	// Get a pointer to the data in the constant buffer.
	timePtr = (TimeBufferType*)mappedResource.pData;
	// Copy the time balue into the constant buffer.
	timePtr->time = time;
	timePtr->height = height;
	timePtr->frequency = frequency;
	timePtr->padding = 0.0f;
	// Unlock the constant buffer.
	deviceContext->Unmap(timeBuffer_, 0);
	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 1;
	// Now set the constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &timeBuffer_);

	//Additional
	// Send light data to pixel shader
	deviceContext->Map(lightBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	lightPtr = (LightBufferType*)mappedResource.pData;
	lightPtr->ambient = light->getAmbientColour();
	lightPtr->diffuse = light->getDiffuseColour();
	lightPtr->direction = light->getDirection();
	lightPtr->specularPower = light->getSpecularPower();
	lightPtr->specular = light->getSpecularColour();
	//lightPtr->padding = 0.0f;
	deviceContext->Unmap(lightBuffer_, 0);
	bufferNumber = 0;
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &lightBuffer_);

	// Set shader texture resource in the pixel and vertex shader.
	deviceContext->PSSetShaderResources(0, 1, &texture);
	deviceContext->VSSetShaderResources(0, 1, &texture);
}

void TerrainShader::render(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// Set the sampler state in the pixel shader.
	deviceContext->PSSetSamplers(0, 1, &sampleState_);

	// Base render function.
	BaseShader::render(deviceContext, indexCount);
}



