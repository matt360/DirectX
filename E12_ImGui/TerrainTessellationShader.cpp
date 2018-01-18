// texture shader.cpp
#include "TerrainTessellationShader.h"

// When resources are being created and interfaced with,
// the device interface is used.
TerrainTessellationShader::TerrainTessellationShader(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd)
{
	// compiled shader object
	initShader(L"terrain_tessellation_vs.cso", L"terrain_tessellation_hs.cso", L"terrain_tessellation_ds.cso", L"terrain_tessellation_gs.cso", L"terrain_tessellation_ps.cso");
}

TerrainTessellationShader::~TerrainTessellationShader()
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


void TerrainTessellationShader::initShader(WCHAR* vsFilename, WCHAR* psFilename)
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
	// Create the constant buffer pointer so we can access the domain shader constant buffer from within this class.
	renderer->CreateBuffer(&cameraBufferDesc, NULL, &cameraBuffer_);

	// Time buffer
	timeBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	timeBufferDesc.ByteWidth = sizeof(TimeBufferType);
	timeBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	timeBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	timeBufferDesc.MiscFlags = 0;
	timeBufferDesc.StructureByteStride = 0;

	renderer->CreateBuffer(&timeBufferDesc, NULL, &timeBuffer_);
}

void TerrainTessellationShader::initShader(WCHAR* vsFilename, WCHAR* hsFilename, WCHAR* dsFilename, WCHAR* gsFilename, WCHAR* psFilename)
{
	// InitShader must be overwritten and it will load both vertex and pixel shaders + setup buffers
	initShader(vsFilename, psFilename);

	// Load other required shaders.
	loadHullShader(hsFilename);
	loadDomainShader(dsFilename);
	loadGeometryShader(gsFilename);
}

void TerrainTessellationShader::setShaderParameters(
	ID3D11DeviceContext* deviceContext, 
	const XMMATRIX& worldMatrix, 
	const XMMATRIX& viewMatrix, 
	const XMMATRIX& projectionMatrix,
	ID3D11ShaderResourceView* texture, 
	Light* light, 
	Camera* camera, 
	float time, 
	float height, 
	float frequency)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	LightBufferType* lightPtr;
	CameraBufferType* cameraPtr;
	TimeBufferType* timePtr;
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
	deviceContext->GSSetConstantBuffers(bufferNumber, 1, &matrixBuffer_);

	// TIME
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
	deviceContext->DSSetConstantBuffers(bufferNumber, 1, &timeBuffer_);

	// CAMERA
	// Lock the constant buffer so it can be written to.
	result = deviceContext->Map(cameraBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	// Get a pointer to the data in the constant buffer.
	cameraPtr = (CameraBufferType*)mappedResource.pData;
	// Copy the matrices into the constant buffer.
	cameraPtr->cameraPosition = camera->getPosition();
	cameraPtr->padding = 0.0f;
	// Unlock the constant buffer.
	deviceContext->Unmap(cameraBuffer_, 0);
	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;
	// Now set the constant buffer in the hull shader with the updated values.
	deviceContext->HSSetConstantBuffers(bufferNumber, 1, &cameraBuffer_);
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &cameraBuffer_);

	// LIGHT
	// Send light data to pixel shader
	deviceContext->Map(lightBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	lightPtr = (LightBufferType*)mappedResource.pData;
	lightPtr->ambient = light->getAmbientColour();
	lightPtr->diffuse = light->getDiffuseColour();
	lightPtr->direction = light->getDirection();
	lightPtr->specularPower = light->getSpecularPower();
	lightPtr->specular = light->getSpecularColour();
	lightPtr->position = light->getPosition();
	lightPtr->padding = 0.0f;
	deviceContext->Unmap(lightBuffer_, 0);
	bufferNumber = 0;
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &lightBuffer_);

	// Set shader texture resource in the pixel and vertex shader.
	deviceContext->DSSetShaderResources(0, 1, &texture);
	deviceContext->PSSetShaderResources(0, 1, &texture);
	/*deviceContext->PSSetShaderResources(0, 1, &mapping_texture_1);
	deviceContext->PSSetShaderResources(1, 1, &mapping_texture_2);
	deviceContext->PSSetShaderResources(2, 1, &height_texture);*/
}

void TerrainTessellationShader::render(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// Set the sampler state in the pixel shader.
	deviceContext->PSSetSamplers(0, 1, &sampleState_);

	// Base render function.
	BaseShader::render(deviceContext, indexCount);
}



