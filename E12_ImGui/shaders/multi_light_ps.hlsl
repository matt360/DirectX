// PIXEL SHADER

// Defines
#define NUM_LIGHTS 8

// Globals
Texture2D shaderTexture;
SamplerState SampleType;

cbuffer LightColorBuffer
{
    float4 diffuseColor[NUM_LIGHTS];
};

// Typedefs
struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 lightPositions[8] : TEXCOORD1;
};

// Pixel Shader
float4 main(PixelInputType input) : SV_TARGET
{
    float4 textureColor;
    float lightIntensity[8];
    float4 color,
    colors[8], 
    add_colours = 0;

	// Calculate the different amounts of light on this pixel based on the positions of the lights.
    for (int i = 0; i < NUM_LIGHTS; ++i)
    {
        lightIntensity[i] = saturate(dot(input.normal, input.lightPositions[i]));
    }

	// Determine the diffuse color amount of each of the four lights.
    for (i = 0; i < NUM_LIGHTS; ++i)
    {
        colors[i] =  diffuseColor[i] * lightIntensity[i];
    }

	// Sample the texture pixel at this location.
    textureColor = shaderTexture.Sample(SampleType, input.tex);

	// Multiply the texture pixel by the combination of all four light colors to get the final result.
    for (i = 0; i < NUM_LIGHTS; ++i)
    {
        add_colours += colors[i];
    }
    color = saturate(add_colours) * textureColor;
	
    return color;
}
