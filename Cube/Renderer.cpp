//// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Copyright (c) Microsoft Corporation. All rights reserved


//-----------------------------------------------------------------------------
// File: Cube.cpp
//
// Renders a spinning, colorful cube.
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "dxstdafx.h"
#include "resource.h"

#include <string>
#include <memory>
#include <ppltasks.h>

#include "Renderer.h"


//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
Renderer::Renderer(std::shared_ptr<DeviceResources> deviceResources)
	:
	m_frameCount(0),
	m_deviceResources(deviceResources)
{
	m_frameCount = 0; // init frame count
}

//-----------------------------------------------------------------------------
// Create Direct3D shader resources by loading the .cso files.
//-----------------------------------------------------------------------------
HRESULT Renderer::CreateShaders()
{
	HRESULT hr = S_OK;

	// Use the Direct3D device to load resources into graphics memory.
	ID3D11Device* device = m_deviceResources->GetDevice();

	// You'll need to use a file loader to load the shader bytecode. In this
	// example, we just use the standard library.
	FILE* vShader, *pShader;
	BYTE* bytes;

	size_t destSize = 4096;
	size_t bytesRead = 0;
	bytes = new BYTE[destSize];

	fopen_s(&vShader, "C:/Users/Rudy/source/repos/Cube/Cube/CubeVertexShader.cso", "rb");
	bytesRead = fread_s(bytes, destSize, 1, 4096, vShader);
	hr = device->CreateVertexShader(
		bytes,
		bytesRead,
		nullptr,
		&m_pVertexShader
	);

	D3D11_INPUT_ELEMENT_DESC iaDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,
		0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	hr = device->CreateInputLayout(
		iaDesc,
		ARRAYSIZE(iaDesc),
		bytes,
		bytesRead,
		&m_pInputLayout
	);

	delete bytes;


	bytes = new BYTE[destSize];
	bytesRead = 0;
	fopen_s(&pShader, "C:/Users/Rudy/source/repos/Cube/Cube/CubePixelShader.cso", "rb");
	bytesRead = fread_s(bytes, destSize, 1, 4096, pShader);
	hr = device->CreatePixelShader(
		bytes,
		bytesRead,
		nullptr,
		m_pPixelShader.GetAddressOf()
	);

	delete bytes;

	CD3D11_BUFFER_DESC cbDesc(
		sizeof(ConstantBufferStruct),
		D3D11_BIND_CONSTANT_BUFFER
	);

	hr = device->CreateBuffer(
		&cbDesc,
		nullptr,
		m_pConstantBuffer.GetAddressOf()
	);

	CD3D11_BUFFER_DESC rtbDesc(
		sizeof(RenderTypeStruct),
		D3D11_BIND_CONSTANT_BUFFER
	);

	hr = device->CreateBuffer(
		&rtbDesc,
		nullptr,
		m_pRenderTypeBuffer.GetAddressOf()
	);

	CD3D11_BUFFER_DESC lbDesc(
		sizeof(LightBufferStruct),
		D3D11_BIND_CONSTANT_BUFFER
	);

	hr = device->CreateBuffer(
		&lbDesc,
		nullptr,
		m_pLightBuffer.GetAddressOf()
	);

	fclose(vShader);
	fclose(pShader);

	return hr;
}

