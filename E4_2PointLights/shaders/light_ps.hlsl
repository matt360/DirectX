// Light pixel shader
// Calculate diffuse lighting for a single directional light (also texturing)
Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

// Defines - HLSL allows the use of defines. 
#define NUM_LIGHTS 4;

cbuffer LightBuffer : register(cb0)
{
    float4 ambientColor;
    float4 diffuseColor;
    float3 lightDirection;
    float specularPower;
    float4 specularColor;
    float3 lightPosition[NUM_LIGHTS];
};

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
	float3 lightPos1 : TEXCOORD1;
    float3 lightPos2 : TEXCOORD2;
    float3 lightPos3 : TEXCOORD3;
    float3 lightPos4 : TEXCOORD4;
};

float4 main(InputType input) : SV_TARGET
{
	float4 textureColor;
	float3 lightDir;
	float lightIntensity;
	float4 color;
    float attenuation;
	
	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
	textureColor = shaderTexture.Sample(SampleType, input.tex);
	
	// Set the default output color to the ambient light value for all pixels.
	color = ambientColor;
	
    lightDir = -(lightPosition - input.position3D);

    float distance = length(lightDir);

    //lightDir /= distance;
	
    lightDir = normalize(lightDir);

	// Calculate the amount of light on this pixel.
	lightIntensity = saturate(dot(input.normal, -lightDir));
	
	if (lightIntensity > 0.0f)
	{
		// attenuation
        attenuation = 1.0f / (1.0f + 0.125f * distance + 0.0f * pow(distance, 2));

		// Determine the final diffuse color based on the diffuse color and the amount of light intensity and attenuation.
		color += (diffuseColor * lightIntensity * attenuation);
	
		// Saturate the ambient and diffuse color.
		color = saturate(color);
	}
	
	// Multiply the texture pixel and the input color to get the textured result.
	color = color * textureColor;
	
	//// Add the specular component last to the output color.
	//color = saturate(color + specular);
	
	return color;
}