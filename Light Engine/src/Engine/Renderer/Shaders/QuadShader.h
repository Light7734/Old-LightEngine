#pragma once

#define QuadShaderSrc_VS \
R"(
+GLSL
#version 450 core

layout(location = 0) in vec2 InPosition;
layout(location = 1) in vec4 InColor;
layout(location = 2) in vec3 InTexCoords;

layout(std140, binding = 6) uniform ViewProjectionVSUniform
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

out VS_OUT
{
	vec4 Color;
	vec3 TexCoords;
} VertexOut;

void main()
{
	gl_Position = ProjectionMatrix * ViewMatrix * vec4(InPosition, 0.0, 1.0);
	VertexOut.Color = InColor;
	VertexOut.TexCoords = InTexCoords;
}
-GLSL

+HLSL
struct VertexOut
{
	float4 Color : COLOR;
	float3 TexCoords : TEXCOORDS;
	float4 Position : SV_Position;
};

cbuffer	ViewVSConstant : register(b6)
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


in VS_OUT
{
	vec4 Color;
	vec3 TexCoords;
} FragmentIn;

uniform sampler2DArray u_TextureArray;

void main()
{
	FSOutFragColor = texture(u_TextureArray, FragmentIn.TexCoords);
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