//-----------------------------------------------------------------------------
// Create the cube:
// Creates the vertex buffer and index buffer.
//-----------------------------------------------------------------------------
HRESULT Renderer::CreateCube()
{
	HRESULT hr = S_OK;

	// Use the Direct3D device to load resources into graphics memory.
	ID3D11Device* device = m_deviceResources->GetDevice();

	// Create cube geometry.
	VertexPositionColorTexture CubeVertices[] =
	{
		//left
		{DirectX::XMFLOAT3(-0.5f,-0.5f,-0.5f), DirectX::XMFLOAT3(0,   0,   0), DirectX::XMFLOAT2(0,   0),},
		{DirectX::XMFLOAT3(-0.5f,-0.5f, 0.5f), DirectX::XMFLOAT3(0,   0,   1), DirectX::XMFLOAT2(0,   1),},
		{DirectX::XMFLOAT3(-0.5f, 0.5f,-0.5f), DirectX::XMFLOAT3(0,   1,   0), DirectX::XMFLOAT2(1,   0),},
		{DirectX::XMFLOAT3(-0.5f, 0.5f, 0.5f), DirectX::XMFLOAT3(0,   1,   1), DirectX::XMFLOAT2(1,   1),},

		//back
		{DirectX::XMFLOAT3( 0.5f,-0.5f,-0.5f), DirectX::XMFLOAT3(1,   0,   0), DirectX::XMFLOAT2(0,   0),},
		{DirectX::XMFLOAT3(-0.5f,-0.5f,-0.5f), DirectX::XMFLOAT3(0,   0,   0), DirectX::XMFLOAT2(0,   1),},
		{DirectX::XMFLOAT3( 0.5f, 0.5f,-0.5f), DirectX::XMFLOAT3(1,   1,   0), DirectX::XMFLOAT2(1,   0),},
		{DirectX::XMFLOAT3(-0.5f, 0.5f,-0.5f), DirectX::XMFLOAT3(0,   1,   0), DirectX::XMFLOAT2(1,   1),},

		//front
		{DirectX::XMFLOAT3(-0.5f,-0.5f, 0.5f), DirectX::XMFLOAT3(0,   0,   1), DirectX::XMFLOAT2(0,   0),},
		{DirectX::XMFLOAT3( 0.5f,-0.5f, 0.5f), DirectX::XMFLOAT3(1,   0,   1), DirectX::XMFLOAT2(0,   1),},
		{DirectX::XMFLOAT3(-0.5f, 0.5f, 0.5f), DirectX::XMFLOAT3(0,   1,   1), DirectX::XMFLOAT2(1,   0),},
		{DirectX::XMFLOAT3( 0.5f, 0.5f, 0.5f), DirectX::XMFLOAT3(1,   1,   1), DirectX::XMFLOAT2(1,   1),},

		//top
		{DirectX::XMFLOAT3(-0.5f, 0.5f,-0.5f), DirectX::XMFLOAT3(0,   1,   0), DirectX::XMFLOAT2(0,   0),},
		{DirectX::XMFLOAT3(-0.5f, 0.5f, 0.5f), DirectX::XMFLOAT3(0,   1,   1), DirectX::XMFLOAT2(0,   1),},
		{DirectX::XMFLOAT3( 0.5f, 0.5f,-0.5f), DirectX::XMFLOAT3(1,   1,   0), DirectX::XMFLOAT2(1,   0),},
		{DirectX::XMFLOAT3( 0.5f, 0.5f, 0.5f), DirectX::XMFLOAT3(1,   1,   1), DirectX::XMFLOAT2(1,   1),},

		//bottom
		{DirectX::XMFLOAT3(-0.5f,-0.5f,-0.5f), DirectX::XMFLOAT3(0,   0,   0), DirectX::XMFLOAT2(0,   0),},
		{DirectX::XMFLOAT3(-0.5f,-0.5f, 0.5f), DirectX::XMFLOAT3(0,   0,   1), DirectX::XMFLOAT2(0,   1),},
		{DirectX::XMFLOAT3( 0.5f,-0.5f,-0.5f), DirectX::XMFLOAT3(1,   0,   0), DirectX::XMFLOAT2(1,   0),},
		{DirectX::XMFLOAT3( 0.5f,-0.5f, 0.5f), DirectX::XMFLOAT3(1,   0,   1), DirectX::XMFLOAT2(1,   1),},

		//right
		{DirectX::XMFLOAT3( 0.5f,-0.5f, 0.5f), DirectX::XMFLOAT3(1,   0,   1), DirectX::XMFLOAT2(0,   0),},
		{DirectX::XMFLOAT3( 0.5f,-0.5f,-0.5f), DirectX::XMFLOAT3(1,   0,   0), DirectX::XMFLOAT2(0,   1),},
		{DirectX::XMFLOAT3( 0.5f, 0.5f, 0.5f), DirectX::XMFLOAT3(1,   1,   1), DirectX::XMFLOAT2(1,   0),},
		{DirectX::XMFLOAT3( 0.5f, 0.5f,-0.5f), DirectX::XMFLOAT3(1,   1,   0), DirectX::XMFLOAT2(1,   1),},
	};

	// Create vertex buffer:

	CD3D11_BUFFER_DESC vDesc(
		sizeof(CubeVertices),
		D3D11_BIND_VERTEX_BUFFER
	);

	D3D11_SUBRESOURCE_DATA vData;
	ZeroMemory(&vData, sizeof(D3D11_SUBRESOURCE_DATA));
	vData.pSysMem = CubeVertices;
	vData.SysMemPitch = 0;
	vData.SysMemSlicePitch = 0;

	hr = device->CreateBuffer(
		&vDesc,
		&vData,
		&m_pVertexBuffer
	);

	// Create index buffer:
	unsigned short CubeIndices[] =
	{
		0,2,1, // left
		1,2,3,

		4,6,5, // back
		5,6,7,

		8,10,9, // front
		9,10,11,

		12,14,13, // top
		13,14,15,

		16,18,17, // bottom
		17,18,19,

		20,22,21, // right
		21,22,23,
	};

	m_indexCount = ARRAYSIZE(CubeIndices);

	CD3D11_BUFFER_DESC iDesc(
		sizeof(CubeIndices),
		D3D11_BIND_INDEX_BUFFER
	);

	D3D11_SUBRESOURCE_DATA iData;
	ZeroMemory(&iData, sizeof(D3D11_SUBRESOURCE_DATA));
	iData.pSysMem = CubeIndices;
	iData.SysMemPitch = 0;
	iData.SysMemSlicePitch = 0;

	hr = device->CreateBuffer(
		&iDesc,
		&iData,
		&m_pIndexBuffer
	);

	return hr;
}

