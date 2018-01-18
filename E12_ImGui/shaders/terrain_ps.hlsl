// PIXEL SHADER

// TODO delete
// Pixel shaders are small programs that are written for doing the coloring of the polygons that we draw. 
// They are run by the GPU for every visible pixel that will be drawn to the screen.
// Coloring, texturing, lighting, and most other effects you plan to do to your polygon faces are handled by the pixel shader program.
// Pixel shaders must be efficiently written due to the number of times they will be called by the GPU.

// Textures
Texture2D tex1 : register(t0);
Texture2D tex2 : register(t1);
Texture2D height_tex : register(t2);

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

cbuffer TimeBuffer : register(cb1)
{
    float time;
    float height;
    float frequency;
    float choice;
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
	float4 textureCol1;
    float4 textureCol2;
    float4 heightCol;
    float4 shiftCol;
    float4 shiftCol1;
    float4 shiftCol2;
	float4 color;

	float3 lightDir;
	float lightIntensity;
    float attenuation;

    float slope;
    float blendAmount;
	
	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
    textureCol1 = tex1.SampleLevel(SampleType, input.tex, 0);
    textureCol2 = tex2.SampleLevel(SampleType, input.tex, 0);
    heightCol = height_tex.SampleLevel(SampleType, input.tex, 0);
	
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
    switch ((int) choice)
    {
        // texture 1 with lighting
        case 0:
            color = color * textureCol1;
            break;
        // texture 2 with lighting
        case 1:
            color = color * textureCol2;
            break;

        case 2:
            color = color * lerp(textureCol1, textureCol2, frequency);
            break;

        case 3:
            // invert colors on texture1
            color = 1 - color * textureCol1;
            break;

        case 4:
            // invert colors on texture2
            color = 1 - color * textureCol2;
            break;

        case 5:
            // invert colors on blended texture1 and texture2
            color = 1 - color * lerp(textureCol1, textureCol2, frequency);
            break;

        case 6:
            // shift colours on texture 1
            shiftCol.x = textureCol1.z;
            shiftCol.y = textureCol1.y;
            shiftCol.z = textureCol1.x;
            shiftCol.w = 1.0f;
            color = color * shiftCol;
            break;

        case 7:
            // shift colours on texture 2
            shiftCol.x = textureCol2.z;
            shiftCol.y = textureCol2.y;
            shiftCol.z = textureCol2.x;
            shiftCol.w = 1.0f;
            color = color * shiftCol;
            break;

        case 8:
            // shift colours on blended texture 1 and texture 2
            shiftCol1.x = textureCol1.z;
            shiftCol1.y = textureCol1.y;
            shiftCol1.z = textureCol1.x;
            shiftCol1.w = 1.0f;

            shiftCol2.x = textureCol2.z;
            shiftCol2.y = textureCol2.y;
            shiftCol2.z = textureCol2.x;
            shiftCol2.w = 1.0f;

            color = color * lerp(shiftCol1, shiftCol2, frequency);
            break;
    }
	
    return color;

	//// Add the specular component last to the output color.
	//color = saturate(color + specular);
}