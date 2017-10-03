// Light vertex shader
// Standard issue vertex shader, apply matrices, pass info to pixel shader

cbuffer MatrixBuffer : register(cb0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

//cbuffer CameraBuffer : register(cb1)
//{
//	float3 cameraPosition;
//	float paddingCamera;
//};

cbuffer TimeBuffer : register(cb1)
{
    float time;
    float height;
    float frequency;
    float padding;
};

struct InputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct OutputType
{
    float4 position : SV_POSITION; // SV - system value
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
	float3 position3D : TEXCOORD1;
};

OutputType main(InputType input)
{
  //  OutputType output;
  //  float4 worldPosition;
    
  //  input.position.w = 1.0f;

  //   Calculate the position of the vertex against the world, view, and projection matrices.
  //  output.position = mul(input.position, worldMatrix);
  //  output.position = mul(output.position, viewMatrix);
  //  output.position = mul(output.position, projectionMatrix);
    
  //   Store the texture coordinates for the pixel shader.
  //  output.tex = input.tex;

	 // Calculate the normal vector against the world matrix only.
  //  output.normal = mul(input.normal, (float3x3) worldMatrix);
	
  //   Normalize the normal vector.
  //  output.normal = normalize(output.normal);

	 //Calculate the position of the vertex in the world
  //  worldPosition = mul(input.position, worldMatrix);

	 //Determine the viewing direction based on the position of the camera and the position of the vertex in the world
  //  output.viewDirection = cameraPosition.xyz - worldPosition.xyz;
	
	 //Normalize the viewing direction vector.
  //  output.viewDirection = normalize(output.viewDirection);

  //  return output;
    OutputType output;
    float heightWave = height;
	
	// Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

   //  offset position based on sine wave
    input.position.x += heightWave * sin((input.normal.x + time) * frequency);
    input.position.y += heightWave * sin((input.normal.y + time) * frequency);
    input.position.z += heightWave * sin((input.normal.z + time) * frequency);

    input.normal.x = 1 - cos(input.position.x + time);
    input.normal.y = abs(cos(input.position.y + time));
    input.normal.z = abs(cos(input.position.y + time));

	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
	// Store the texture coordinates for the pixel shader.
    output.tex = input.tex;

	// Store normals for the pixel shader
    //output.position3D = mul(input.position, worldMatrix);
    output.normal = mul(input.normal, (float3x3) worldMatrix);
    output.normal = normalize(output.normal);

    return output;
}