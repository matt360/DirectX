// PIXEL SHADER

// Tessellation pixel shader
// Output colour passed to stage.

struct DomainOut
{
    float4 position : SV_POSITION;
    float4 colour : COLOR;
};

float4 main(DomainOut input) : SV_TARGET
{
    return input.colour;
}