HRESULT Renderer::LoadTexture()
{
	HRESULT hr = S_OK;

	// Use the Direct3D device to load resources into graphics memory.
	ID3D11Device* device = m_deviceResources->GetDevice();

	FILE* textureFile;
	BYTE* bytes;
	BYTE* bytesBGRA;

	size_t destSize = 480*395*4;
	size_t headerSize = 121;
	size_t bytesRead = 0;
	bytes = new BYTE[headerSize+destSize];
	bytesBGRA = new BYTE[destSize];

	fopen_s(&textureFile, "C:/Users/Rudy/source/repos/Cube/Cube/texture.bmp", "rb");
	bytesRead = fread_s(bytes, destSize+headerSize, 1, 480*395*4+121, textureFile);
	
	//Process header
	for (int i = 0; i < destSize; i++) {
		bytesBGRA[i] = bytes[i + headerSize];
	}

	//Convert from ARGB to BGRA
	for (int j = 0; j < destSize / 4; j++) { 
		BYTE alpha = bytesBGRA[4*j];

		bytesBGRA[4*j]     = bytesBGRA[4*j + 1];
		bytesBGRA[4*j + 1] = bytesBGRA[4*j + 2];
		bytesBGRA[4*j + 2] = bytesBGRA[4*j + 3];
		bytesBGRA[4*j + 3] = alpha;
	
	}

	D3D11_SUBRESOURCE_DATA tData;
	ZeroMemory(&tData, sizeof(D3D11_SUBRESOURCE_DATA));
	tData.pSysMem = bytesBGRA;
	tData.SysMemPitch = 480*4;
	tData.SysMemSlicePitch = 0;

	D3D11_TEXTURE2D_DESC desc;
	desc.Width = 480;
	desc.Height = 395;
	desc.MipLevels = desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;

	hr = device->CreateTexture2D(&desc, &tData, &m_pTexture);

	CD3D11_SHADER_RESOURCE_VIEW_DESC textureViewDesc(D3D11_SRV_DIMENSION_TEXTURE2D);

	hr = device->CreateShaderResourceView(
		m_pTexture.Get(),
		&textureViewDesc,
		&m_pTextureView
	);

	// Create the sample state
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = device->CreateSamplerState(&sampDesc, &m_pSampler);

	delete bytes;
	delete bytesBGRA;
	fclose(textureFile);

	return hr;
}

//-----------------------------------------------------------------------------
// Create the view matrix and create the perspective matrix.
//-----------------------------------------------------------------------------
void Renderer::CreateViewAndPerspective()
{
	// Use DirectXMath to create view and perspective matrices.

	DirectX::XMVECTOR eye = DirectX::XMVectorSet(0.0f, 0.7f, 1.5f, 0.f);
	DirectX::XMVECTOR at = DirectX::XMVectorSet(0.0f, -0.1f, 0.0f, 0.f);
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.f);

	DirectX::XMStoreFloat4x4(
		&m_constantBufferData.view,
		DirectX::XMMatrixTranspose(
			DirectX::XMMatrixLookAtRH(
				eye,
				at,
				up
			)
		)
	);

	float aspectRatio = m_deviceResources->GetAspectRatio();

	DirectX::XMStoreFloat4x4(
		&m_constantBufferData.projection,
		DirectX::XMMatrixTranspose(
			DirectX::XMMatrixPerspectiveFovRH(
				DirectX::XMConvertToRadians(70),
				aspectRatio,
				0.01f,
				100.0f
			)
		)
	);
}

void Renderer::CreateLightInfo()
{
	// Use DirectXMath to create view and perspective matrices.

	DirectX::XMVECTOR position = DirectX::XMVectorSet(2.0f, 2.0f, 2.0f, 1.0f);
	DirectX::XMVECTOR intensity = DirectX::XMVectorSet(100.0f, 100.0f, 100.0f, 100.0f);

	DirectX::XMStoreFloat4(&m_lightBufferData.position, position);
	DirectX::XMStoreFloat4(&m_lightBufferData.intensity, intensity);
	
}

//-----------------------------------------------------------------------------
// Create device-dependent resources for rendering.
//-----------------------------------------------------------------------------
void Renderer::CreateDeviceDependentResources()
{
	// Compile shaders using the Effects library.
	auto CreateShadersTask = Concurrency::create_task(
		[this]()
	{
		CreateShaders();
	}
	);

	// Load the geometry for the spinning cube.
	auto CreateCubeTask = CreateShadersTask.then(
		[this]()
	{
		CreateCube();
	}
	);

	// Load the texture for the cube.
	auto LoadTextureTask = CreateShadersTask.then(
		[this]()
	{
		LoadTexture();
	}
	);
}

