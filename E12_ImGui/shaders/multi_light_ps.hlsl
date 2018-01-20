// PIXEL SHADER

// Defines
#define NUM_LIGHTS 16

// Globals
Texture2D shaderTexture;
SamplerState SampleType;

cbuffer LightColorBuffer
{
    float4 diffuseColour[NUM_LIGHTS];
};

// Typedefs
struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 lightPositions[NUM_LIGHTS] : TEXCOORD1;
};

float4 main(InputType input) : SV_TARGET
{
    float4 textureColour;
    float lightIntensity[NUM_LIGHTS];
    float4 colour,
    colors[NUM_LIGHTS],
    add_colours = 0;

	// Calculate the different amounts of light on this pixel based on the positions of the lights.
    for (int i = 0; i < NUM_LIGHTS; ++i)
    {
        lightIntensity[i] = saturate(dot(input.normal, input.lightPositions[i]));
    }

	// Determine the diffuse color amount of each of the light.
    for (i = 0; i < NUM_LIGHTS; ++i)
    {
        colors[i] =  diffuseColour[i] * lightIntensity[i];
    }

	// Sample the texture pixel at this location.
    textureColour = shaderTexture.Sample(SampleType, input.tex);

	// Multiply the texture pixel by the combination of all light colors to get the final result.
    for (i = 0; i < NUM_LIGHTS; ++i)
    {
        add_colours += colors[i];
    }
    colour = saturate(add_colours) * textureColour;
	
    return colour;
}
