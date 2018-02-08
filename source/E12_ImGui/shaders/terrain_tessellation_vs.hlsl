// VERTEX SHADER

struct VertexIn
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct VertexOut
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

VertexOut main(VertexIn input)
{
    VertexOut output;
 
    output.position = input.position;
    output.tex = input.tex;
    output.normal = input.normal;

    return output;
}