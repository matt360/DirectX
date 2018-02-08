// VERTEX SHADER

// TEXTURES
Texture2D tex0 : register(t0);

// SAMPLE STATES
SamplerState Sampler0 : register(s0);

// GLOBALS
cbuffer MatrixBuffer : register(cb0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

// TYPEDEFS
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
    // return output;
    OutputType output;
 
	// Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    float4 textureColor = tex0.SampleLevel(Sampler0, input.tex, 0);

    for (float i = 1.0f; i >= 0.0f; i -= 0.01f)
    {
        if (textureColor.r > i)
        {
            input.position.y -= i * 0.5f;
            input.normal.y -= abs(0.9 * 15.0f);
        }
    }
       
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