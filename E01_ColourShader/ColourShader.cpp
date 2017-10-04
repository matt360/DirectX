// texture shader.cpp
#include "colourshader.h"

// The ColorShader class is what we will use to invoke our HLSL shaders for drawing the 3D models that are on the GPU.

ColourShader::ColourShader(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd)
{
	initShader(L"colour_vs.cso", L"colour_ps.cso");
}


ColourShader::~ColourShader()
{
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

	//Release base shader components
	BaseShader::~BaseShader();
}

/*
 Now we will start with one of the more important functions to this tutorial which is called initShader.
 This function is what actually loads the shader files and makes it usable to DirectX and the GPU.
 You will also see the setup of the layout and how the vertex buffer data is going to look on the graphics pipeline in the GPU. 
 The layout will need the match the VertexType in the modelclass.h file as well as the one defined in the color.vs file
*/
void ColourShader::initShader(WCHAR* vsFilename, WCHAR* psFilename)
{
	/*
	As we saw in the vertex shader we currently have just one constant buffer so we only need to setup one here so we can interface with the shader.
	*/
	D3D11_BUFFER_DESC matrixBufferDesc;
	
	// Load (+ compile) shader files
	loadVertexShader(vsFilename);
	loadPixelShader(psFilename);

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC; // The buffer usage needs to be set to dynamic since we will be updating it each frame.
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // The bind flags indicate that this buffer will be a constant buffer.
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // The cpu access flags need to match up with the usage so it is set to D3D11_CPU_ACCESS_WRITE. 
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	/*
	Once we fill out the description we can then create the constant buffer interface and then use that to access the internal variables in the shader using the function setShaderParameters.
	*/
	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	renderer->CreateBuffer(&matrixBufferDesc, NULL, &matrixBuffer_);
}


void ColourShader::setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;
	XMMATRIX tworld, tview, tproj;


	// Transpose the matrices to prepare them for the shader.
	tworld = XMMatrixTranspose(worldMatrix);
	tview = XMMatrixTranspose(viewMatrix);
	tproj = XMMatrixTranspose(projectionMatrix);

	/*
	Lock the matrixBuffer_, set the new matrices inside it, and then unlock it.
	*/
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

	/*
	Now set the updated matrix buffer in the HLSL vertex shader.
	*/
	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Now set the constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &matrixBuffer_);
}




