+GLSL
#version 450 core

layout(location = 0) in vec2 a_Pos;
layout(location = 1) in vec3 a_Col;

out vec3 VSOut_Color;

void main()
{
	gl_Position = vec4(a_Pos, 0.0, 1.0);
	VSOut_Color = a_Col;
}

-GLSL

+HLSL

struct VSOut
{
	float3 col : Color;
	float4 pos : SV_Position;
};


VSOut main(float2 pos : Position, float3 col : Color) 
{
	VSOut vso;
	vso.pos = float4(pos.x, pos.y, 0.0f, 1.0f);
	vso.col = col;
	return vso;
}

-HLSL