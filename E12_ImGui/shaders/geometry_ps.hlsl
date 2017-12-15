Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

//uniform sampler2D tex0;
//in vec2 Vertex_UV;
//in vec4 Vertex_Color;
//out vec4 FragColor;

float4 main(InputType input) : SV_TARGET
{
    //vec2 uv = Vertex_UV.xy;
    //uv.y *= -1.0;
    //vec3 t = texture(tex0, uv).rgb;
    //FragColor = vec4(t, 1.0) * Vertex_Color;

    //float2 uv = input.tex.xy;

    //uv.y *= -1.0f;

    float4 textureColor;
    textureColor = shaderTexture.SampleLevel(SampleType, input.tex, 0);

    return textureColor;
}