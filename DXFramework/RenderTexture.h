// Render texture object
// Is a texture object that can be used as an alternative render target. Store what is rendered to it.
// Size can be speicified but traditionally this will match window size.
// Used in post processing and multi-render stages.

#ifndef _RENDERTEXTURE_H_
#define _RENDERTEXTURE_H_

#include <d3d11.h>
#include <directxmath.h>

using namespace DirectX;

class RenderTexture
{
public:
	void* operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}

	void operator delete(void* p)
	{
		_mm_free(p);
	}

	RenderTexture(ID3D11Device* device, int textureWidth, int textureHeight, float screenNear, float screenDepth);
	~RenderTexture();

	void setRenderTarget(ID3D11DeviceContext* deviceContext);
	void clearRenderTarget(ID3D11DeviceContext* deviceContext, float red, float green, float blue, float alpha);
	ID3D11ShaderResourceView* getShaderResourceView();

	XMMATRIX getProjectionMatrix();
	XMMATRIX getOrthoMatrix();

	int getTextureWidth();
	int getTextureHeight();

private:
	int textureWidth, textureHeight;
	ID3D11Texture2D* renderTargetTexture;           // resource
	ID3D11RenderTargetView* renderTargetView;       // The render target view (RTV) is used to attach a resource to receive the output of the rendering pipeline
	ID3D11ShaderResourceView* shaderResourceView;   // The shader resource view (SRV) provides read access to a resource to the programmable shader stages of the pipepline.
	ID3D11Texture2D* depthStencilBuffer;
	ID3D11DepthStencilView* depthStencilView;       // The depth stencil view (DSV) is similar to a render target view in that it is attached for receiving output from the rendering pipeline.
	D3D11_VIEWPORT viewport;
	XMMATRIX projectionMatrix;
	XMMATRIX orthoMatrix;
};

#endif