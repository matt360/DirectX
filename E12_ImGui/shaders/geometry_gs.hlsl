// GEOMETRY SHADER

// Example geometry shader
// Receives a point and outputs a triangle

//Texture2D tex0 : register(t0);
//SamplerState Sampler0 : register(s0);

cbuffer MatrixBuffer : register(cb0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer PositionBuffer
{
	/*  
		((UV)) and (Vertex) positions
	    ((0,0))           ((1, 0))
	       0------------------2
	       |(-1, 1)     (1, 1)|
	       |                  |
	       |                  |
	       |       (0.0)      |
	       |                  |
	       |                  |
	       |(-1, -1)   (1, -1)|
	       1------------------3
	    ((0, 1))          ((1, 1))
	*/
    static float3 g_positions[4] =
    {
        float3(-1, 1, 0),   // 0
        float3(-1, -1, 0),  // 1
        float3(1, 1, 0),    // 2
        float3(1, -1, 0)    // 3
    };
};

cbuffer TextureBuffer
{
    static float2 uv_positions[4] = 
	{
        float2(0, 0),  // 0
		float2(0, 1),  // 1
		float2(1, 0),  // 2
		float2(1, 1)   // 3
    };
}

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
[maxvertexcount(8)]
// input[1] means take one point from the primitive

// if the topology is a pointlist and the mesh is a triangle
// then each vertex will become first and only point
// so gs will execute 3 times for each vertex
// creating a quad in a place of a vertex 
// (3 quads instead of 3 points overall)

// if the topology is a trianglelist and the mesh is a triangle
// then the gs will run only once taking the first vertex from
// the triangle mesh and convert it into a quad
// creating a quad in a place of a vertex
// (1 quad instead of 1 triangle)
                                                    //  < Data type > 
                                                    // -Any data type 
                                                    // -In most casesa a struct defining data for the next shader
void main(point InputType input[1], inout TriangleStream<OutputType> triStream)
{
    OutputType output;

    // Change the position vector to be 4 units for proper matrix calculations.
    input[0].position.w = 1.0f;

   /* v1 = input[0].position.xyz - 1

    v2  = input[0].position.xyz - 2

    cross(v1, v2);

    output.normal = cross(v1, v2);*/

    for (int i = 0; i < 4; i++)
    {
        float3 vposition = g_positions[i];
        // place the point in the world
        vposition = mul(vposition, (float3x3)worldMatrix) + input[0].position;
        output.position = mul(float4(vposition, 1.0), viewMatrix);
        output.position = mul(output.position, projectionMatrix);

        float2 uv_position = uv_positions[i];
        output.tex = uv_position;

        output.normal = mul(input[0].normal, (float3x3) worldMatrix);
        output.normal = normalize(output.normal);
        // add the triangle to the rendering list
        triStream.Append(output);
    }
    triStream.RestartStrip();

    for (i = 0; i < 4; i++)
    {
        float3 vposition = g_positions[i];
        // place the point in the world
        vposition = mul(vposition, (float3x3)worldMatrix) + input[0].position + normalize(input[0].normal) * -10.f;
        output.position = mul(float4(vposition, 1.0), viewMatrix);
        output.position = mul(output.position, projectionMatrix);

        float2 uv_position = uv_positions[i];
        output.tex = uv_position;

        output.normal = mul(input[0].normal, (float3x3)worldMatrix);
        output.normal = normalize(output.normal);
        // add the triangle to the rendering list
        triStream.Append(output);
    }
    triStream.RestartStrip();
}

//// gs function
////[maxvertexcount(3)]
//void main(point InputType input[1], inout TriangleStream<OutputType> triStream)
//{
//    OutputType output;
	
	
//	// Change the position vector to be 4 units for proper matrix calculations.
//    input[0].position.w = 1.0f;

//	// Move the vertex away from the point position
//    output.position = input[0].position + float4(0.0, 1.0, 0.0, 0.0);
	
//    output.position = mul(output.position, worldMatrix);
//    output.position = mul(output.position, viewMatrix);
//    output.position = mul(output.position, projectionMatrix);

//    output.tex = input[0].tex;

//    output.normal = mul(input[0].normal, (float3x3) worldMatrix);
//    output.normal = normalize(output.normal);

//    triStream.Append(output);

//	// Move the vertex away from the point position
//    output.position = input[0].position + float4(-1.0, 0.0, 0.0, 0.0);
	
//    output.position = mul(output.position, worldMatrix);
//    output.position = mul(output.position, viewMatrix);
//    output.position = mul(output.position, projectionMatrix);

//    output.tex = input[0].tex;

//    output.normal = mul(input[0].normal, (float3x3) worldMatrix);
//    output.normal = normalize(output.normal);

//    triStream.Append(output);

//	// Move the vertex away from the point position
//    output.position = input[0].position + float4(1.0, 0.0, 0.0, 0.0);
	
//    output.position = mul(output.position, worldMatrix);
//    output.position = mul(output.position, viewMatrix);
//    output.position = mul(output.position, projectionMatrix);

//    output.tex = input[0].tex;

//    output.normal = mul(input[0].normal, (float3x3) worldMatrix);
//    output.normal = normalize(output.normal);

//    triStream.Append(output);

//    triStream.RestartStrip();

//}