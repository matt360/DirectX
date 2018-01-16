// GEOMETRY SHADER

// Output control point
struct InputType
{
    float4 position : SV_POSITION;
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
	point InputType input[1],
	inout TriangleStream<OutputType> output
)
{
    //input[0].position.w = 1.0f;

   /* v1 = input[0].position.xyz - 1

    v2  = input[0].position.xyz - 2

    cross(v1, v2);

    output.normal = cross(v1, v2);*/

	//for (uint i = 0; i < 3; i++)
	//{
  //      OutputType element;
		//element.position = input[i];
		//output.Append(element);
	//}
}