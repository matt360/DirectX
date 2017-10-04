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

cbuffer MatrixBuffer : register(cb0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

// Similar to C we can create our own type definitions. We will use different types such as float4 that are available to HLSL which make programming shaders easier and readable. 
// The POSITION, TEXCOORD0, and NORMAL are semantics that convey to the GPU the use of the variable.
// I have to create two different structures here since the semantics are different for vertex and pixel shaders even though the structures are the same otherwise.
// POSITION works for vertex shaders and SV_POSITION works for pixel shaders
// while TEXCOORD0 and NORMAL works for both.
// If you want more than one of the same type then you have to add a number to the end such as TEXCOORD0, TEXCOORD1, and so forth.

// The vertex shader is called by the GPU when it is processing data from the vertex buffers that have been sent to it.
// This vertex shader which is named 'main' will be called for every single vertex in the vertex buffer.
// The input to the vertex shader must match the data format in the vertex buffer as well as the type definition in the shader source file which in this case is VertexInputType. ->
struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

// -> The output of the vertex shader will be sent to the pixel shader. In this case the output type is called PixelInputType which is defined below as well. -->
struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

PixelInputType main(VertexInputType input)
{
    // --> With that in mind you see that the vertex shader creates an output variable that is of the PixelInputType type. --->
    PixelInputType output;
	
	input.position *= 5.0f;

    // (Also note that I do set the W value of the input position to 1.0 otherwise it is undefined since we only read in a XYZ vector for position.)
	// Change the position vector to be 4 units for proper matrix calculations.
	input.position.w = 1.0f;


    // ---> It then takes the position of the input vertex and multiplies it by the world, view, and then projection matrices.
    // This will place the vertex in the correct location for rendering in 3D space according to our view and then onto the 2D screen. ---->
	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

    // ----> After that the output variable takes a copy of the input color and then returns the output which will be used as input to the pixel shader. 
	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;

	// Store normals for the pixel shader
	output.normal = mul(input.normal, (float3x3)worldMatrix);
	output.normal = normalize(output.normal);

	return output;
}