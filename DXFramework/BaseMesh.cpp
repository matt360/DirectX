// BaseMesh.cpp
// Base mesh class, for inheriting base mesh functionality.

#include "basemesh.h"

BaseMesh::BaseMesh()
{
}

// Release base objects (index, vertex buffers and texture object.
BaseMesh::~BaseMesh()
{
	if (indexBuffer)
	{
		indexBuffer->Release();
		indexBuffer = 0;
	}

	if (vertexBuffer)
	{
		vertexBuffer->Release();
		vertexBuffer = 0;
	}
}

int BaseMesh::getIndexCount()
{
	return indexCount;
}

/*
sendData function puts the geometry on the video card to prepare it for drawing by the shader.
*/

// Sends geometry data to the GPU. Default primitive topology is TriangleList.
// To render alternative topologies this function needs to be overwritten.
void BaseMesh::sendData(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;
	
	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
}




