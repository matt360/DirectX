// VERTEX SHADER 

// Global
cbuffer MatrixBuffer : register(cb0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

// Global
cbuffer TimeBuffer : register(cb1)
{
    float time;
    float height;
    float frequency;
    float padding;
};

struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct PixelInputType
{
    float4 position : SV_POSITION; // SV - system value
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
	float3 position3D : TEXCOORD1;
};

PixelInputType main(VertexInputType input)
{
    //  return output;
    PixelInputType output;
    float heightWave = height;
	
	// Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    //  offset position based on sine wave
    input.position.x += (heightWave * sin((input.position.y + time) * frequency));
    input.position.y += (heightWave * sin((input.position.x + time) * frequency));
    input.position.z += (heightWave * sin((input.position.y + time) * frequency));
                        
    input.position.x += (heightWave * sin((input.normal.y + time) * frequency));
    input.position.y += (heightWave * sin((input.normal.x + time) * frequency));
    input.position.z += (heightWave * sin((input.normal.y + time) * frequency));

    input.normal.x = 1 - cos(input.position.x + time);
    input.normal.y = abs(cos(input.position.y + time));
    input.normal.z = abs(cos(input.position.y + time));

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