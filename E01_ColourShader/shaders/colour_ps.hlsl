// Colour pixel/fragment shader
// Basic fragment shader outputting a colour

//////////////
// TYPEDEFS //
//////////////
// The pixel shader draws each pixel on the polygons that will be rendered to the screen. ->
struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
// -> In this pixel shader it uses PixelInputType as input and returns a float4 as output which represents the final pixel color. 
// Note that the pixel shader gets its input from the vertex shader output.
float4 main(PixelInputType input) : SV_TARGET
{

	
    // color the pixel yellow
    float4 colour = float4(1.0, 1.0, 0.0, 1.0); // equal read and equal green makes yellow
    
    //return color
    return float4(1.0, 1.0, 0.0, 1.0); // color the pixel yellow

    // This pixel shader program is very simple as we just tell it to color the pixel the same as the input value of the color.
    // return input.color;
}