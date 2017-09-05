// SquareMesh.h
// Simple single square mesh (for example purposes).
// Mesh contains texture coordinates and normals.

#ifndef _SQUARE_MESH_H
#define _SQAURE_MESH_H

#include "..\DXFramework\BaseMesh.h"

using namespace DirectX;

class SquareMesh : public BaseMesh
{
public:
	SquareMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	~SquareMesh();

protected:
	void initBuffers(ID3D11Device* device);
};
#endif // !_SQUARE_MESH_H


