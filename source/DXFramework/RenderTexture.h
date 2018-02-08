// Render texture object
// Is a texture object that can be used as an alternative render target. Store what is rendered to it.
// Size can be speicified but traditionally this will match window size.
// Used in post processing and multi-render stages.
/*
The RenderTexture class allows you to set it as the render target instead of the back buffer. 
It also allows you to retrieve the data rendered to it in the form of a ID3D11ShaderResourceView.
*/

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
	int textureWidth_, textureHeight_;
	ID3D11Texture2D* renderTargetTexture_;           // resource
	/*
	Render Target View (RTV)
	Render targets enable a scene to be rendered to a temporary intermediate buffer, 
	rather than to the back buffer to be rendered to the screen. 
	This feature enables use of the complex scene that might be rendered, 
	perhaps as a reflection texture or other purpose within the graphics pipeline, 
	or perhaps to add additional pixel shader effects to the scene before rendering.
	*/
	ID3D11RenderTargetView* renderTargetView_;       // The render target view (RTV) is used to attach a resource to receive the output of the rendering pipeline (A render-target-view interface identifies the render-target subresources that can be accessed during rendering.)
	ID3D11ShaderResourceView* shaderResourceView_;   // The shader resource view (SRV) provides read access to a resource to the programmable shader stages of the pipepline. (Create a shader-resource view for accessing data in a resource.)
	ID3D11Texture2D* depthStencilBuffer_;
	ID3D11DepthStencilView* depthStencilView_;       // The depth stencil view (DSV) is similar to a render target view in that it is attached for receiving output from the rendering pipeline.
	D3D11_VIEWPORT viewport_;
	XMMATRIX projectionMatrix_;
	XMMATRIX orthoMatrix_;
};

#endif