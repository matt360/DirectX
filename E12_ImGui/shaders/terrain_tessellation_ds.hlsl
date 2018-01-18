// DOMAIN SHADER
// The domain shader is called for every vertex created by the tessellator.
// It is like the vertex shader after tessellation.

// TEXTURES
Texture2D tex0 : register(t0);

// SAMPLE STATES
SamplerState Sampler0 : register(s0);

// TODO delete if not used
cbuffer TimeBuffer : register(cb1)
{
    float time;
    float height;
    float frequency;
    float padding;
};

// Output control point
struct HullOut
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct DomainOut
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 position3D : TEXCOORD1;
};

// Output patch constant data.
struct PatchTess // HS_CONSTANT_DATA_OUTPUT
{
	//float EdgeTessFactor[3]			: SV_TessFactor; // e.g. would be [4] for a quad domain
	//float InsideTessFactor			: SV_InsideTessFactor; // e.g. would be Inside[2] for a quad domain
    float edges[3] : SV_TessFactor;
    float inside : SV_InsideTessFactor;
};

#define NUM_CONTROL_POINTS 3

[domain("tri")]
DomainOut main(
	PatchTess input, // HS_CONSTANT_DATA_OUTPUT
	float3 uvwCoord : SV_DomainLocation,
	const OutputPatch<HullOut, NUM_CONTROL_POINTS> tri)
{
    DomainOut output;
    float3 vertexPosition;
    float2 texPosition;
    float3 normalPosition;
    float3 vertex3DPosition;

    // Determine the position of the new vertex.
	// Invert the Y and Z components of uvwCoord as these coords are generated in UV space and therefore Y is positive downward.
	// Alternatively you can set the output topology of the hull shader to cw instead of ccw (or vice versa).
    // position
    vertexPosition = 
    uvwCoord.x * tri[0].position + 
    uvwCoord.y * tri[1].position + 
    uvwCoord.z * tri[2].position;
    // tex 
    texPosition =
    uvwCoord.x * tri[0].tex +
    uvwCoord.y * tri[1].tex +
    uvwCoord.z * tri[2].tex;
    // normal
    normalPosition = 
    uvwCoord.x * tri[0].normal +
    uvwCoord.y * tri[1].normal +
    uvwCoord.z * tri[2].normal;
 
     // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    float4 textureColor = tex0.SampleLevel(Sampler0, texPosition, 0);

    // Sample height map
    for (float i = 1.0f; i >= 0.0f; i -= 0.01f)
    {
        if (textureColor.r > i)
        {
            vertexPosition.y -= i * 0.5f;
            normalPosition.y -= abs(0.9 * 15.0f); // normal mapping
        }
    }

    // cross product

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = float4(vertexPosition, 1.0f);
    
    // Store normals for the pixel shader
    output.normal = normalPosition;

    output.tex = texPosition;

    output.position3D = vertexPosition;

	return output;
}