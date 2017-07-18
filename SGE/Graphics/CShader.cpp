//====================================================================================================
// Filename:	CShader.cpp
// Created by:	Peter Chan
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "CShader.h"

#include "Core/CLog.h"
#include "Graphics/CDXGraphics.h"

//====================================================================================================
// Class Definitions
//====================================================================================================

CShader::CShader(void)
	: mpEffect(NULL)
{
	// Empty
}

//----------------------------------------------------------------------------------------------------

CShader::~CShader(void)
{
	// Release everything
	Unload();
}

//----------------------------------------------------------------------------------------------------

void CShader::Load(const char* pFilename)
{
	// Clear everything before we create the effect
	Unload();

	// Load effect
	ID3DXBuffer* pErrorBuffer = NULL;
	D3DXCreateEffectFromFileA
	(
		CDXGraphics::Get()->D3DDevice(),	// Direct3D device interface
		pFilename,							// Filename
		NULL,								// Macros
		NULL,								// Includes
		D3DXSHADER_DEBUG,					// Compile flags
		NULL,								// Effect pool
		&mpEffect,							// Pointer to effect interface
		&pErrorBuffer						// Error buffer
	);

	if (NULL != pErrorBuffer)
	{
		// Show error dialog
		MessageBoxA
		(
			NULL,
			(const char*)pErrorBuffer->GetBufferPointer(),
			"Shader Error",
			MB_OK | MB_ICONERROR
		);
		
		// Write to log
		CLog::Get()->Write(ELogMessageType_ERROR, "[Shader] Failed to compile shader %s", pFilename);

		pErrorBuffer->Release();
		return;
	}

	// Check if we have any errors
	if (NULL == mpEffect)
	{
		// Write to log
		CLog::Get()->Write(ELogMessageType_ERROR, "[Shader] Failed to create shader from file %s", pFilename);
		return;
	}
}

//----------------------------------------------------------------------------------------------------

void CShader::Unload(void)
{
	// Release everything
	if (NULL != mpEffect)
	{
		mpEffect->Release();
		mpEffect = NULL;
	}
}

//----------------------------------------------------------------------------------------------------

ID3DXEffect* CShader::GetEffect(void) const
{
	return mpEffect;
}