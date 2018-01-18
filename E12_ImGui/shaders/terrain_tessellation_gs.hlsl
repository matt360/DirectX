// GEOMETRY SHADER

// Globals
cbuffer MatrixBuffer : register(cb0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer TimeBuffer : register(cb1)
{
    float time;
    float height;
    float frequency;
    float padding;
};
// Output control point
struct InputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 position3D : TEXCOORD1;
};

struct OutputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 position3D : TEXCOORD1;
};

[maxvertexcount(3)]
void main(
	triangle InputType input[3],
	inout TriangleStream<OutputType> triStream
)
{
    OutputType output;

	for (uint i = 0; i < 3; i++)
	{
        float2 uv_position = input[i].tex;
        output.tex = uv_position;

        output.normal = mul(input[i].normal, (float3x3) worldMatrix);
        output.normal = normalize(output.normal);

        float3 v1 = input[0].position.xyz - input[1].position.xyz;
        float3 v2 = input[0].position.xyz - input[2].position.xyz;
      
        output.normal = normalize(cross(v1.xyz, v2.xyz));

		// normal per face
        output.position = input[i].position + float4(time * output.normal, 0.0f);

		// place the point in the world
        float4 vposition = mul(output.position, worldMatrix);
        output.position = mul(vposition, viewMatrix);
        output.position = mul(output.position, projectionMatrix);

        output.position3D = input[i].position3D;

        // add the triangle to the rendering list
        triStream.Append(output);
    }
    triStream.RestartStrip();
}

 