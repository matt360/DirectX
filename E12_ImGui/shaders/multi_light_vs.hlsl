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
    float3 lightPos1 : TEXCOORD1;  // put lightPos1 into TEXCOORD1 
    float3 lightPos2 : TEXCOORD2;  // put lightPos2 into TEXCOORD2 
	float3 lightPos3 : TEXCOORD3;  // put lightPos3 into TEXCOORD3 
	float3 lightPos4 : TEXCOORD4;  // put lightPos4 into TEXCOORD4
	float3 lightPos5 : TEXCOORD5;  // put lightPos5 into TEXCOORD5 
	float3 lightPos6 : TEXCOORD6;  // put lightPos6 into TEXCOORD6 
	float3 lightPos7 : TEXCOORD7;  // put lightPos7 into TEXCOORD7 
	float3 lightPos8 : TEXCOORD8;  // put lightPos8 into TEXCOORD8
    float3 lightPositions[8] : TEXCOORD9;
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
    //output.lightPos1.xyz = lightPosition[0].xyz - worldPosition.xyz;
    //output.lightPos2.xyz = lightPosition[1].xyz - worldPosition.xyz;
    //output.lightPos3.xyz = lightPosition[2].xyz - worldPosition.xyz;
    //output.lightPos4.xyz = lightPosition[3].xyz - worldPosition.xyz;
    //output.lightPos5.xyz = lightPosition[4].xyz - worldPosition.xyz;
    //output.lightPos6.xyz = lightPosition[5].xyz - worldPosition.xyz;
    //output.lightPos7.xyz = lightPosition[6].xyz - worldPosition.xyz;
    //output.lightPos8.xyz = lightPosition[7].xyz - worldPosition.xyz;

    // Normalize the light position vectors.
    for (i = 0; i < NUM_LIGHTS; ++i)
    {
        output.lightPositions[i].xyz = normalize(output.lightPositions[i]);
    }

    //output.lightPos1 = normalize(output.lightPos1);
    //output.lightPos2 = normalize(output.lightPos2);
    //output.lightPos3 = normalize(output.lightPos3);
    //output.lightPos4 = normalize(output.lightPos4);
    //output.lightPos5 = normalize(output.lightPos5);
    //output.lightPos6 = normalize(output.lightPos6);
    //output.lightPos7 = normalize(output.lightPos7);
    //output.lightPos8 = normalize(output.lightPos8);

    return output;
 }