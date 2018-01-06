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
    float3 lightPos1 : TEXCOORD1;
    float3 lightPos2 : TEXCOORD2;
    float3 lightPos3 : TEXCOORD3;
    float3 lightPos4 : TEXCOORD4;
    float3 lightPos5 : TEXCOORD5; 
    float3 lightPos6 : TEXCOORD6; 
    float3 lightPos7 : TEXCOORD7; 
    float3 lightPos8 : TEXCOORD8;
    float3 lightPositions[8] : TEXCOORD9;
};

// Pixel Shader
float4 main(PixelInputType input) : SV_TARGET
{
    float4 textureColor;
    float lightIntensity0, 
    lightIntensity1, 
    lightIntensity2, 
    lightIntensity3, 
    lightIntensity4, 
    lightIntensity5, 
    lightIntensity6,
    lightIntensity7,
    lightIntensity[8];
    float4 color,
    color0, 
    color1, 
    color2, 
    color3, 
    color4, 
    color5, 
    color6, 
    color7,
    colors[8], 
    add_colours = 0;

	// Calculate the different amounts of light on this pixel based on the positions of the lights.
    for (int i = 0; i < NUM_LIGHTS; ++i)
    {
        lightIntensity[i] = saturate(dot(input.normal, input.lightPositions[i]));
    }

    //lightIntensity0 = saturate(dot(input.normal, input.lightPos1));
    //lightIntensity1 = saturate(dot(input.normal, input.lightPos2));
    //lightIntensity2 = saturate(dot(input.normal, input.lightPos3));
    //lightIntensity3 = saturate(dot(input.normal, input.lightPos4));
    //lightIntensity4 = saturate(dot(input.normal, input.lightPos5));
    //lightIntensity5 = saturate(dot(input.normal, input.lightPos6));
    //lightIntensity6 = saturate(dot(input.normal, input.lightPos7));
    //lightIntensity7 = saturate(dot(input.normal, input.lightPos8));
	
	// Determine the diffuse color amount of each of the four lights.
    for (i = 0; i < NUM_LIGHTS; ++i)
    {
        colors[i] =  diffuseColor[i] * lightIntensity[i];
    }
    //color0 = diffuseColor[0] * lightIntensity0;
    //color1 = diffuseColor[1] * lightIntensity1;
    //color2 = diffuseColor[2] * lightIntensity2;
    //color3 = diffuseColor[3] * lightIntensity3;
    //color4 = diffuseColor[4] * lightIntensity4;
    //color5 = diffuseColor[5] * lightIntensity5;
    //color6 = diffuseColor[6] * lightIntensity6;
    //color7 = diffuseColor[7] * lightIntensity7;

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
