// PIXEL SHADER 

Texture2D shaderTexture : register(t0);

SamplerState SampleType : register(s0);

cbuffer LightBuffer : register(cb0)
{
    float4 ambientColor;
    float4 diffuseColor;
    float3 lightDirection;
    float specularPower;
    float4 specularColor;
    float3 lightPosition;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
	float3 position3D : TEXCOORD1;
};

float4 main(PixelInputType input) : SV_TARGET
{
	float4 textureColor;
	float3 lightDir;
	float lightIntensity;
	float4 color;
    float attenuation;
    float slope;
	
	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
	textureColor = shaderTexture.Sample(SampleType, input.tex);
	
	// Set the default output color to the ambient light value for all pixels.
	color = ambientColor;

    // Calculate the slope of this point
    //slope = 1.0f - input.normal.y;
    slope = textureColor;
	
    // determine which texutre to use based on texture's pixel color
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

	return color;
}