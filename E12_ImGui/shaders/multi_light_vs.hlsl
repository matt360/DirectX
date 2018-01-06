// VERTEX SHADER

// Defines
#define NUM_LIGHTS 8

// Globals
cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer LightPositionBuffer
{
    float3 lightPosition[NUM_LIGHTS];
};

// Typedefs
struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct PixelInputType
{
    float4 position : SV_POSITION; // put position into SV_POSITION
    float2 tex : TEXCOORD0;        // put tex into TEXCOORD0 which is a place in the memory (TODO check this)
    float3 normal : NORMAL;        // put norml into NORMAL
    float3 lightPositions[8] : TEXCOORD1;
};

// Vertex Shader
PixelInputType main(VertexInputType input)
{
    PixelInputType output;
    float4 worldPosition;

	// Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
	// Store the texture coordinates for the pixel shader.
    output.tex = input.tex;
    
	// Calculate the normal vector against the world matrix only.
    output.normal = mul(input.normal, (float3x3) worldMatrix);
	
	// Normalize the normal vector.
    output.normal = normalize(output.normal);
	
    // Calculate the position of the vertex in the world.
    worldPosition = mul(input.position, worldMatrix);

    // Determine the light positions based on the position of the lights and the position of the vertex in the world.
    for (int i = 0; i < NUM_LIGHTS; ++i)
    {
        output.lightPositions[i].xyz = lightPosition[i].xyz - worldPosition.xyz;
    }
    // Normalize the light position vectors.
    for (i = 0; i < NUM_LIGHTS; ++i)
    {
        output.lightPositions[i].xyz = normalize(output.lightPositions[i]);
    }

    return output;
 }