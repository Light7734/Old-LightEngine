#define QuadShaderSrc_VS \
R"(
+GLSL
#version 450 core

layout(location = 0) in vec2 a_Pos;
layout(location = 1) in vec4 a_Col;

out vec4 VSOut_Color;

void main()
{
	gl_Position = vec4(a_Pos, 0.0, 1.0);
	VSOut_Color = a_Col;
}

-GLSL

+HLSL

struct VSOut
{
	float4 col : Color;
	float4 pos : SV_Position;
};


VSOut main(float2 pos : Position, float4 col : Color) 
{
	VSOut vso;
	vso.pos = float4(pos.x, pos.y, 0.0f, 1.0f);
	vso.col = col;
	return vso;
}

-HLSL)"

#define QuadShaderSrc_FS \
R"(
+GLSL
#version 450 core

out vec4 FragmentColor;

in vec4 VSOut_Color;

void main()
{
	FragmentColor = vec4(VSOut_Color);
}

-GLSL

+HLSL

float4 main(float4 col : Color) : SV_Target
{
	return float4(col);
}

-HLSL)"
