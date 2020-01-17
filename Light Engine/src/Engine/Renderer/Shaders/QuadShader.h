#define QuadShaderSrc_VS \
R"(
+GLSL
#version 450 core

layout(location = 0) in vec2 InPosition;
layout(location = 1) in vec4 InColor;

layout(std140, binding = 0) uniform ViewProjectionUniform
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

out vec4 OutColor;

void main()
{
	gl_Position = ProjectionMatrix * ViewMatrix * vec4(InPosition, 0.0, 1.0);
	OutColor = InColor;
}
-GLSL

+HLSL
struct VertexOut
{
	float4 Color : COLOR;
	float4 Position : SV_Position;
};

cbuffer	ViewVSConstant : register(b0)
{
	row_major matrix ViewMatrix;
}

cbuffer ProjectionVSConstant : register(b1)
{
	row_major matrix ProjectionMatrix;
}

VertexOut main(float2 InPosition : POSITION, float4 InColor : COLOR) 
{
	VertexOut vso;
	vso.Position = mul(float4(InPosition, 0.0f, 1.0f), mul(ViewMatrix, ProjectionMatrix));
	vso.Color = InColor;
	return vso;
}
-HLSL)"

#define QuadShaderSrc_FS \
R"(
+GLSL
#version 450 core

out vec4 OutFS;

in vec4 OutColor;

void main()
{
	OutFS = vec4(OutColor);
}
-GLSL

+HLSL
float4 main(float4 Color : COLOR) : SV_Target
{
	return float4(Color);
}
-HLSL)"