// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "../DXFramework/baseapplication.h"
#include "D3D.h"
#include "../DXFramework/planemesh.h"
#include "../DXFramework/Light.h"
#include "../DXFramework/CubeMesh.h"
#include "../DXFramework/DXF.h"
#include "TessellationShader.h"


class App1 : public BaseApplication
{
public:

	App1();
	~App1();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input*);

	bool frame();

protected:
	bool render();

private:
	
	TessellationMesh* mesh;
	TessellationShader* shader;
	

};

#endif