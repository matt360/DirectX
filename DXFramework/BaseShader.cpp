// base shader
// Base class for shader object. Handles loading in shader files (vertex, pixel, domain, hull and geometry).
// Handle render/sending to GPU for processing.
#include "baseshader.h"

// Store pointer to render device and handle to window.
// When the resources are being created and and interfaced with,
// the device interface is used.
BaseShader::BaseShader(ID3D11Device* device, HWND lhwnd)
{
	renderer = device;
	hwnd = hwnd;
}

// Release resources (if used).
BaseShader::~BaseShader()
{
	if (pixelShader_)
	{
		pixelShader_->Release();
		pixelShader_ = nullptr;
	}

	if (vertexShader_)
	{
		vertexShader_->Release();
		vertexShader_ = nullptr;
	}

	if (hullShader_)
	{
		hullShader_->Release();
		hullShader_ = nullptr;
	}

	if (domainShader_)
	{
		domainShader_->Release();
		domainShader_ = nullptr;
	}

	if (geometryShader_)
	{
		geometryShader_->Release();
		geometryShader_ = nullptr;
	}

	if (computeShader_)
	{
		computeShader_->Release();
		computeShader_ = nullptr;
	}
}

// Given pre-compiled file, load and create vertex shader.
void BaseShader::loadVertexShader(WCHAR* filename)
{
	ID3DBlob* vertexShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
	unsigned int numElements;
	
	vertexShaderBuffer = 0;

	// check file extension for correct loading function.
	std::wstring fn(filename);
	std::string::size_type idx;
	std::wstring extension;

	idx = fn.rfind('.');

	if (idx != std::string::npos)
	{
		extension = fn.substr(idx + 1);
	}
	else
	{
		// No extension found
		MessageBox(hwnd, L"Error finding vertex shader file", L"ERROR", MB_OK);
		exit(0);
	}

	// Load the texture in.
	if (extension != L"cso")
	{
		MessageBox(hwnd, L"Incorrect vertex shader file type", L"ERROR", MB_OK);
		exit(0);
	}
	
	/*
	Here is where we compile the shader programs into buffers.
	We give it the name of the shader file and the buffer to compile the shader into.
	*/
	// Reads compiled shader into buffer (bytecode).
	HRESULT result = D3DReadFileToBlob(filename, &vertexShaderBuffer);
	if (result != S_OK)
	{
		MessageBox(NULL, filename, L"File ERROR", MB_OK);
		exit(0);
	}
	/*if (FAILED(result))
	{
		Log::Get().Write(L"Failed to create blend state");
		return(-1);
	}*/
	
	/* 
	Once the vertex shader code has successfully compiled into buffer
	we then use those buffers to create the shader objects themselves. 
	We will use these pointers to interface with the vertex shader from this point forward.
	*/
	// Create the vertex shader from the buffer.
	renderer->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &vertexShader_);
	
	/*
	The next step is to create the layout of the vertex data that will be processed by the shader.
	As this shader uses a position, texcoord and normal vectors we need to create all of them in the layout specifying the size of all of them as well.
	*/
	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the MeshClass and in the shader.
	polygonLayout[0].SemanticName = "POSITION"; // The semantic name is the first thing to fill out in the layout, this allows the shader to determine the usage of this element of the layout.
	polygonLayout[0].SemanticIndex = 0; // As we have three different elements we use POSITION for the first one,  TEXCOORD0 for the second and NORMAL for the third.
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT; // The next important part of the layout is the Format. For the position vector we use DXGI_FORMAT_R32G32B32_FLOAT
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0; // The final thing you need to pay attention to is the AlignedByteOffset which indicates how the data is spaced in the buffer. For this layout we are telling it the first 12 bytes are position ->
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT; // For the TEXCOORD0 vector we use DXGI_FORMAT_R32G32_FLOAT. -> and the next 8 bytes will be texcoord -->
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT; // For the NORMAL Format we use DXGI_FORMAT_R32G32B32_FLOAT. --> and the last 12 bytes will be normal, AlignedByteOffset shows where each element begins.
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT; // You can use D3D11_APPEND_ALIGNED_ELEMENT instead of placing your own values in AlignedByteOffset and it will figure out the spacing for you.
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	/*
	Once the layout description has been setup we can get the size of it and then create the input layout using the D3D device.
	*/
	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	renderer->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &layout_);
	
	// Also release the vertex shader buffer since they are no longer needed once the layout has been created.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	/*
	The final thing that needs to be setup to utilize the shader is the constant buffer.
	Currently we set up the constant buffers in ColourShader, TextureShader, LightShader ect. shader handler class.
	*/
}

