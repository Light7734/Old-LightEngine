+GLSL // GLSL VERTEX SHADER //
#version 450 core

layout(location = 0) in vec2 a_Pos;
layout(location = 1) in vec2 a_TexCoords;

out vec2 VSOutTexCoords;

void main()
{
	gl_Position = vec4(a_Pos, 0.0, 1.0);
	VSOutTexCoords = a_TexCoords;
}
-GLSL


+HLSL // HLSL VERTEX SHADER //
struct VertexOut
{
	float2 TexCoords : TEXCOORDS;
	float4 Position : SV_POSITION;
};

VertexOut main(float2 InPosition : POSITION, float2 InTexCoords : TEXCOORDS)
{
	VertexOut vso;
	vso.Position = float4(InPosition, 0.0f, 1.0f);
	vso.TexCoords = InTexCoords;
	return vso;
}
-HLSL