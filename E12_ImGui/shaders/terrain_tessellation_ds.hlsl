// DOMAIN SHADER
// TEXTURES
Texture2D tex0 : register(t0);

// SAMPLE STATES
SamplerState Sampler0 : register(s0);

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
struct HullOut
{
    //float3 vPosition : WORLDPOS;
    float4 position : POSITION; // SV - system value
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 position3D : TEXCOORD1;
};

struct DomainOut
{
	//float4 vPosition  : SV_POSITION;
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 position3D : TEXCOORD1;
};

// Output patch constant data.
struct PatchTess // HS_CONSTANT_DATA_OUTPUT
{
	//float EdgeTessFactor[3]			: SV_TessFactor; // e.g. would be [4] for a quad domain
	//float InsideTessFactor			: SV_InsideTessFactor; // e.g. would be Inside[2] for a quad domain
    float edges[3] : SV_TessFactor;
    float inside : SV_InsideTessFactor;
};

#define NUM_CONTROL_POINTS 3

[domain("tri")]
DomainOut main(
	PatchTess input, // HS_CONSTANT_DATA_OUTPUT
	float3 uvwCoord : SV_DomainLocation,
	const OutputPatch<HullOut, NUM_CONTROL_POINTS> patch)
{
    DomainOut output;
    float3 vertexPosition;
    float2 texPosition;
    float3 normalPosition;
    float3 vertex3DPosition;

	//output.position = float4(
	//	patch[0].vPosition*domain.x+patch[1].vPosition*domain.y+patch[2].vPosition*domain.z,1);

    // Determine the position of the new vertex.
	// Invert the Y and Z components of uvwCoord as these coords are generated in UV space and therefore Y is positive downward.
	// Alternatively you can set the output topology of the hull shader to cw instead of ccw (or vice versa).
    // position
    vertexPosition = 
    uvwCoord.x * patch[0].position + 
    uvwCoord.y * patch[1].position + 
    uvwCoord.z * patch[2].position;
    // tex 
    texPosition =
    uvwCoord.x * patch[0].tex +
    uvwCoord.y * patch[1].tex +
    uvwCoord.z * patch[2].tex;
    // normal
    normalPosition = 
    uvwCoord.x * patch[0].normal +
    uvwCoord.y * patch[1].normal +
    uvwCoord.z * patch[2].normal;
    // position3D
    vertex3DPosition =
    uvwCoord.x * patch[0].position3D +
    uvwCoord.y * patch[1].position3D +
    uvwCoord.z * patch[2].position3D;
     // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    float4 textureColor = tex0.SampleLevel(Sampler0, texPosition, 0);

    // VERTEX CODE
    for (float i = 1.0f; i >= 0.0f; i -= 0.01f)
    {
        if (textureColor.r > i)
        {
            input.position.y -= i * 0.5f;
            input.normal.y -= abs(0.9 * 15.0f);
        }
    }

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

[domain("quad")]
DomainOut main(
	HS_CONSTANT_DATA_OUTPUT input,
	float2 domain : SV_DomainLocation,
	const OutputPatch<HS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> patch)
{
    DomainOut output;

    output.worldpos = lerp(lerp(patch[0].worldpos, patch[1].worldpos, domain.x), lerp(patch[2].worldpos, patch[3].worldpos, domain.x), domain.y);
	
    float h;
    if (input.skirt < 5)
    {
        if (input.skirt > 0 && domain.y == 1)
        {
            h = heightmap.SampleLevel(hmsampler, output.worldpos / width, 0.0f).x;
            output.worldpos.z = h * scale;
        }
    }
    else
    {
        h = heightmap.SampleLevel(hmsampler, output.worldpos / width, 0.0f).x;
        output.worldpos.z = h * scale;
    }
	
    float3 norm = estimateNormal(output.worldpos / width);
    output.worldpos += norm * 0.5f * (2.0f * displacementmap.SampleLevel(displacementsampler, output.worldpos / 32, 0.0f).w - 1.0f);

	// generate coordinates transformed into view/projection space.
    output.pos = float4(output.worldpos, 1.0f);
    output.pos = mul(output.pos, viewproj);

	[unroll]
    for (int i = 0; i < 4; ++i)
    {
		// generate projective tex-coords to project shadow map onto scene.
        output.shadowpos[i] = float4(output.worldpos, 1.0f);

        output.shadowpos[i] = mul(output.shadowpos[i], shadowtexmatrices[i]);
    }
    return output;
}