// Light pixel shader
// Calculate diffuse lighting for a single directional light (also texturing)
Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

// Defines - HLSL allows the use of defines. 
static const int NUM_LIGHTS = 4;

cbuffer LightColorBuffer : register(cb0)
{
    float4 ambientColor;
    float4 diffuseColor[4];
    float3 lightDirection;
    float specularPower;
    float4 specularColor;
    float3 lightPosition[4];
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
    float lightIntensity1, lightIntensity2, lightIntensity3, lightIntensity4;
    float4 color, color1, color2, color3, color4;
	

	// Calculate the different amounts of light on this pixel based on the positions of the lights.
    lightIntensity1 = saturate(dot(input.normal, input.lightPos1));
    lightIntensity2 = saturate(dot(input.normal, input.lightPos2));
    lightIntensity3 = saturate(dot(input.normal, input.lightPos3));
    lightIntensity4 = saturate(dot(input.normal, input.lightPos4));
	
	// Determine the diffuse color amount of each of the four lights.
    color1 = diffuseColor[0] * lightIntensity1;
    color2 = diffuseColor[1] * lightIntensity2;
    color3 = diffuseColor[2] * lightIntensity3;
    color4 = diffuseColor[3] * lightIntensity4;

	// Sample the texture pixel at this location.
    textureColor = shaderTexture.Sample(SampleType, input.tex);

	// Multiply the texture pixel by the combination of all four light colors to get the final result.
    color = saturate(color1 + color2 + color3 + color4) * textureColor;
	
    return color;
}

//float4 main(InputType input) : SV_TARGET
//{
//    float4 textureColor;

//    float attenuation1, attenuation2, attenuation3, attenuation4;
//	float3 lightDir1, lightDir2, lightDir3, lightDir4;
//    float lightIntensity1, lightIntensity2, lightIntensity3, lightIntensity4;
//    float4 color, color1, color2, color3, color4;
	
//	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
//    textureColor = shaderTexture.Sample(SampleType, input.tex);

//	// Set the default output color to the ambient light value for all pixels.
//    color = ambientColor;

//	// Calculate the different amounts of light on this pixel based on the positions of the lights.
//    lightIntensity1 = saturate(dot(input.normal, input.lightPos1));
//    lightIntensity2 = saturate(dot(input.normal, input.lightPos2));
//    lightIntensity3 = saturate(dot(input.normal, input.lightPos3));
//    lightIntensity4 = saturate(dot(input.normal, input.lightPos4));
	
//	// Determine the diffuse color amount of each of the four lights.
//    color1 = diffuseColor[0] * lightIntensity1;
//    color2 = diffuseColor[1] * lightIntensity2;
//    color3 = diffuseColor[2] * lightIntensity3;
//    color4 = diffuseColor[3] * lightIntensity4;
	
//	// Set the default output color to the ambient light value for all pixels.
//    color = ambientColor;
	
//    lightDir = -(lightPosition - input.position3D);

//    float distance = length(lightDir);

//    //lightDir /= distance;
	
//    lightDir = normalize(lightDir);

//	// Calculate the amount of light on this pixel.
//    lightIntensity = saturate(dot(input.normal, -lightDir));
	
//    if (lightIntensity > 0.0f)
//    {
//		// attenuation
//        attenuation = 1.0f / (1.0f + 0.125f * distance + 0.0f * pow(distance, 2));

//		// Determine the final diffuse color based on the diffuse color and the amount of light intensity and attenuation.
//        color += (diffuseColor * lightIntensity * attenuation);
	
//		// Saturate the ambient and diffuse color.
//        color = saturate(color);
//    }

//	// Multiply the texture pixel by the combination of all four light colors to get the final result.
//    color = saturate(color1 + color2 + color3 + color4) * textureColor;
	
//    return color;
//}