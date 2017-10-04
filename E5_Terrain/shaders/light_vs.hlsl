// Light vertex shader
// Standard issue vertex shader, apply matrices, pass info to pixel shader

// Vertex buffer 
// Each of the triangles in the sphere model has three points to it, we call each point a vertex. So for us to render the sphere model we need to put all the vertices that form the sphere into a special 
// data array that we call a vertex buffer. Once all the points of the sphere model are in the vertex buffer we can then send the vertex buffer to the GPU so that it can render the model.

// The vertex shader program will be called by the GPU for each vertex it needs to process.
// For example a 5,000 polygon model will run your vertex shader program 15,000 times each frame just to draw that single model.
// So if you lock your graphics program to 60 fps it will call your vertex shader 900,000 times a second to draw just 5,000 triangles. As you can tell writing efficient vertex shaders is important.

// HLSL is the language we use in DirectX 11 to code these small vertex and pixel shader programs. 
// The syntax is pretty much identical to the C language with some pre-defined types. 
// HLSL program files are composed of global variables, type defines, vertex shaders, pixel shaders, and geometry shaders. 

/////////////
// GLOBALS //
/////////////
// Global
cbuffer MatrixBuffer : register(cb0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

// Global
//cbuffer CameraBuffer : register(cb1)
//{
//	float3 cameraPosition;
//	float paddingCamera;
//};

// Global
cbuffer TimeBuffer : register(cb1)
{
    float time;
    float height;
    float frequency;
    float padding;
};

//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct PixelInputType
{
    float4 position : SV_POSITION; // SV - system value
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
	float3 position3D : TEXCOORD1;
};

PixelInputType main(VertexInputType input)
{
    //  return output;
    PixelInputType output;
    float heightWave = height;
	
	// Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    //  offset position based on sine wave
    input.position.x += heightWave * sin((input.position.y + time) * frequency);
    input.position.y += heightWave * sin((input.position.x + time) * frequency);
    input.position.z += heightWave * sin((input.position.y + time) * frequency);

    input.position.x += heightWave * sin((input.normal.y + time) * frequency);
    input.position.y += heightWave * sin((input.normal.x + time) * frequency);
    input.position.z += heightWave * sin((input.normal.y + time) * frequency);

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
    output.normal = mul(input.normal, (float3x3) worldMatrix);
    output.normal = normalize(output.normal);

    return output;
}