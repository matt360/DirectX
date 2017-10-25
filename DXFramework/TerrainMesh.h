// TerrainMesh.h
// Generates a plane mesh based on resolution.
// Resolution is the width and depth of the generated plane, built from unit quads.

#pragma once

#include "BaseMesh.h"
#include <string>

class TerrainMesh : public BaseMesh
{
public:
	TerrainMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int x_resolution = 100, int y_resolution = 100);
	~TerrainMesh();

protected:
	void initBuffers(ID3D11Device* device);
	int x_resolution_;
	int y_resolution_;
};

