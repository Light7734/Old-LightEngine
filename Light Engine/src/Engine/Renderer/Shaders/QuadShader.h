#define QuadShaderSrc_VS \
R"(
+GLSL
#version 450 core

layout(location = 0) in vec2 InPosition;
layout(location = 1) in vec4 InColor;
layout(location = 2) in vec2 InTexCoords;
layout(location = 3) in uint InTexIndex;

layout(std140, binding = 0) uniform ViewProjectionVSUniform
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

out vec4 VSOutColor;
out vec2 VSOutTexCoords;
out flat uint VSOutTexIndex;

void main()
{
	gl_Position = ProjectionMatrix * ViewMatrix * vec4(InPosition, 0.0, 1.0);
	VSOutColor = InColor;
	VSOutTexCoords = InTexCoords;
	VSOutTexIndex = InTexIndex;
}
-GLSL

+HLSL
struct VertexOut
{
	float4 Color : COLOR;
	float2 TexCoords : TEXCOORDS;
	float4 Position : SV_Position;
	nointerpolation uint TexIndex : TEXINDEX;
};

cbuffer	ViewVSConstant : register(b0)
{
	row_major matrix ViewMatrix;
}

cbuffer ProjectionVSConstant : register(b1)
{
	row_major matrix ProjectionMatrix;
}

VertexOut main(float2 InPosition : POSITION, float4 InColor : COLOR, float2 InTexCoords : TEXCOORDS, uint InTexIndex : TEXINDEX) 
{
	VertexOut vso;
	vso.Position = mul(float4(InPosition, 0.0f, 1.0f), mul(ViewMatrix, ProjectionMatrix));
	vso.Color = InColor;
	vso.TexCoords = InTexCoords;
	vso.TexIndex = InTexIndex;
	return vso;
}
-HLSL)"

#define QuadShaderSrc_FS \
R"(
+GLSL
#version 450 core

out vec4 FSOutFragColor;

in vec4 VSOutColor;
in vec2 VSOutTexCoords;
in flat uint VSOutTexIndex;

uniform sampler2D[16] u_Textures;

void main()
{
	FSOutFragColor = texture(u_Textures[VSOutTexIndex], VSOutTexCoords) * VSOutColor;
}
-GLSL
+HLSL
SamplerState samplerState : register(s0);
Texture2D textures[16] : register(t0);

float4 main(float4 Color : COLOR, float2 TexCoords : TEXCOORDS, nointerpolation uint TexIndex : TEXINDEX) : SV_Target
{
	uint test = TexIndex;
	// See Trello board's In Progress List for more information
	return textures[0].Sample(samplerState, TexCoords) * Color;
}
-HLSL)"