void Renderer::CreateWindowSizeDependentResources()
{
	// Create the view matrix and the perspective matrix.
	CreateViewAndPerspective();
	CreateLightInfo();
}


//-----------------------------------------------------------------------------
// Update the scene.
//-----------------------------------------------------------------------------
void Renderer::Update(uint32_t colourFlag, uint32_t textureFlag, uint32_t pixelLightFlag, uint32_t vertexLightFlag)
{
	// Rotate the cube 1 degree per frame.
	DirectX::XMStoreFloat4x4(
		&m_constantBufferData.world,
		DirectX::XMMatrixTranspose(
			DirectX::XMMatrixRotationY(
				DirectX::XMConvertToRadians(
				(float)m_frameCount++
				)
			)
		)
	);

	if (m_frameCount == 360)  m_frameCount = 0;

	SetTexture(textureFlag);
	SetColour(colourFlag);
	SetPixelLight(pixelLightFlag);
	SetVertexLight(vertexLightFlag);

}

//-----------------------------------------------------------------------------
// Render the cube.
//-----------------------------------------------------------------------------
void Renderer::Render()
{
	// Use the Direct3D device context to draw.
	ID3D11DeviceContext* context = m_deviceResources->GetDeviceContext();

	ID3D11RenderTargetView* renderTarget = m_deviceResources->GetRenderTarget();
	ID3D11DepthStencilView* depthStencil = m_deviceResources->GetDepthStencil();

	context->UpdateSubresource(
		m_pConstantBuffer.Get(),
		0,
		nullptr,
		&m_constantBufferData,
		0,
		0
	);

	context->UpdateSubresource(
		m_pRenderTypeBuffer.Get(),
		0,
		nullptr,
		&m_renderTypeData,
		0,
		0
	);

	context->UpdateSubresource(
		m_pLightBuffer.Get(),
		0,
		nullptr,
		&m_lightBufferData,
		0,
		0
	);

	// Clear the render target and the z-buffer.
	const float teal[] = { 0.098f, 0.439f, 0.439f, 1.000f };
	context->ClearRenderTargetView(
		renderTarget,
		teal
	);
	context->ClearDepthStencilView(
		depthStencil,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	// Set the render target.
	context->OMSetRenderTargets(
		1,
		&renderTarget,
		depthStencil
	);

	// Set up the IA stage by setting the input topology and layout.
	UINT stride = sizeof(VertexPositionColorTexture);
	UINT offset = 0;

	context->IASetVertexBuffers(
		0,
		1,
		m_pVertexBuffer.GetAddressOf(),
		&stride,
		&offset
	);

	context->IASetIndexBuffer(
		m_pIndexBuffer.Get(),
		DXGI_FORMAT_R16_UINT,
		0
	);

	context->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	);

	context->IASetInputLayout(m_pInputLayout.Get());

	// Set up the vertex shader stage.
	context->VSSetShader(
		m_pVertexShader.Get(),
		nullptr,
		0
	);

	context->VSSetConstantBuffers(
		0,
		1,
		m_pConstantBuffer.GetAddressOf()
	);

	context->VSSetConstantBuffers(
		1,
		1,
		m_pRenderTypeBuffer.GetAddressOf()
	);

	context->VSSetConstantBuffers(
		2,
		1,
		m_pLightBuffer.GetAddressOf()
	);

	//context->VSSetShaderResources(0, 1, &m_pTextureView);

	// Set up the pixel shader stage.
	context->PSSetShader(
		m_pPixelShader.Get(),
		nullptr,
		0
	);

	context->PSSetConstantBuffers(
		0,
		1,
		m_pRenderTypeBuffer.GetAddressOf()
	);

	context->PSSetConstantBuffers(
		1,
		1,
		m_pLightBuffer.GetAddressOf()
	);

	context->PSSetShaderResources(0, 1, m_pTextureView.GetAddressOf());
	context->PSSetSamplers(0, 1, m_pSampler.GetAddressOf());

	// Calling Draw tells Direct3D to start sending commands to the graphics device.
	context->DrawIndexed(
		m_indexCount,
		0,
		0
	);
}

//-----------------------------------------------------------------------------
// Clean up cube resources when the Direct3D device is lost or destroyed.
//-----------------------------------------------------------------------------
Renderer::~Renderer()
{
	// ComPtr will clean up references for us. But be careful to release
	// references to anything you don't need whenever you call Flush or Trim.
	// As always, clean up your system (CPU) memory resources before exit.
}

