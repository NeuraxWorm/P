//====================================================================================================
// Filename:	CRenderSurface.cpp
// Created by:	Peter Chan
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "CRenderSurface.h"

#include "Core/CLog.h"
#include "Graphics/CDXGraphics.h"

//====================================================================================================
// Class Definitions
//====================================================================================================

CRenderSurface::CRenderSurface(void)
	: mpRenderTexture(NULL)
	, mpRenderSurface(NULL)
	, mpBackBuffer(NULL)
{
	// Empty
}

//----------------------------------------------------------------------------------------------------

CRenderSurface::~CRenderSurface(void)
{
	// Release everything
	Destroy();
}

//----------------------------------------------------------------------------------------------------

void CRenderSurface::Create(int iWidth, int iHeight)
{
	// Create a new render target
	HRESULT hr = CDXGraphics::Get()->D3DDevice()->CreateTexture
	(
		iWidth,					// Texture width
		iHeight,				// Texture height
		1,						// Mipmap level
		D3DUSAGE_RENDERTARGET,	// Usage
		D3DFMT_A8R8G8B8,		// Format
		D3DPOOL_DEFAULT,		// Memory pool
		&mpRenderTexture,		// Pointer to texture interface
		NULL
	);

	if (FAILED(hr))
	{
		// Write to log
		CLog::Get()->Write(ELogMessageType_ERROR, "[RenderSurface] Failed to create render surface.");
		return;
	}

	// Get the surface for the render target
	mpRenderTexture->GetSurfaceLevel(0, &mpRenderSurface);
}

//----------------------------------------------------------------------------------------------------

void CRenderSurface::Destroy(void)
{
	// Release everything
	if (NULL != mpBackBuffer)
	{
		mpBackBuffer->Release();
		mpBackBuffer = NULL;
	}
	if (NULL != mpRenderSurface)
	{
		mpRenderSurface->Release();
		mpRenderSurface = NULL;
	}
	if (NULL != mpRenderTexture)
	{
		mpRenderTexture->Release();
		mpRenderTexture = NULL;
	}
}

//----------------------------------------------------------------------------------------------------

void CRenderSurface::Bind()
{
	// Retain current render surface
	CDXGraphics::Get()->D3DDevice()->GetRenderTarget(0, &mpBackBuffer);

	// Set render target to texture
	CDXGraphics::Get()->D3DDevice()->SetRenderTarget(0, mpRenderSurface);
}

//----------------------------------------------------------------------------------------------------

void CRenderSurface::UnBind()
{
	// Restore original render surface
	CDXGraphics::Get()->D3DDevice()->SetRenderTarget(0, mpBackBuffer);
	mpBackBuffer->Release();
}

//----------------------------------------------------------------------------------------------------

IDirect3DTexture9* CRenderSurface::GetTexture(void) const
{
	return mpRenderTexture;
}