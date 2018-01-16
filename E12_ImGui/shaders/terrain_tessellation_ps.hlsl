// PIXEL SHADER

// TODO delete
// Pixel shaders are small programs that are written for doing the coloring of the polygons that we draw. 
// They are run by the GPU for every visible pixel that will be drawn to the screen.
// Coloring, texturing, lighting, and most other effects you plan to do to your polygon faces are handled by the pixel shader program.
// Pixel shaders must be efficiently written due to the number of times they will be called by the GPU.

// Textures
Texture2D shaderTexture : register(t0);

// Sample states
SamplerState SampleType : register(s0);

// Constant buffers
cbuffer LightBuffer : register(cb0)
{
    float4 ambientColor;
    float4 diffuseColor;
    float3 lightDirection;
    float specularPower;
    float4 specularColor;
    float3 lightPosition;
};

cbuffer CameraBuffer : register(cb1)
{
    float3 cameraPosition;
    float padding;
};

// Typedefs
struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
	float3 position3D : TEXCOORD1;
};

// PIXEL SHADER
float4 main(PixelInputType input) : SV_TARGET
{
	//float4 textureColor;
	//float3 lightDir;
	//float lightIntensity;
	//float4 color;
 //   float attenuation;
 //   float slope;
	
	//// Sample the pixel color from the texture using the sampler at this texture coordinate location.
	//textureColor = shaderTexture.SampleLevel(SampleType, input.tex, 0);
	
	//// Set the default output color to the ambient light value for all pixels.
	//color = ambientColor;

 //   lightDir = -(lightPosition - input.position3D);

 //   float distance = length(lightDir);

 //   //lightDir /= distance;
	
 //   lightDir = normalize(lightDir);

	//// Calculate the amount of light on this pixel.
	//lightIntensity = saturate(dot(input.normal, -lightDir));
	
	//if (lightIntensity > 0.0f)
	//{
	//	// attenuation
 //       attenuation = 1.0f / (1.0f + 0.125f * distance + 0.0f * pow(distance, 2));

	//	// Determine the final diffuse color based on the diffuse color and the amount of light intensity and attenuation.
	//	color += (diffuseColor * lightIntensity * attenuation);
	
	//	// Saturate the ambient and diffuse color.
	//	color = saturate(color);
	//}
	
	//// Multiply the texture pixel and the input color to get the textured result.
	//color = color * textureColor;
	
	////// Add the specular component last to the output color.
	////color = saturate(color + specular);
	
	//return color;
 //   //return float4(1.0, 0.0, 0.0, 1.0);

    float4 textureColor;
    float3 lightDir;
    float lightIntensity;
    float4 color;
    float3 reflection;
    float4 specular;
    float3 viewDirection;

	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
    textureColor = shaderTexture.Sample(SampleType, input.tex);
	
	// Set the default output color to the ambient light value for all pixels.
    color = ambientColor;
	
	// Initialize the specular color.
    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
	
	// Invert the light direction for calculations.
    lightDir = -lightDirection;
	
	// Calculate the amount of light on this pixel.
    lightIntensity = saturate(dot(input.normal, lightDir));
	
    if (lightIntensity > 0.0f)
    {
		// Determine the final diffuse color based on the diffuse color and the amount of light intensity.
        color += (diffuseColor * lightIntensity);
	
		// Saturate the ambient and diffuse color.
        color = saturate(color);
	
		// Calculate the reflection vector based on the light intensity, normal vector, and light direction.
        reflection = normalize(2 * lightIntensity * input.normal - lightDir);
	
        viewDirection = cameraPosition.xyz - input.position.xyz;

		// Determine the amount of specular light based on the reflection vector, viewing direction, and specular power.
        specular = pow(saturate(dot(reflection, viewDirection)), specularPower);
    }
	
	// Multiply the texture pixel and the input color to get the textured result.
    color = color * textureColor;
	
	// Add the specular component last to the output color.
    color = saturate(color + specular);
	
    return color;
}