#pragma once
// Include system level headers
#include "System.h"
//#include "D3D.h"
#include "BaseApplication.h"
#include "BaseShader.h"
//#include "TextureManager.h"

// Inlcude geometry headers
#include "BaseMesh.h"
#include "CubeMesh.h"
#include "Model.h"
#include "OrthoMesh.h"
#include "PlaneMesh.h"
#include "PointMesh.h"
#include "QuadMesh.h"
#include "SphereMesh.h"
#include "TerrainMesh.h"
#include "TessellationMesh.h"
#include "TriangleMesh.h"

// Include additional rendering headers
#include "Light.h"
#include "RenderTexture.h"

// smart pointers
// std::unique_ptr its constructor is explicit meaning
// you have to call the construor explicitly
#include <memory>

// imGUI includes
//#include "imgui.h"
//#include "imgui_impl_dx11.h"
