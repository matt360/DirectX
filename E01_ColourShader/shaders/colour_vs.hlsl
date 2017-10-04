// colour vertex shader
// Simple geometry pass
// texture coordinates and normals will be ignored.

// These will be our first shader programs.
// Shaders are small programs that do the actual rendering of models.
// These shaders are written in HLSL and stored in source files called color.vs and color.ps

/////////////
// GLOBALS //
/////////////
// In shader programs you begin with the global variables. 
// These globals can be modified externally from your C++ code.
// You can use many types of variables such as int or float and then set them externally for the shader program to use.
// Generally you will put most globals in buffer object types called "cbuffer" even if it is just a single global variable.
// Logically organizing these buffers is important for efficient execution of shaders as well as how the graphics card will store the buffers.
// In this example I've put three matrices in the same buffer since I will update them each frame at the same time.

cbuffer MatrixBuffer 
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

struct InputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct OutputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

OutputType main(InputType input)
{
	OutputType output;
	
	input.position *= 5.0f;

	// Change the position vector to be 4 units for proper matrix calculations.
	input.position.w = 1.0f;


	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;

	// Store normals for the pixel shader
	output.normal = mul(input.normal, (float3x3)worldMatrix);
	output.normal = normalize(output.normal);

	return output;
}