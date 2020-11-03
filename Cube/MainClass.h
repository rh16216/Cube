//// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Copyright (c) Microsoft Corporation. All rights reserved
#pragma once


//-----------------------------------------------------------------------------
// File: Cube.cpp
//
// Desktop app that renders a spinning, colorful cube.
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "DeviceResources.h"
#include "Renderer.h"

//-----------------------------------------------------------------------------
// Class declarations
//-----------------------------------------------------------------------------

class MainClass
{
public:
	MainClass();
	~MainClass();

	HRESULT CreateDesktopWindow();

	HWND GetWindowHandle() { return m_hWnd; };

	static LRESULT CALLBACK StaticWindowProc(
		HWND hWnd,
		UINT uMsg,
		WPARAM wParam,
		LPARAM lParam
	);

	HRESULT Run(
		std::shared_ptr<DeviceResources> deviceResources,
		std::shared_ptr<Renderer> renderer
	);

private:
	//-----------------------------------------------------------------------------
	// Desktop window resources
	//-----------------------------------------------------------------------------
	HMENU     m_hMenu;
	RECT      m_rc;
	HWND      m_hWnd;

};

// These are STATIC because this sample only creates one window.
// If your app can have multiple windows, MAKE SURE this is dealt with 
// differently.
static HINSTANCE m_hInstance;
static std::wstring m_windowClassName;

//Flags for updates based on user input
static uint32_t colourFlag;
static uint32_t textureFlag;
static uint32_t pixelLightFlag;
static uint32_t vertexLightFlag;