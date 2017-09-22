
// Light pixel shader
// Calculate diffuse lighting for a single directional light (also texturing)

Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

cbuffer LightBuffer : register(cb0)
{
	float4 ambientColor;
    float4 diffuseColor;
    float3 direction;
	float specularPower;
	float4 specularColor;
};

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
	float3 viewDirection : TEXCOORD1;
};

float4 main(InputType input) : SV_TARGET
{
    float4 textureColor;
	// Light that travels in a single direction
    float3 lightDir;
    float lightIntensity;
    float4 color;
	float spec;
	float4 finalSpec;
	float3 halfway;

	//float3 lightDir = light.position - pos3D; //3D position in space of the surface
	//float distance = length(lightDir);
	//lightDir = lightDir / distance; // = normalize( lightDir );
	//distance = distance * distance; //This line may be optimised using Inverse square root

    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    textureColor = shaderTexture.Sample(SampleType, input.tex);

	// Set a default amount of color
	color = ambientColor;
	
	// Invert the light direction for calculations.
    lightDir = -direction;

    // Calculate the amount of light on this pixel. (Light intensity based on angle of face to direction of light) / Diffuse Power
    lightIntensity = saturate(dot(input.normal, lightDir));

	// Determine the final amount of diffuse color based on the diffuse color combined with the light intensity.
	if (lightIntensity > 0.0f)
	{
		//color += saturate(diffuseColor * lightIntensity);
		color += (diffuseColor * lightIntensity);
		// saturate - clamps the specified value within the range of 0 to 1
		color = saturate(color);

		// blinn pong
		halfway = normalize(lightDir + input.viewDirection);
		// pow - return the specified value raised to the specified power.
		// max - selects the greater of x and y.
		// dot - returns the dot product of two vectors.
		spec = pow(max(dot(input.normal, halfway), 0.0), specularPower);
		finalSpec = saturate(specularColor * spec);
	}

	color = color * textureColor;

	color = saturate(color + finalSpec);

	return color;
    // Multiply the texture pixel and the final diffuse color to get the final pixel color result.
   // return clamp(color * textureColor, -1.0f, 1.0f);
}

