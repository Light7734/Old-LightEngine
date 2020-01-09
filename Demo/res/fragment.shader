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

-HLSL