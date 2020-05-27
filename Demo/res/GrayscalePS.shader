+GLSL // GLSL PIXEL SHADER //
#version 450 core

out vec4 FSOutFragmentColor;

in vec2 VSOutTexCoords;

uniform sampler2D u_Texture; // #BINDING_FRAMEBUFFER0

void main()
{
	vec3 colors = texture(u_Texture, VSOutTexCoords).rgb;
	float avg = 0.2126 * colors.r + 0.7152 * colors.g + 0.0722 * colors.b;;
	FSOutFragmentColor = vec4(avg, avg, avg, 1.0);
}
-GLSL


+HLSL // HLSL PIXEL SHADER //
SamplerState linearSampler : register(s0);
Texture2D frameTexture : register(BINDING_FRAMEBUFFER0);

float4 main(float2 TexCoords : TEXCOORDS) : SV_TARGET
{
	float3 colors = frameTexture.Sample(linearSampler, float2(TexCoords.x, 1.0f - TexCoords.y)).rgb;
	float avg = 0.2126 * colors.r + 0.7152 * colors.g + 0.0722 * colors.b;
	return float4(avg, avg, avg, 1.0f);
}
-HLSL
