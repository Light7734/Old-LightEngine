#pragma once

#define QuadShaderSrc_VS \
R"(
+GLSL
#version 450 core

layout(location = 0) in vec2 InPosition;
layout(location = 1) in vec3 InTexCoords;
layout(location = 2) in vec4 InColor;

layout(std140, binding = 6) uniform ViewProjectionVSUniform
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

out VS_OUT
{
	vec3 TexCoords;
	vec4 Color;
} VertexOut;

void main()
{
	gl_Position = ProjectionMatrix * ViewMatrix * vec4(InPosition, 0.0, 1.0);
	VertexOut.TexCoords = InTexCoords;
	VertexOut.Color = InColor;
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

VertexOut main(float2 InPosition : POSITION, float3 InTexCoords : TEXCOORDS, float4 InColor : COLOR)
{
	VertexOut vso;

	vso.Position = mul(float4(InPosition, 0.0, 1.0), mul(ViewMatrix, ProjectionMatrix));

	vso.TexCoords = InTexCoords;
	vso.Color = InColor;

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
	vec3 TexCoords;
	vec4 Color;
} FragmentIn;

uniform sampler2DArray u_TextureArray; // #BINDING_TEXTUREARRAY0

void main()
{
	FSOutFragColor = texture(u_TextureArray, FragmentIn.TexCoords) * FragmentIn.Color;
}
-GLSL

+HLSL
SamplerState samplerState : register(s0);
Texture2DArray textureArray : register(BINDING_TEXTUREARRAY0);

float4 main(float4 Color : COLOR, float3 TexCoords : TEXCOORDS) : SV_Target
{
	return textureArray.Sample(samplerState, TexCoords) * Color;
}
-HLSL)"