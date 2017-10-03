// Light vertex shader
// Standard issue vertex shader, apply matrices, pass info to pixel shader

cbuffer MatrixBuffer : register(cb0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer CameraBuffer : register(cb1)
{
	float3 cameraPosition;
	float paddingCamera;
};

cbuffer TimeBuffer : register(cb2)
{
    float time;
    float3 paddingTime;
};

struct InputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct OutputType
{
    float4 position : SV_POSITION; // SV - system value
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
	float3 position3D : TEXCOORD1;
};

OutputType main(InputType input)
{
    OutputType output;
    float height = 1.0f;
	
	// Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    // offset position based on sine wave
    input.position.y = height * sin(input.position.x + time);

    input.normal.x = 1 - cos(input.position.x + time);
    input.normal.y = abs(cos(input.position.y + time));

	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

	// Store the texture coordinates for the pixel shader.
    output.tex = input.tex;

	// Store normals for the pixel shader
    output.normal = mul(input.normal, (float3x3) worldMatrix);
    output.normal = normalize(output.normal);

    return output;
}