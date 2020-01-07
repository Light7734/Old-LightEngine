+GLSL
#version 450 core

out vec4 FragmentColor;

in vec3 VSOut_Color;

void main()
{
	FragmentColor = vec4(VSOut_Color, 1.0);
}

-GLSL

+HLSL

float4 main(float3 col : Color) : SV_Target
{
	return float4(col, 1.0f);
}

-HLSL