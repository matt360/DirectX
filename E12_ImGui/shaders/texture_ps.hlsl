// PIXEL SHADER

Texture2D texture0 : register(t0);
Texture2D texture1 : register(t1);

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

	float4 finalCol;
	// invert colors on texture1
	//return 1 - textureColor1;

	// invert colors on texture2
	//return 1 - textureColor2;

	// blend texture1 and texture 1
	//return lerp(textureColor1, textureColor2, 0.5);

	// color shifting
    finalCol.x = textureColor1.z;
    finalCol.y = textureColor1.y;
    finalCol.z = textureColor1.x;
	finalCol.w = 1.0f;

    return finalCol;
}