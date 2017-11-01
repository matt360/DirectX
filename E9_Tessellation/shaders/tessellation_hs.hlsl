// Tessellation Hull Shader
// Prepares control points for tessellation
// http://richardssoftware.net/Home/Post/28

cbuffer TessellationBuffer : register(cb0)
{
	float tessellationAmount;
	float3 padding;
};

struct InputType
{
    float3 position : POSITION;
    float4 colour : COLOR;
};

// triangle
struct ConstantOutputType
{
    float edges[3] : SV_TessFactor;
    float inside : SV_InsideTessFactor;
};

// quad
//struct ConstantOutputType
//{
//	float edges[4] : SV_TessFactor;
//	float inside[2] : SV_InsideTessFactor;
//};

struct OutputType
{
    float3 position : POSITION;
    float4 colour : COLOR;
};

// CONSTANT HULL SHADER
// triangle
ConstantOutputType PatchConstantFunction(InputPatch<InputType, 3> inputPatch, uint patchId : SV_PrimitiveID)
{    
    ConstantOutputType output;

    // Set the tessellation factors for the three edges of the triangle.
	output.edges[0] = tessellationAmount;
	output.edges[1] = tessellationAmount;
	output.edges[2] = tessellationAmount;

    // Set the tessellation factor for tessallating inside the triangle.
	output.inside = tessellationAmount;

    return output;
}

// CONTROL POINT HULL SHADER
[domain("tri")]
[partitioning("fractional_even")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("PatchConstantFunction")]
OutputType main(InputPatch<InputType, 3> patch, uint pointId : SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
{
    OutputType output;

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
//    // Set the tessellation factors for the three edges of the triangle.
//	output.edges[0] = tessellationAmount;
//	output.edges[1] = tessellationAmount;
//	output.edges[2] = tessellationAmount;
//	output.edges[3] = tessellationAmount;
//
//    // Set the tessellation factor for tessallating inside the triangle.
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