// Given pre-compiled file, load and create pixel shader.
void BaseShader::loadPixelShader(WCHAR* filename)
{
	ID3DBlob* pixelShaderBuffer;

	// check file extension for correct loading function.
	std::wstring fn(filename);
	std::string::size_type idx;
	std::wstring extension;

	idx = fn.rfind('.');

	if (idx != std::string::npos)
	{
		extension = fn.substr(idx + 1);
	}
	else
	{
		// No extension found
		MessageBox(hwnd, L"Error finding pixel shader file", L"ERROR", MB_OK);
		exit(0);
	}

	// Load the texture in.
	if (extension != L"cso")
	{
		MessageBox(hwnd, L"Incorrect pixel shader file type", L"ERROR", MB_OK);
		exit(0);
	}

	// Reads compiled shader into buffer (bytecode).
	HRESULT result = D3DReadFileToBlob(filename, &pixelShaderBuffer);
	if (result != S_OK)
	{
		MessageBox(NULL, filename, L"File not found", MB_OK);
		exit(0);
	}
	// Create the pixel shader from the buffer.
	renderer->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &pixelShader_);
	
	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;
}

// Given pre-compiled file, load and create hull shader.
void BaseShader::loadHullShader(WCHAR* filename)
{
	ID3D10Blob* hullShaderBuffer;
	
	// check file extension for correct loading function.
	std::wstring fn(filename);
	std::string::size_type idx;
	std::wstring extension;

	idx = fn.rfind('.');

	if (idx != std::string::npos)
	{
		extension = fn.substr(idx + 1);
	}
	else
	{
		// No extension found
		MessageBox(hwnd, L"Error finding hull shader file", L"ERROR", MB_OK);
		exit(0);
	}

	// Load the texture in.
	if (extension != L"cso")
	{
		MessageBox(hwnd, L"Incorrect hull shader file type", L"ERROR", MB_OK);
		exit(0);
	}

	// Reads compiled shader into buffer (bytecode).
	HRESULT result = D3DReadFileToBlob(filename, &hullShaderBuffer);
	if (result != S_OK)
	{
		MessageBox(NULL, filename, L"File not found", MB_OK);
		exit(0);
	}
	// Create the hull shader from the buffer.
	renderer->CreateHullShader(hullShaderBuffer->GetBufferPointer(), hullShaderBuffer->GetBufferSize(), NULL, &hullShader_);
	
	hullShaderBuffer->Release();
	hullShaderBuffer = 0;
}

// Given pre-compiled file, load and create domain shader.
void BaseShader::loadDomainShader(WCHAR* filename)
{
	ID3D10Blob* domainShaderBuffer;

	// check file extension for correct loading function.
	std::wstring fn(filename);
	std::string::size_type idx;
	std::wstring extension;

	idx = fn.rfind('.');

	if (idx != std::string::npos)
	{
		extension = fn.substr(idx + 1);
	}
	else
	{
		// No extension found
		MessageBox(hwnd, L"Error finding domain shader file", L"ERROR", MB_OK);
		exit(0);
	}

	// Load the texture in.
	if (extension != L"cso")
	{
		MessageBox(hwnd, L"Incorrect vertex domain file type", L"ERROR", MB_OK);
		exit(0);
	}

	// Reads compiled shader into buffer (bytecode).
	HRESULT result = D3DReadFileToBlob(filename, &domainShaderBuffer);
	if (result != S_OK)
	{
		MessageBox(NULL, filename, L"File not found", MB_OK);
		exit(0);
	}
	// Create the domain shader from the buffer.
	renderer->CreateDomainShader(domainShaderBuffer->GetBufferPointer(), domainShaderBuffer->GetBufferSize(), NULL, &domainShader_);
	
	domainShaderBuffer->Release();
	domainShaderBuffer = 0;
}

// Given pre-compiled file, load and create geometry shader.
void BaseShader::loadGeometryShader(WCHAR* filename)
{
	ID3D10Blob* geometryShaderBuffer;

	// check file extension for correct loading function.
	std::wstring fn(filename);
	std::string::size_type idx;
	std::wstring extension;

	idx = fn.rfind('.');

	if (idx != std::string::npos)
	{
		extension = fn.substr(idx + 1);
	}
	else
	{
		// No extension found
		MessageBox(hwnd, L"Error finding geometry shader file", L"ERROR", MB_OK);
		exit(0);
	}

	// Load the texture in.
	if (extension != L"cso")
	{
		MessageBox(hwnd, L"Incorrect geometry shader file type", L"ERROR", MB_OK);
		exit(0);
	}

	// Reads compiled shader into buffer (bytecode).
	HRESULT result = D3DReadFileToBlob(filename, &geometryShaderBuffer);
	if (result != S_OK)
	{
		MessageBox(NULL, filename, L"File not found", MB_OK);
		exit(0);
	}
	// Create the domain shader from the buffer.
	renderer->CreateGeometryShader(geometryShaderBuffer->GetBufferPointer(), geometryShaderBuffer->GetBufferSize(), NULL, &geometryShader_);

	geometryShaderBuffer->Release();
	geometryShaderBuffer = 0;
}

