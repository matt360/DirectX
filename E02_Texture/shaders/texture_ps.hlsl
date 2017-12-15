// Texture pixel/fragment shader
// Basic fragment shader for rendering textured geometry

// texture resource
//  This will be our texture resource that will be used for rendering the texture on the model.
Texture2D texture0 : register(t0);
Texture2D texture1 : register(t1);
// The sampler state allows us to modify how the pixels are written to the polygon face when shaded.
// For example if the polygon is really far away and only makes up 8 pixels on the screen
// then we use the sample state to figure out which pixels or what combination of pixels will actually be drawn from the original texture. 
SamplerState Sampler0 : register(s0);

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};


float4 main(InputType input) : SV_TARGET
{
	float4 textureColor1;
	float4 textureColor2;

	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
	textureColor1 = texture0.Sample(Sampler0, input.tex);
	textureColor2 = texture1.Sample(Sampler0, input.tex);

	// invert colors on texture1
	//return 1 - textureColor1;

	// invert colors on texture2
	//return 1 - textureColor2;

	// blend texture1 and texture 1
	//return lerp(textureColor1, textureColor2, 0.5);

	// color shifting
	//float4 finalCol;
	//finalCol.x = textureColor1.z;
	//finalCol.y = textureColor1.y;
	//finalCol.z = textureColor1.x;
	//finalCol.w = 1.0f ;
    //return textureColor1;

    return textureColor1;
}