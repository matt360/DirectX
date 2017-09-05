// SquareMesh.cpp
// Simple square mesh for example purposes. With texture cooridnates and normals.
#include "SquareMesh.h"

SquareMesh::SquareMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	initBuffers(device);
}

// Release resources.
SquareMesh::~SquareMesh()
{
	// Run parent deconstructor
	BaseMesh::~BaseMesh();
}

// Build shape and fill buffers.
void SquareMesh::initBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	vertexCount = 3;
	indexCount = 3;

	vertices = new VertexType[vertexCount];
	indices = new unsigned long[indexCount];

	// Load the vertex array with data
	vertices[0].position = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertices[0].texture = XMFLOAT2(0.5f, 0.0f);
	vertices[0].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);


}
