// PIXEL SHADER

Texture2D tex0 : register(t0);
Texture2D tex1 : register(t1);

SamplerState SamplerType : register(s0);

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

float4 main(InputType input) : SV_TARGET
{

    float4 texture_colour_0;
    float4 texture_colour_1;

    texture_colour_0 = tex0.Sample(SamplerType, input.tex);
    texture_colour_1 = tex1.Sample(SamplerType, input.tex);

    return lerp(texture_colour_0, texture_colour_1, 0.5);
}