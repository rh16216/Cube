//fxc in C:\Program Files (x86)\Windows Kits\10\bin\10.0.17134.0\x64
//compile using fxc /T ps_4_0_level_9_0 /Fo CubePixelShader.cso CubePixelShader.hlsl

Texture2D simpleTexture : register(t0);
SamplerState simpleTextureSampler : register(s0);

cbuffer RenderTypeBuffer : register(b0)
{
	int colourFlag;
	int textureFlag;
	int pixelLightFlag;
	int vertexLightFlag;
};

cbuffer LightBuffer : register(b1)
{
	float4 lightPosition;
	float4 lightIntensity;
};

/*
SamplerState simpleTextureSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};
*/
struct PS_INPUT
{
	float4 Position   : SV_POSITION;  // interpolated pixel position (system value)
	float4 Position3d : POSITION;     // interpolated vertex position (accessible by Pixel Shader)
	float4 Color      : COLOR0;       // interpolated diffuse color
	float4 Light      : COLOR1;       // interpolated vertex lighting
	float2 TextureUV  : TEXCOORD0;    // UV texture coordinates
};


struct PS_OUTPUT
{
	float4 RGBColor    : SV_TARGET;       // interpolated diffuse color
};

PS_OUTPUT main(PS_INPUT In)
{
	PS_OUTPUT Output;

	float4 finalColour = { 1.0f, 1.0f, 1.0f, 1.0f };
	float4 finalTexture = { 1.0f, 1.0f, 1.0f, 1.0f };
	float4 finalLight = { 1.0f, 1.0f, 1.0f, 1.0f };
	
	if (colourFlag) {
		finalColour = In.Color;
	}

	if (textureFlag) {
		finalTexture = simpleTexture.Sample(simpleTextureSampler, In.TextureUV);
	}

	if (pixelLightFlag) {
		float lightDistanceSquared = dot((lightPosition - In.Position3d),(lightPosition - In.Position3d));
		finalLight = lightIntensity/(4*3.14*lightDistanceSquared);
	}
	
	if (vertexLightFlag) {
		finalLight = In.Light;
	}


	Output.RGBColor = finalTexture*finalColour*finalLight;
	//Output.RGBColor = In.Color;

	return Output;
}