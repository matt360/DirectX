// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "../DXFramework/baseapplication.h"
#include "D3D.h"
#include "../DXFramework/quadmesh.h"
#include "TextureShader.h"
#include "GeometryShader.h"
#include "../DXFramework/DXF.h"
//#include "../DXFramework/imgui.h"
//#include "../DXFramework/imgui_impl_dx11.h"

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
	GeometryShader* geometryShader;
	TextureShader* textureShader;
	PointMesh* mesh;

	float f;
	
};

#endif