void BaseShader::loadComputeShader(WCHAR* filename)
{
	/// https://msdn.microsoft.com/en-us/library/windows/desktop/bb173507(v=vs.85).aspx
	/// ID3D11Blob interface has these methods: 
	/// - GetBufferPointer - Get a pointer to the data.
	/// - GetBufferSize - Get the size.
	/// An ID3D10Blob is obtained by calling D3D10CreateBlob.
	/// ID3DBlob is version neutral and can be used in code for any Direct3D version.
	/// Blobs can be used as a data buffer, storing vertex, adjacency, and material information
	/// during mesh optimization and loading operations. Also, these objects are used to return
	/// object code and error messages in APIs that compile vertex, geometry and pixel shaders.
	ID3D10Blob* computeShaderBuffer;

	// check file extension for correct loading function.
	std::wstring fn(filename);
	std::string::size_type idx;
	std::wstring extension;

	idx = fn.rfind('.');

	if (idx != std::string::npos)
	{
		extension = fn.substr(idx + 1);
	}
	else
	{
		// No extension found
		MessageBox(hwnd, L"Error finding compute shader file", L"ERROR", MB_OK); // TODO check if the message is correct
		exit(0);
	}

	// Load the texture in.
	if (extension != L"cso")
	{
		MessageBox(hwnd, L"Incorrect compute domain file type", L"ERROR", MB_OK); // TODO check if the message is correct
		exit(0);
	}

	// Reads compiled shader into buffer (bytecode).
	HRESULT result = D3DReadFileToBlob(filename, &computeShaderBuffer);
	if (result != S_OK)
	{
		MessageBox(NULL, filename, L"File not found", MB_OK);
		exit(0);
	}

	// Create the domain shader from the buffer.
	renderer->CreateComputeShader(computeShaderBuffer->GetBufferPointer(), computeShaderBuffer->GetBufferSize(), NULL, &computeShader_);

	computeShaderBuffer->Release();
	computeShaderBuffer = 0;
}

// The render function sets the shader parameters and then draws the prepared model vertices using the shader.
// De/Activate shader stages and send shaders to GPU.
void BaseShader::render(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// Set the vertex input layout.
	deviceContext->IASetInputLayout(layout_);

	// if vertex shader and pixel shader are not null the set VS and PS
	/*vertexShader ? deviceContext->VSSetShader(vertexShader, NULL, 0) : deviceContext->VSSetShader(NULL, NULL, 0);*/
	/*pixelShader ? deviceContext->PSSetShader(pixelShader, NULL, 0) : deviceContext->VSSetShader(NULL, NULL, 0);*/

	// Set the vertex and pixel shaders that will be used to render.
	deviceContext->VSSetShader(vertexShader_, NULL, 0);
	deviceContext->PSSetShader(pixelShader_, NULL, 0);
	
	// if Hull shader is not null then set HS and DS
	/*hullShader ? deviceContext->HSSetShader(hullShader_, NULL, 0), deviceContext->DSSetShader(domainShader, NULL, 0) :
	             deviceContext->HSSetShader(NULL, NULL, 0), deviceContext->DSSetShader(NULL, NULL, 0);*/
	if (hullShader_)
	{
		deviceContext->HSSetShader(hullShader_, NULL, 0);
		deviceContext->DSSetShader(domainShader_, NULL, 0);
	}
	else
	{
		deviceContext->HSSetShader(NULL, NULL, 0);
		deviceContext->DSSetShader(NULL, NULL, 0);
	}

	// if geometry shader is not null then set GS
	/*geometryShader_ ? deviceContext->GSSetShader(geometryShader_, NULL, 0) : deviceContext->GSSetShader(NULL, NULL, 0);*/
	if (geometryShader_)
	{
		deviceContext->GSSetShader(geometryShader_, NULL, 0);
	}
	else
	{
		deviceContext->GSSetShader(NULL, NULL, 0);
	}

	// if compute shader is not null then set CS
	/*computeShader_ ? deviceContext->CSSetShader(computeShader_, NULL, 0) : deviceContext->CSSetShader(NULL, NULL, 0);*/
	if (computeShader_)
	{
		deviceContext->CSSetShader(computeShader_, NULL, 0);
	}
	else
	{
		deviceContext->CSSetShader(NULL, NULL, 0);
	}

	// Render the triangle.
	deviceContext->DrawIndexed(indexCount, 0, 0);
}
