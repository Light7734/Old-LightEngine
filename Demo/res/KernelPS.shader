#pragma once

+GLSL // GLSL PIXEL SHADER //
#version 450 core

out vec4 FSOutFragmentColor;

in vec2 VSOutTexCoords;

uniform sampler2D u_Texture;

layout(std140, binding = 0) uniform KarnelFSUniform
{
	vec4 ConvolutionMatrix[3];
	vec2 KarnelOffset;
	vec2 padding;
};

void main()
{
    const vec2 offsets[9] = {
        const vec2(-KarnelOffset.x,  KarnelOffset.y),
        const vec2( 0.0f,            KarnelOffset.y),
        const vec2( KarnelOffset.x,  KarnelOffset.y),
        const vec2(-KarnelOffset.x,  0.0f          ),
        const vec2( 0.0f,            0.0f          ),
        const vec2( KarnelOffset.x,  0.0f          ),	
        const vec2(-KarnelOffset.x, -KarnelOffset.y),
        const vec2( 0.0f,           -KarnelOffset.y),
        const vec2( KarnelOffset.x, -KarnelOffset.y),
     };							   

    vec3 col = vec3(0.0f, 0.0f, 0.0f);
	for(uint i = 0; i < 9; i++)
		col += vec3( texture(u_Texture, VSOutTexCoords.st + offsets[i]).rgb ) * ConvolutionMatrix[i / 4][i % 4];

    FSOutFragmentColor = vec4(col, 1.0);
}  
-GLSL


+HLSL // HLSL PIXEL SHADER //
Texture2D frameTexture : register(t0);
SamplerState linearSampler : register(s0);

cbuffer	KarnelFSConstant : register(b0)
{
	float4 ConvolutionMatrix[3];
	float2 KarnelOffset;
	float2 padding;
}

float4 main(float2 TexCoords : TEXCOORDS) : SV_TARGET
{
    float2 offsets[9] = {
       float2(-KarnelOffset.x, -KarnelOffset.y),
       float2( 0.0f,           -KarnelOffset.y),
       float2( KarnelOffset.x, -KarnelOffset.y),
       float2(-KarnelOffset.x, 0.0f        ),
       float2( 0.0f,           0.0f        ),
       float2( KarnelOffset.x, 0.0f        ),
       float2(-KarnelOffset.x, KarnelOffset.y),
       float2( 0.0f,           KarnelOffset.y),
       float2( KarnelOffset.x, KarnelOffset.y),
    };

    float3 col = float3(0.0f, 0.0f, 0.0f);
	for(uint i = 0; i < 9; i++)
		col += float3( (frameTexture.Sample(linearSampler, float2(TexCoords.x + offsets[i].x, 1.0f - TexCoords.y + offsets[i].y)).xyz )).xyz * ConvolutionMatrix[i / 4][ i % 4];

	return float4(col, 1.0f);
}
-HLSL
