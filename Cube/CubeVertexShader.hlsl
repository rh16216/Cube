//fxc in C:\Program Files (x86)\Windows Kits\10\bin\10.0.17134.0\x64
//compile using fxc /T vs_4_0_level_9_0 /Fo CubeVertexShader.cso CubeVertexShader.hlsl

cbuffer ModelViewProjectionConstantBuffer : register(b0)
{
	matrix mWorld;      // world matrix for object
	matrix View;        // view matrix
	matrix Projection;  // projection matrix
};

struct VS_INPUT
{
	float3 vPos       : POSITION;
	float3 vColor     : COLOR0;
	float2 vTextureUV : TEXCOORD0; 
};

struct VS_OUTPUT
{
	float4 Position  : SV_POSITION;  // interpolated vertex position (system value)
	float4 Color     : COLOR0;       // interpolated diffuse color
	float2 TextureUV : TEXCOORD0;    // UV texture coordinates
};

VS_OUTPUT main(VS_INPUT input) // main is the default function name
{
	VS_OUTPUT Output;

	float4 pos = float4(input.vPos, 1.0f);
	// Transform the position from object space to homogeneous projection space
	pos = mul(pos, mWorld);
	pos = mul(pos, View);
	pos = mul(pos, Projection);
	Output.Position = pos;

	// Just pass through the color and texture data
	Output.Color = float4(input.vColor, 1.0f);
	Output.TextureUV = input.vTextureUV;

	return Output;
}