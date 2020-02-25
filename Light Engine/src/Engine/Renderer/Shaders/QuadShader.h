#define QuadShaderSrc_VS \
R"(
+GLSL
#version 450 core

layout(location = 0) in vec2 InPosition;
layout(location = 1) in vec4 InColor;
layout(location = 2) in vec3 InTexCoords;

layout(std140, binding = 0) uniform ViewProjectionVSUniform
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

out vec4 VSOutColor;
out vec3 VSOutTexCoords;

void main()
{
	gl_Position = ProjectionMatrix * ViewMatrix * vec4(InPosition, 0.0, 1.0);
	VSOutColor = InColor;
	VSOutTexCoords = InTexCoords;
}
-GLSL

+HLSL
struct VertexOut
{
	float4 Color : COLOR;
	float3 TexCoords : TEXCOORDS;
	float4 Position : SV_Position;
};

cbuffer	ViewVSConstant : register(b0)
{
	row_major matrix ViewMatrix;
	row_major matrix ProjectionMatrix;
}

VertexOut main(float2 InPosition : POSITION, float4 InColor : COLOR, float3 InTexCoords : TEXCOORDS) 
{
	VertexOut vso;
	vso.Position = mul(float4(InPosition, 0.0f, 1.0f), mul(ViewMatrix, ProjectionMatrix));
	vso.Color = InColor;
	vso.TexCoords = InTexCoords;
	return vso;
}
-HLSL)"

#define QuadShaderSrc_FS \
R"(
+GLSL
#version 450 core

out vec4 FSOutFragColor;

in vec4 VSOutColor;
in vec3 VSOutTexCoords;

uniform sampler2DArray u_TextureArray;

void main()
{
	FSOutFragColor = texture(u_TextureArray, VSOutTexCoords) * VSOutColor;
}
-GLSL

+HLSL
SamplerState samplerState : register(s0);
Texture2DArray textureArray : register(t0);

float4 main(float4 Color : COLOR, float3 TexCoords : TEXCOORDS) : SV_Target
{
	return textureArray.Sample(samplerState, TexCoords) * Color;
}
-HLSL)"