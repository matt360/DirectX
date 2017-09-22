// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "../DXFramework/baseapplication.h"
#include "D3D.h"
#include "../DXFramework/quadmesh.h"
#include "TextureShader.h"
#include "SquareMesh.h"

class App1 : public BaseApplication
{
public:

	App1();
	~App1();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input*);

	bool frame();

protected:
	bool render();
	void gui();

private:
	TextureShader* textureShader;
	QuadMesh* mesh;
	SquareMesh* squareMesh;

	float rotate = 0.0f;
};

#endif