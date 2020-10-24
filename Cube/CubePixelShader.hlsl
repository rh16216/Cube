//fxc in C:\Program Files (x86)\Windows Kits\10\bin\10.0.17134.0\arm64
//compile using fxc /T ps_4_0_level_9_0 /Fo CubePixelShader.cso CubePixelShader.hlsl

Texture2D simpleTexture : register(t0);
//SamplerState simpleSampler : register(s0);

SamplerState simpleTextureSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

struct PS_INPUT
{
	float4 Position  : SV_POSITION;  // interpolated vertex position (system value)
	float4 Color     : COLOR0;       // interpolated diffuse color
	float2 TextureUV : TEXCOORD0;    // UV texture coordinates
};


struct PS_OUTPUT
{
	float4 RGBColor    : SV_TARGET;       // interpolated diffuse color
};

PS_OUTPUT main(PS_INPUT In)
{
	PS_OUTPUT Output;
	Output.RGBColor = simpleTexture.Sample(simpleTextureSampler, In.TextureUV)*In.Color;
	//Output.RGBColor = In.Color;

	return Output;
}