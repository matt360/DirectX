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
	void resetExample(Camera* camera);

private:
	// shader handler
	MultiLightShader* shader_;

	vector<Light*> lights_;
	vector<XMFLOAT4*> light_colours_;
	vector<XMFLOAT4*> light_positions_;
	string texture;
	// TODO delete if not used SphereMesh *lightSphere0_, *lightSphere1_, *lightSphere2_, *lightSphere3_;
};

