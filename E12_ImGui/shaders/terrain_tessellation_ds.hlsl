// DOMAIN SHADER

// Globals
cbuffer MatrixBuffer : register(cb0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

// Output control point
struct HullOut
{
    //float3 vPosition : WORLDPOS;
    float4 position : POSITION; // SV - system value
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct DomainOut
{
	//float4 vPosition  : SV_POSITION;
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

// Output patch constant data.
struct PatchTess // HS_CONSTANT_DATA_OUTPUT
{
	//float EdgeTessFactor[3]			: SV_TessFactor; // e.g. would be [4] for a quad domain
	//float InsideTessFactor			: SV_InsideTessFactor; // e.g. would be Inside[2] for a quad domain
    float edges[3] : SV_TessFactor;
    float inside : SV_InsideTessFactor;
};

#define NUM_CONTROL_POINTS 4

[domain("tri")]
DomainOut main(
	PatchTess input,
	float3 uvwCoord : SV_DomainLocation,
	const OutputPatch<HullOut, NUM_CONTROL_POINTS> patch)
{
    DomainOut output;
    float3 vertexPosition;

	//output.position = float4(
	//	patch[0].vPosition*domain.x+patch[1].vPosition*domain.y+patch[2].vPosition*domain.z,1);

    // Determine the position of the new vertex.
	// Invert the Y and Z components of uvwCoord as these coords are generated in UV space and therefore Y is positive downward.
	// Alternatively you can set the output topology of the hull shader to cw instead of ccw (or vice versa).
    vertexPosition = uvwCoord.x * patch[0].position + uvwCoord.y * patch[1].position + uvwCoord.z * patch[2].position;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(float4(vertexPosition, 1.0f), worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    // Store normals for the pixel shader
    output.normal = mul(float4(vertexPosition, 1.0f), worldMatrix);
    output.normal = normalize(output.normal); // (float3x3) 

    output.tex = patch[0].tex;

	return output;
}
