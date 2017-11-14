// Example geometry shader
// Receives a point and outputs a triangle

cbuffer MatrixBuffer : register(cb0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer PositionBuffer
{
    static float3 g_positions[4] =
    {
        float3(-1, 1, 0),
        float3(-1, -1, 0),
        float3(1, 1, 0),
        float3(1, -1, 0)
    };
};

struct InputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;

};

// pixel input type
struct OutputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

// gs function
[maxvertexcount(4)]
void main(point InputType input[1], inout TriangleStream<OutputType> triStream)
{
    OutputType output;

    // Change the position vector to be 4 units for proper matrix calculations.
    input[0].position.w = 1.0f;

    for (int i = 0; i < 4; i++)
    {
        float3 vposition = g_positions[i];
        // place the point in the world
        vposition = mul(vposition, (float3x3) worldMatrix) + input[0].position;
        output.position = mul(float4(vposition, 1.0), viewMatrix);
        output.position = mul(output.position, projectionMatrix);

        output.tex = input[0].tex;
        output.normal = input[0].normal;
        // add the triangle to the rendering list
        triStream.Append(output);
    }

    triStream.RestartStrip();

}

// gs function
//[maxvertexcount(3)]
//void main(point InputType input[1], inout TriangleStream<OutputType> triStream)
//{
//	OutputType output;
	
	
//	// Change the position vector to be 4 units for proper matrix calculations.
//    input[0].position.w = 1.0f;

//	// Move the vertex away from the point position
//	output.position = input[0].position + float4(0.0, 1.0, 0.0, 0.0);
	
//	output.position = mul(output.position, worldMatrix);
//    output.position = mul(output.position, viewMatrix);
//    output.position = mul(output.position, projectionMatrix);

//	output.tex = input[0].tex;

//    output.normal = mul(input[0].normal, (float3x3)worldMatrix);
//    output.normal = normalize(output.normal);

//	triStream.Append(output);

//	// Move the vertex away from the point position
//	output.position = input[0].position + float4(-1.0, 0.0, 0.0, 0.0);
	
//	output.position = mul(output.position, worldMatrix);
//    output.position = mul(output.position, viewMatrix);
//    output.position = mul(output.position, projectionMatrix);

//	output.tex = input[0].tex;

//    output.normal = mul(input[0].normal, (float3x3)worldMatrix);
//    output.normal = normalize(output.normal);

//	triStream.Append(output);

//	// Move the vertex away from the point position
//	output.position = input[0].position + float4(1.0, 0.0, 0.0, 0.0);
	
//	output.position = mul(output.position, worldMatrix);
//    output.position = mul(output.position, viewMatrix);
//    output.position = mul(output.position, projectionMatrix);

//	output.tex = input[0].tex;

//    output.normal = mul(input[0].normal, (float3x3)worldMatrix);
//    output.normal = normalize(output.normal);

//	triStream.Append(output);

//	triStream.RestartStrip();

//}