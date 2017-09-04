Texture2D texture0 : register(t0);
SamplerState Sampler0 : register(s0);

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};


float4 main(InputType input) : SV_TARGET
{
    float4 textureColour;
	
    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    textureColour = texture0.Sample(Sampler0, input.tex);
	//float4 colour = float4(1.0, 0.0, 0.0, 1.0);

	//textureColor = lerp(textureColor, colour, 0.5);

	return float4(1.0, 0.0, 0.0, 1.0);

    return textureColour;
}