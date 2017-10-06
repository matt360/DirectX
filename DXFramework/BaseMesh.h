// BaseMesh.h
// Base mesh class. The parent of all mesh objects. Provides required functions
// including loading textures, returning index count and transmitting geometry to the GPU

/*
BaseMesh class is responsible for encapsulating the geometry for 3D models. 
*/

#ifndef _BASEMESH_H_
#define _BASEMESH_H_

#include <d3d11.h>
#include <directxmath.h>

using namespace DirectX;

class BaseMesh
{
protected:
	// Here is the definition of our vertex type that will be used with the vertex buffer in derived classes.
	// Also take note that this typedef must match the layout in the "%Shader" handler class that will be looked at later in the tutorial.


	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
	};

public:
	BaseMesh();
	~BaseMesh();

	virtual void sendData(ID3D11DeviceContext* deviceContext);
	int getIndexCount();

protected:
	virtual void initBuffers(ID3D11Device*) = 0;

	ID3D11Buffer *vertexBuffer, *indexBuffer;
	int vertexCount, indexCount;
};

#endif