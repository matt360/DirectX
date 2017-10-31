// Tessellation vertex shader.
// Doesn't do much, could manipulate the control points
// Pass forward data, strip out some values not required for example.

/*
In the vertex shader we pass the vertices and color data through to the hull shader.
Since the vertices will be increasing with the tessellation we now do the transforming and such in the domain shader. 
The vertex shader's purpose is now just for vertex animation and passing data into the hull shader.
*/

struct InputType
{
    float3 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct OutputType
{
    float3 position : POSITION;
    float4 colour : COLOR;
};

OutputType main(InputType input)
{
    OutputType output;

	 // Pass the vertex position into the hull shader.
    output.position = input.position;
    
    // Pass the input color into the hull shader.
    output.colour = float4(1.0, 0.0, 0.0, 1.0);
    
    return output;
}
