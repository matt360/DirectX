// BaseMesh.cpp
// Base mesh class, for inheriting base mesh functionality.

#include "basemesh.h"

BaseMesh::BaseMesh()
{
}

// Release base objects (index, vertex buffers and texture object.
BaseMesh::~BaseMesh()
{
	if (indexBuffer_)
	{
		indexBuffer_->Release();
		indexBuffer_ = 0;
	}

	if (vertexBuffer_)
	{
		vertexBuffer_->Release();
		vertexBuffer_ = 0;
	}
}

/*
// getIndexCount returns the number of indexes in the "*Mesh". The shader handler will need this information to draw this model.
*/
int BaseMesh::getIndexCount()
{
	return indexCount_;
}

/*
 * sendData function puts the geometry on the video card to prepare it for drawing by the shader.
 * The purpose of this function is to set the vertex buffer and index buffer as active on the input assembler in the GPU.
 * Once the GPU has an active vertex buffer it can then use the shader to render that buffer.
 * This function also defines how those buffers should be drawn such as triangles, lines, fans, and so forth. 
 * In this tutorial we set the vertex buffer and index buffer as active on the input assembler 
 * and tell the GPU that the buffers should be drawn as triangles using the IASetPrimitiveTopology DirectX function.
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
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer_, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(indexBuffer_, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}




