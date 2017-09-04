// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "../DXFramework/baseapplication.h"
#include "D3D.h"
#include "../DXFramework/planemesh.h"
#include "LightShader.h"
#include "../DXFramework/Light.h"

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
	LightShader* lightShader;
	PlaneMesh* mesh;
	Light* light;

};

#endif