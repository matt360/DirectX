// texture vertex shader
// Basic shader for rendering textured geometry

// VERTEX SHADERS ARE THE PART OF THE PIPELINE WHERE YOU ARE GIVEN CONTROL OF EVERY
// VERTEX THAT GETS PROCESSED BY THE SYSTEM.

// Beginning DirectX 11 Game Programming: with the introduction of Direct3D 10,
// all external variables now reside in constant buffers. Constant buffers are
// used to group variables visible to the calling program so that they can be optimized for access.
// Constant buffers are similar in definition to structures and are created using the cbuffer keyword.
// cbuffer Variables 
// {
//     matrix Projection;
// }
// Constant buffers are commonly declared at the top of an effect file and reside outside of any other section.
// For ease of use, it can be useful to group together variables based on the amount they are accessed. 
// For instance, variables that get an initial value would be grouped separately from variables that are updated
// on a frame by frame basis. You have the ability to create multiple constant buffers.
cbuffer MatrixBuffer : register(cb0)
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
    float4 position : SV_POSITION; // SV_POSITION - A float4 system value specifying a transformed position
    float2 tex : TEXCOORD0; 
	float3 normal : NORMAL; // NORMAL - Semantic that is used when defining a normal vector
};

// TODO
//technique11 Render
//{
//	// technique defintion
//};

//ReturnValue FunctionName(parameterName : semantic)
//{
//	// function code goes here
//}

// Using the 'out' keyword before your parameter declaration lets the compiler
// know that the variable will be used as an output. Additionally, the keyword
// 'inout' allows the variable to be used both as an input and output
// void GetColor(out float3 color)
//{
//	color = float3(0.0f, 1.0f, 1.0f);
//}

OutputType main(InputType input)
{
	OutputType output;
    
    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

	// Store the texture coordinates for the pixel shader.
    output.tex = input.tex;

	// Calculate the normal vector against the world matrix only.
    output.normal = mul(input.normal, (float3x3)worldMatrix);
	
    // Normalize the normal vector.
    output.normal = normalize(output.normal);
    
    return output;
}