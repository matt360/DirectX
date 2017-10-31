// Tessellation domain shader
// After tessellation the domain shader processes the all the vertices

cbuffer MatrixBuffer : register(cb0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
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
//    float edges[4] : SV_TessFactor;
//    float inside[2] : SV_InsideTessFactor;
//};

struct InputType
{
    float3 position : POSITION;
    float4 colour : COLOR;
};

struct OutputType
{
    float4 position : SV_POSITION;
    float4 colour : COLOR;
};

// triangle
[domain("tri")]
OutputType main(ConstantOutputType input, float3 uvwCoord : SV_DomainLocation, const OutputPatch<InputType, 4> patch)
{
    float3 vertexPosition;
    OutputType output;
 
    // Determine the position of the new vertex.
	// Invert the y and Z components of uvwCoord as these coords are generated in UV space and therefore y is positive downward.
	// Alternatively you can set the output topology of the hull shader to cw instead of ccw (or vice versa).
	vertexPosition = uvwCoord.x * patch[0].position + uvwCoord.y * patch[1].position + uvwCoord.z * patch[2].position;
    
    // Calculate the position of the new vertex against the world, view, and projection matrices.
    output.position = mul(float4(vertexPosition, 1.0f), worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    // Send the input color into the pixel shader.
    output.colour = patch[0].colour;

    return output;
}

// quad
//[domain("quad")]
//OutputType main(ConstantOutputType input, float2 uvwCoord : SV_DomainLocation, const OutputPatch<InputType, 4> patch)
//{
//	float3 vertexPosition;
//	OutputType output;
//
//	float3 v1 = lerp(patch[0].position, patch[1].position, 1 - uvwCoord.y);
//	float3 v2 = lerp(patch[2].position, patch[3].position, 1 - uvwCoord.y);
//	vertexPosition = lerp(v1, v2, uvwCoord.x);
//
//	// Calculate the position of the new vertex against the world, view, and projection matrices.
//	output.position = mul(float4(vertexPosition, 1.0f), worldMatrix);
//	output.position = mul(output.position, viewMatrix);
//	output.position = mul(output.position, projectionMatrix);
//	// Send the input color into the pixel shader.
//	output.colour = patch[0].colour;
//	return output;
//}
