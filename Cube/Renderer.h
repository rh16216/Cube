//// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Copyright (c) Microsoft Corporation. All rights reserved
#pragma once


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "DeviceResources.h"

//-----------------------------------------------------------------------------
// Class declarations
//-----------------------------------------------------------------------------

class Renderer
{
public:
	Renderer(std::shared_ptr<DeviceResources> deviceResources);
	~Renderer();

	void CreateDeviceDependentResources();
	void CreateWindowSizeDependentResources();
	void Update(uint32_t colourFlag, uint32_t textureFlag, uint32_t pixelLightFlag, uint32_t vertexLightFlag);
	void Render();
	void SetTexture();
	void SetColour();


private:
	HRESULT CreateShaders();
	HRESULT CreateCube();
	HRESULT LoadTexture();
	void    CreateViewAndPerspective();
	void    CreateLightInfo();

	//-----------------------------------------------------------------------------
	// Pointer to device resource manager
	//-----------------------------------------------------------------------------
	std::shared_ptr<DeviceResources> m_deviceResources;

	//-----------------------------------------------------------------------------
	// Variables for rendering the cube
	//-----------------------------------------------------------------------------
	typedef struct _constantBufferStruct {
		DirectX::XMFLOAT4X4 world;
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 projection;
	} ConstantBufferStruct;

	// Assert that the constant buffer remains 16-byte aligned.
	static_assert((sizeof(ConstantBufferStruct) % 16) == 0, "Constant Buffer size must be 16-byte aligned");

	typedef struct _renderTypeStruct { //uint32_t used to ensure 16-byte alignment
		uint32_t colourFlag;
		uint32_t textureFlag;
		uint32_t pixelLightFlag;
		uint32_t vertexLightFlag;
	} RenderTypeStruct;

	// Assert that the render type buffer remains 16-byte aligned.
	static_assert((sizeof(RenderTypeStruct) % 16) == 0, "Render Type Buffer size must be 16-byte aligned");

	void SetTexture(uint32_t input) {
		m_renderTypeData.textureFlag = input;
	}

	void SetColour(uint32_t input) {
		m_renderTypeData.colourFlag = input;
	}

	void SetPixelLight(uint32_t input) {
		m_renderTypeData.pixelLightFlag = input;
	}

	void SetVertexLight(uint32_t input) {
		m_renderTypeData.vertexLightFlag = input;
	}

	typedef struct _lightBufferStruct {
		DirectX::XMFLOAT4 position;
		DirectX::XMFLOAT4 intensity;
	} LightBufferStruct;

	// Assert that the constant buffer remains 16-byte aligned.
	static_assert((sizeof(LightBufferStruct) % 16) == 0, "Light Buffer size must be 16-byte aligned");

	//-----------------------------------------------------------------------------
	// Per-vertex data
	//-----------------------------------------------------------------------------
	typedef struct _vertexPositionColor
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 color;
	} VertexPositionColor;

	//-----------------------------------------------------------------------------
	// Per-vertex data (extended)
	//-----------------------------------------------------------------------------
	typedef struct _vertexPositionColorTexture
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 texture;
	} VertexPositionColorTexture;

	ConstantBufferStruct m_constantBufferData;
	RenderTypeStruct m_renderTypeData;
	LightBufferStruct m_lightBufferData;
	unsigned int  m_indexCount;
	unsigned int  m_frameCount;

	//-----------------------------------------------------------------------------
	// Direct3D device resources
	//-----------------------------------------------------------------------------
	//ID3DXEffect* m_pEffect;
	Microsoft::WRL::ComPtr<ID3D11Buffer>              m_pVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>              m_pIndexBuffer;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>        m_pVertexShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>         m_pInputLayout;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>         m_pInputLayoutExtended;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>         m_pPixelShader;
	Microsoft::WRL::ComPtr<ID3D11Buffer>              m_pConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>              m_pRenderTypeBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>              m_pLightBuffer;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>           m_pTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>  m_pTextureView;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>        m_pSampler;
};