#pragma once
#include "Example.h"
#include "MultiLightShader.h"

class MultiLightExample : public Example
{
public:
	MultiLightExample();
	~MultiLightExample();

	void init(D3D* renderer, HWND hwnd);
	void initShader(D3D* renderer, HWND hwnd);
	void initVariables();
	void initLight();
	void render(D3D* renderer, Camera* camera, TextureManager* textureMgr);
	void gui(Camera* camera);

	// shader handler
	MultiLightShader* shader;

	unsigned number_of_lights_;
	vector<Light*> lights_;
	ImVec4 light0_col, light1_col, light2_col, light3_col;
	//XMFLOAT3 light0_pos, light1_pos, light2_pos, light3_pos;
	vector<XMFLOAT3*> light_positions_;
	//SphereMesh *lightSphere0_, *lightSphere1_, *lightSphere2_, *lightSphere3_;
};

