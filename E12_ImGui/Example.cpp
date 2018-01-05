#include "Example.h"

Example::Example()
{
}

Example::~Example()
{
}

// not passing through the reference because we're not modifying the passed value
void Example::set_mesh_choice(const MESH_CHOICE msh_ch)
{
	switch (msh_ch)
	{
	case MESH_CHOICE::TRIANGLE:
		mesh_choice = MESH_CHOICE::TRIANGLE;
		triangle_mesh = true;
		sphere_mesh = false;
		cube_mesh = false;
		quad_mesh = false;
		plane_mesh = false;
		break;

	case MESH_CHOICE::SPHERE:
		mesh_choice = MESH_CHOICE::SPHERE;
		triangle_mesh = false;
		sphere_mesh = true;
		cube_mesh = false;
		quad_mesh = false;
		plane_mesh = false;
		break;

	case MESH_CHOICE::CUBE:
		mesh_choice = MESH_CHOICE::CUBE;
		triangle_mesh = false;
		sphere_mesh = false;
		cube_mesh = true;
		quad_mesh = false;
		plane_mesh = false;
		break;

	case MESH_CHOICE::QUAD:
		mesh_choice = MESH_CHOICE::QUAD;
		triangle_mesh = false;
		sphere_mesh = false;
		cube_mesh = false;
		quad_mesh = true;
		plane_mesh = false;
		break;

	case MESH_CHOICE::PLANE:
		mesh_choice = MESH_CHOICE::PLANE;
		triangle_mesh = false;
		sphere_mesh = false;
		cube_mesh = false;
		quad_mesh = false;
		plane_mesh = true;
		break;

	// triangle mesh_choice by default
	/*default:
		mesh_choice = MESH_CHOICE::TRIANGLE;
		triangle_mesh = true;
		sphere_mesh = false;
		cube_mesh = false;
		quad_mesh = false;
		plane_mesh = false;
		break;*/
	}
}
