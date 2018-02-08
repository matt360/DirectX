// render texture
// alternative render target
#include "rendertexture.h"

/*
The function creates 
a render target texture
by first setting up 
the description of 
the texture 
and then creating 
that texture. 
It then uses 
that texture
to setup 
a render target view 
so that 
the texture
can be drawn to as 
a render target. 
The third and final thing we do is create 
a shader resource view 
of 
that texture 
so that we can supply 
the rendered data
to 
calling objects.
*/
// Initialise texture object based on provided dimensions. Usually to match window.
RenderTexture::RenderTexture(ID3D11Device* device, int ltextureWidth, int ltextureHeight, float screenNear, float screenFar)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT result;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

	textureWidth_ = ltextureWidth;
	textureHeight_ = ltextureHeight;

	ZeroMemory(&textureDesc, sizeof(textureDesc));

	// The function creates a render target texture by first setting up the description of the texture...
	// Setup the render target texture description.
	textureDesc.Width = textureWidth_;
	textureDesc.Height = textureHeight_;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	// ...and then creating that texture...
	// Create the render target texture.
	result = device->CreateTexture2D(&textureDesc, NULL, &renderTargetTexture_);
	
	// ...It then uses that texture to setup a render target view so that the texture can be drawn to as a render target...
	// Setup the description of the render target view.
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;
	// Create the render target view.
	result = device->CreateRenderTargetView(renderTargetTexture_, &renderTargetViewDesc, &renderTargetView_);
	
	// Setup the description of the shader resource view.
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;
	// ...The third and final thing we do is create a shader resource view of that texture so that we can supply the rendered data to calling objects.
	// Create the shader resource view.
	result = device->CreateShaderResourceView(renderTargetTexture_, &shaderResourceViewDesc, &shaderResourceView_);
	
	// Set up the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	depthBufferDesc.Width = textureWidth_;
	depthBufferDesc.Height = textureHeight_;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
	result = device->CreateTexture2D(&depthBufferDesc, NULL, &depthStencilBuffer_);
	
	// Set up the depth stencil view description.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	result = device->CreateDepthStencilView(depthStencilBuffer_, &depthStencilViewDesc, &depthStencilView_);
	
	// Setup the viewport for rendering.
	viewport_.Width = (float)textureWidth_;
	viewport_.Height = (float)textureHeight_;
	viewport_.MinDepth = 0.0f;
	viewport_.MaxDepth = 1.0f;
	viewport_.TopLeftX = 0.0f;
	viewport_.TopLeftY = 0.0f;

	// Setup the projection matrix.
	projectionMatrix_ = XMMatrixPerspectiveFovLH(((float)XM_PI / 4.0f), ((float)textureWidth_ / (float)textureHeight_), screenNear, screenFar);

	// Create an orthographic projection matrix for 2D rendering.
	orthoMatrix_ = XMMatrixOrthographicLH((float)textureWidth_, (float)textureHeight_, screenNear, screenFar);
}

// Release resources.
RenderTexture::~RenderTexture()
{
	if (depthStencilView_)
	{
		depthStencilView_->Release();
		depthStencilView_ = 0;
	}

	if (depthStencilBuffer_)
	{
		depthStencilBuffer_->Release();
		depthStencilBuffer_ = 0;
	}

	if (shaderResourceView_)
	{
		shaderResourceView_->Release();
		shaderResourceView_ = 0;
	}

	if (renderTargetView_)
	{
		renderTargetView_->Release();
		renderTargetView_ = 0;
	}

	if (renderTargetTexture_)
	{
		renderTargetTexture_->Release();
		renderTargetTexture_ = 0;
	}
}

// Set this renderTexture as the current render target.
// All rendering is now store here, rather than the back buffer.
/*
The setRenderTarget function sets the render target view in this class as the current rendering location for all graphics to be rendered to.
*/
void RenderTexture::setRenderTarget(ID3D11DeviceContext* deviceContext)
{
	deviceContext->OMSetRenderTargets(1, &renderTargetView_, depthStencilView_);
	deviceContext->RSSetViewports(1, &viewport_);
}

// Clear render texture to specified colour. Similar to clearing the back buffer, ready for the next frame.
/*
clearRenderTarget mimics the functionality of the D3DClass::BeginScene function 
except for that it operates on the render target view within this class. 

This should be called each frame before rendering to this render target view.
*/
void RenderTexture::clearRenderTarget(ID3D11DeviceContext* deviceContext, float red, float green, float blue, float alpha)
{
	float color[4];
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	// Clear the back buffer and depth buffer.
	deviceContext->ClearRenderTargetView(renderTargetView_, color);
	deviceContext->ClearDepthStencilView(depthStencilView_, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

/*
The getShaderResourceView function returns the render to texture data as a shader resource view. 
This way whatever has been rendered to the render target view can be used as a texture in different shaders that call this function. 

Where you would normally send a texture into a shader you can instead 
send a call to this function in its place and the render to texture will be used.
*/
ID3D11ShaderResourceView* RenderTexture::getShaderResourceView()
{
	return shaderResourceView_;
}

XMMATRIX RenderTexture::getProjectionMatrix()
{
	return projectionMatrix_;
}

XMMATRIX RenderTexture::getOrthoMatrix()
{
	return orthoMatrix_;
}

int RenderTexture::getTextureWidth()
{
	return textureWidth_;
}

int RenderTexture::getTextureHeight()
{
	return textureHeight_;
}