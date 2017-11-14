// Tessellation Hull Shader
// Prepares control points for tessellation
// http://richardssoftware.net/Home/Post/28

cbuffer CameraBuffer : register(cb0)
{
	float3 cameraPosition;
	float padding;
};

struct VertexOut
{
    float3 position : POSITION;
    float4 colour : COLOR;
};

// triangle
struct PatchTess
{
    float edges[3] : SV_TessFactor;
    float inside : SV_InsideTessFactor;
	// Additional info you want associated per patch goes here.
};

// quad
//struct ConstantOutputType
//{
//	float edges[4] : SV_TessFactor;
//	float inside[2] : SV_InsideTessFactor;
//  Additional info you want associated per patch goes here.
//};

struct HullOut
{
    float3 position : POSITION;
    float4 colour : COLOR;
};
 
/*
CONSTANT HULL SHADER
This constant hull shader is evaluated per patch, and is tasked with outputting the so-called tessellation factors of the mesh. The
tessellation factors instruct the tessellation stage how much to tessellate the patch. Here is an example of a triangle patch with 3 control
points, where we tessellate it uniformly 'tessellationAmount' times.
*/
// triangle
PatchTess ConstantHS(InputPatch<VertexOut, 3> inputPatch, uint patchId : SV_PrimitiveID)
{    
    PatchTess pt;
    float tessellationAmount;

    float3 distance = inputPatch[patchId].position - cameraPosition;
    
    if (length(distance) > 8.0f)
        tessellationAmount = 4.0f;
	else
        tessellationAmount = 64.0f / length(distance);

	// no need to clamp - Tessellator clamps it for us (TODO check this info)
    // clamp(tessellationAmount, 1.0f, 64.0f);

	// Tessellating a triangle patch also consists of two parts:
	// 1. Three edge tessellation factors control how much to tessellate along each edge.
    // Set the tessellation factors for the three edges of the triangle.
	// Uniformly tessellate the patch 'tessellationAmount' times.
	pt.edges[0] = tessellationAmount;
	pt.edges[1] = tessellationAmount;
    pt.edges[2] = tessellationAmount;

	// 2. One interior tessellation factor indicates how much to tessellate the triangle patch.
    // Set the tessellation factor for tessallating inside the triangle.
	// Uniformly tessellate the patch 'tessellationAmount' times.
    pt.inside = tessellationAmount;

    return pt;
}

// CONTROL POINT HULL SHADER
/*
The system also provides a patch ID value via the SV_PrimitiveID semantic that can be used if needed; 
the ID uniquely identifies the patches in a draw call.
*/
[domain("tri")]
[partitioning("fractional_even")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("ConstantHS")]
HullOut main(InputPatch<VertexOut, 3> patch, uint pointId : SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
{
    HullOut output;
	
    // Set the position for this control point as the output position.
    output.position = patch[pointId].position;

    // Set the input color as the output color.
    output.colour = patch[pointId].colour;

    return output;
}

// quad
//ConstantOutputType PatchConstantFunction(InputPatch<InputType, 4> inputPatch, uint patchId : SV_PrimitiveID)
//{    
//    ConstantOutputType output;
//
//  // Tessellating a quad patch consists of two parts:
//  // 1. Four edge tessellation factors control how much to tessellate along each edge.
//  // Set the tessellation factors for the three edges of the triangle.

//  // Uniformly tessellate the patch 'tessellationAmount' times.
//	output.edges[0] = tessellationAmount;
//	output.edges[1] = tessellationAmount;
//	output.edges[2] = tessellationAmount;
//	output.edges[3] = tessellationAmount;
//
//  // 2. Two interior tessellation factors indicate how to tessellate the quad patch (one tessellation factor for the horizontal dimension of
//  // the quad, and one tessellation factor for the vertical dimension of the quad).
//  // Set the tessellation factor for tessallating inside the triangle.

//  // Uniformly tessellate the patch 'tessellationAmount' times.
//	output.inside[0] = tessellationAmount;
//	output.inside[1] = tessellationAmount;
//
//    return output;
//}
//
//
//[domain("quad")]
//[partitioning("integer")]
//[outputtopology("triangle_cw")]
//[outputcontrolpoints(4)]
//[patchconstantfunc("PatchConstantFunction")]
//OutputType main(InputPatch<InputType, 4> patch, uint pointId : SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
//{
//    OutputType output;
//
//    // Set the position for this control point as the output position.
//    output.position = patch[pointId].position;
//
//    // Set the input color as the output color.
//    output.colour = patch[pointId].colour;
//
//    return output;
//}