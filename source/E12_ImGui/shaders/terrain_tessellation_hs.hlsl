// HULL SHADER

cbuffer CameraBuffer : register(cb0)
{
    float3 cameraPosition;
    float distance;
};

// Input control point
struct VertexOut
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

// Output control point
struct HullOut
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

// Output patch constant data.
// Triangle
struct PatchTess // HS_CONSTANT_DATA_OUTPUT
{
    float edges[3] : SV_TessFactor;     //float EdgeTessFactor[3] : SV_TessFactor; // e.g. would be [4] for a quad domain
    float inside : SV_InsideTessFactor; //float InsideTessFactor : SV_InsideTessFactor; // e.g. would be Inside[2] for a quad domain
};

#define NUM_CONTROL_POINTS 3

// Patch Constant Function
PatchTess ConstantHS(
	InputPatch<VertexOut, NUM_CONTROL_POINTS> inputPatch,
	uint patchId : SV_PrimitiveID)
{
	PatchTess output;
    float tessellationAmount;

    float3 distance = inputPatch[patchId].position.xyz - cameraPosition.xyz;

    if (length(distance) <= 6.0f)
        tessellationAmount = 4.0f;
    else
    tessellationAmount = 64.0f / length(distance);
    // default amount

    // Tessellating a triangle patch also consists of two parts:
	// 1. Three edge tessellation factors control how much to tessellate along each edge.
    // Set the tessellation factors for the three edges of the triangle.
	// Uniformly tessellate the patch 'tessellationAmount' times.
    output.edges[0] = tessellationAmount ;
    output.edges[1] = tessellationAmount ;
    output.edges[2] = tessellationAmount;

    // 2. One interior tessellation factor indicates how much to tessellate the triangle patch.
    // Set the tessellation factor for tessallating inside the triangle.
	// Uniformly tessellate the patch 'tessellationAmount' times.
    output.inside = tessellationAmount;

    return output;
}

[domain("tri")]
[partitioning("fractional_even")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("ConstantHS")]
HullOut main(
	InputPatch<VertexOut, NUM_CONTROL_POINTS> patch,
	uint pointId : SV_OutputControlPointID,
	uint patchId : SV_PrimitiveID)
{
    HullOut output;
	
    // Set the position for this control point as the output position.
    output.position = patch[pointId].position;
    output.tex = patch[pointId].tex;
    output.normal = patch[pointId].normal;

    return output;
}
