// terrain mesh
// Quad mesh made of many quads. Default is 100x100
#include "TerrainMesh.h"

// Initialise buffer and load texture.
TerrainMesh::TerrainMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::string height_map, int x_resolution, int y_resolution)
{
	heightMap_ = nullptr;
	x_resolution_ = x_resolution;
	y_resolution_ = y_resolution;

	LoadHeightMap(height_map);
	NormalizeHeightMap();

	initBuffers(device);
}

// Initialise buffer and load texture.
TerrainMesh::TerrainMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int x_resolution, int y_resolution)
{
	x_resolution_ = x_resolution;
	y_resolution_ = y_resolution;

	initBuffers(device);
}

// Release resources.
TerrainMesh::~TerrainMesh()
{
	ShutdownHeightMap();
	// Run parent deconstructor
	BaseMesh::~BaseMesh();
}

bool TerrainMesh::LoadHeightMap(std::string height_map)
{
	FILE* filePtr;
	int error;
	unsigned int count;
	BITMAPFILEHEADER bitmapFileHeader;
	BITMAPINFOHEADER bitmapInfoHeader;
	int imageSize, i, j, k, index;
	unsigned char* bitmapImage;
	unsigned char height;

	/*
	Begin by opening the file and then read it into a unsigned char array. Close the file after we are finished reading the data from it.
	*/
	// Open the height map file in binary.
	error = fopen_s(&filePtr, height_map.c_str(), "rb");
	if (error != 0)
	{
		return false;
	}

	// Read in the file header.
	count = fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	// Read in the bitmap info header.
	count = fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	/*
	Store the size of the terrain so we can use these values for building the vertex and index buffers as well as rendering the terrain.
	*/
	// Save the dimensions of the terrain.
	x_resolution_ = bitmapInfoHeader.biWidth;
	y_resolution_ = bitmapInfoHeader.biHeight;

	// Calculate the size of the bitmap image data.
	imageSize = x_resolution_ * y_resolution_ * 3;

	// Allocate memory for the bitmap image data.
	bitmapImage = new unsigned char[imageSize];
	if (!bitmapImage)
	{
		return false;
	}

	// Move to the beginning of the bitmap data.
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	// Read in the bitmap image data.
	count = fread(bitmapImage, 1, imageSize, filePtr);
	if (count != imageSize)
	{
		return false;
	}

	// Close the file.
	error = fclose(filePtr);
	if (error != 0)
	{
		return false;
	}

	/*
	Now that the bitmap has been read in create the two dimensional height map array and read the buffer into it.
	Note that during the for loop I use the two loop variables (i and j) to be the X (width) and Z (depth) of the terrain. And then I use the bitmap value to be the Y (height) of the terrain.
	You will also see I increment the index into the bitmap (k) by three since we only need one of the color values (red, green, or blue) to be used as the grey scale value.
	*/

	// Create the structure to hold the height map data.
	heightMap_ = new HeightMapType[x_resolution_ * y_resolution_];
	if (!heightMap_)
	{
		return false;
	}

	// Initialize the position in the image data buffer.
	k = 0;

	// Read the image data into the height map.
	for (j = 0; j<y_resolution_; j++)
	{
		for (i = 0; i<x_resolution_; i++)
		{
			height = bitmapImage[k];

			index = (y_resolution_ * j) + i;

			heightMap_[index].x = (float)i;
			heightMap_[index].y = (float)height;
			heightMap_[index].z = (float)j;

			k += 3;
		}
	}

	/*
	Now that we have stored the height map data for the terrain in our own array we can release the bitmap array.
	*/
	// Release the bitmap image data.
	delete[] bitmapImage;
	bitmapImage = 0;

	return true;
}

void TerrainMesh::NormalizeHeightMap()
{
	int i, j;

	for (j = 0; j<y_resolution_; j++)
	{
		for (i = 0; i<x_resolution_; i++)
		{
			heightMap_[(y_resolution_ * j) + i].y /= 15.0f;
		}
	}

	return;
}

void TerrainMesh::ShutdownHeightMap()
{
	if (heightMap_)
	{
		delete[] heightMap_;
		heightMap_ = 0;
	}

	return;
}

// Generate plane (including texture coordinates and normals).
void TerrainMesh::initBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	int index, i, j;
	float positionX, positionZ, u, v, u_increment, v_increment;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	// Calculate the number of vertices in the terrain mesh.
	vertexCount_ = (x_resolution_ - 1) * (y_resolution_ - 1) * 8;
	//vertexCount_ = (x_resolution_ - 1) * (y_resolution_ - 1) * 12;

	indexCount_ = vertexCount_;
	vertices = new VertexType[vertexCount_];
	indices = new unsigned long[indexCount_];


	index = 0;
	// UV coords.
	u = 0;
	v = 0;
    u_increment = 1.0f / x_resolution_;
	v_increment = 1.0f / y_resolution_;

	for (j = 0; j < (y_resolution_- 1); j++)
	{
		for (i = 0; i < (x_resolution_ - 1); i++)
		{
			// Upper left.
			positionX = (float)i;
			positionZ = (float)(j);

			vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
			vertices[index].texture = XMFLOAT2(u, v);
			vertices[index].normal = XMFLOAT3(0.0, 1.0, 0.0);
			indices[index] = index;
			index++;

			// Upper right.
			positionX = (float)(i + 1);
			positionZ = (float)(j + 1);

			vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
			vertices[index].texture = XMFLOAT2(u + u_increment, v + v_increment);
			vertices[index].normal = XMFLOAT3(0.0, 1.0, 0.0);
			indices[index] = index;
			index++;


			// lower left
			positionX = (float)(i);
			positionZ = (float)(j + 1);


			vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
			vertices[index].texture = XMFLOAT2(u, v + v_increment);
			vertices[index].normal = XMFLOAT3(0.0, 1.0, 0.0);
			indices[index] = index;
			index++;

			// Upper left
			positionX = (float)(i);
			positionZ = (float)(j);

			vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
			vertices[index].texture = XMFLOAT2(u, v);
			vertices[index].normal = XMFLOAT3(0.0, 1.0, 0.0);
			indices[index] = index;
			index++;

			// Bottom right
			positionX = (float)(i + 1);
			positionZ = (float)(j);

			vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
			vertices[index].texture = XMFLOAT2(u + u_increment, v);
			vertices[index].normal = XMFLOAT3(0.0, 1.0, 0.0);
			indices[index] = index;
			index++;

			// Upper right.
			positionX = (float)(i + 1);
			positionZ = (float)(j + 1);

			vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
			vertices[index].texture = XMFLOAT2(u + u_increment, v + v_increment);
			vertices[index].normal = XMFLOAT3(0.0, 1.0, 0.0);
			indices[index] = index;
			index++;

			u += u_increment;
		}

		u = 0;
		v += v_increment;
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType)* vertexCount_;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;
	// Now create the vertex buffer.
	device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer_);

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long)* indexCount_;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;
	// Create the index buffer.
	device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer_);

	// Release the arrays now that the buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;
	delete[] indices;
	indices = 0;
}


