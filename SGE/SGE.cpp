//====================================================================================================
// Filename:	SGE.cpp
// Created by:	Peter Chan
//====================================================================================================

//====================================================================================================
// Libraries
//====================================================================================================

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "../SGE/External/Lib/FMOD/fmodex_vc.lib")
#pragma comment (lib, "winmm.lib")

//====================================================================================================
// Includes
//====================================================================================================

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "Audio/Audio.h"
#include "Core/Core.h"
#include "Graphics/Graphics.h"
#include "Input/Input.h"
#include "Misc/Misc.h"
#include "Resource/Resource.h"
#include "Utility/Utility.h"

#include "SGE.h"

//====================================================================================================
// Externs
//====================================================================================================

extern void SGE_Initialize(void);
extern void SGE_Terminate(void);
extern bool SGE_Update(float fSeconds);
extern void SGE_Render(void);

//====================================================================================================
// Local Definitions
//====================================================================================================

namespace
{
	ID3DXEffect* spEffect = NULL;
}

//====================================================================================================
// Class Definitions
//====================================================================================================

class CGame : public CApplication
{
public:
	// Constructor
	CGame()
		: mDebug(CIniFile::Get()->GetBool("Debug", false))
		, mFPS(CIniFile::Get()->GetBool("FPS", false))
		, mPostProcessing(CIniFile::Get()->GetBool("PostProcessing", false))
	{}

protected:
	// Game initialization logic
	virtual void OnInitialize(void)
	{
		const int iWinWidth = CIniFile::Get()->GetInt("WinWidth", 800);
		const int iWinHeight = CIniFile::Get()->GetInt("WinHeight", 600);
		const int iResourceLimit = CIniFile::Get()->GetInt("ResourceLimit", 50000);
		const bool bFullscreen = CIniFile::Get()->GetBool("Fullscreen", false);

		// Initialize singletons
		CTimer::Create();
		CDXGraphics::Create();
		CSpriteRenderer::Create();
		CDXInput::Create();
		CResourceManager::Create();
		CFMOD::Create();

		// Initialize all engine systems
		CTimer::Get()->Initialize();
		CDXGraphics::Get()->Initialize(mWindow, iWinWidth, iWinHeight, bFullscreen);
		CSpriteRenderer::Get()->Initialize();
		CDXInput::Get()->Initialize(mWindow);
		CResourceManager::Get()->Initialize(iResourceLimit);
		CFMOD::Get()->Initialize(100);
		Random::SetRandSeed();

		// Debug initialization
		if (mDebug)
		{
			Debug::Initialize();
		}

		// Post processing initialization
		if (mPostProcessing)
		{
			mRenderSurface.Create(iWinWidth, iWinHeight);
			mPostProcessShader.Load("postprocess.fx");
			spEffect = mPostProcessShader.GetEffect();
		}

		// Create a new font
		mFont.Create(FontType::GARAMOND);
		mFont.SetColor(255, 255, 0);

		// User init logic
		SGE_Initialize();
	}

	// Game termination logic
	virtual void OnTerminate(void)
	{
		// User clean up logic
		SGE_Terminate();

		// Destroy the font
		mFont.Destroy();
		
		// Post processing termination
		if (mPostProcessing)
		{
			spEffect = NULL;
			mPostProcessShader.Unload();
			mRenderSurface.Destroy();
		}

		// Debug termination
		if (mDebug)
		{
			Debug::Terminate();
		}

		// Shutdown all engine systems
		CFMOD::Get()->Terminate();
		CResourceManager::Get()->Terminate();
		CDXInput::Get()->Terminate();
		CSpriteRenderer::Get()->Terminate();
		CDXGraphics::Get()->Terminate();
		CTimer::Get()->Terminate();

		// Destroy all singletons
		CFMOD::Destroy();
		CResourceManager::Destroy();
		CDXInput::Destroy();
		CSpriteRenderer::Destroy();
		CDXGraphics::Destroy();
		CTimer::Destroy();
	}

	// Game execution logic
	virtual void OnMainLoop(void)
	{
		// Update timer
		CTimer::Get()->Update();
		float fSeconds = CTimer::Get()->GetElapsedSeconds();
		if (fSeconds > 0.0333f)
		{
			fSeconds = 0.0333f;
		}

		// Update input
		CDXInput::Get()->Update();

		// Update sound
		CFMOD::Get()->Update();

		// User update logic
		if (SGE_Update(fSeconds))
		{
			Quit();
			return;
		}

		// Check if we are using post processing
		if (mPostProcessing)
		{
			mRenderSurface.Bind();
		}

		// Begin scene
		CDXGraphics::Get()->BeginRender();

		// Begin sprite rendering
		CSpriteRenderer::Get()->BeginRender();

		// User render logic
		SGE_Render();

		// Render FPS
		if (mFPS)
		{
			char temp[256];
			sprintf_s(temp, "FPS: %f", CTimer::Get()->GetFPS());
			mFont.PrintText(temp, 0, 0);
		}

		// End sprite rendering
		CSpriteRenderer::Get()->EndRender();

		// Debug render
		if (mDebug)
		{
			Debug::Render();
		}

		// End scene
		CDXGraphics::Get()->EndRender(!mPostProcessing);
		
		// If we are using post processing, unbind the render surface
		if (mPostProcessing)
		{
			mRenderSurface.UnBind();
		}

		// Render second pass if we are using post processing
		if (NULL != spEffect)
		{
			// Begin scene
			CDXGraphics::Get()->BeginRender();

			// Set technique and texture
			spEffect->SetTechnique("PostProcess");
			spEffect->SetTexture("Texture", mRenderSurface.GetTexture());
			
			// Draw a screen sized quad
			struct Vertex
			{
				float x, y, z, w, u, v;
			};
			Vertex vertices[] =
			{
				{ -0.5f,             -0.5f,              1.0f, 1.0f, 0.0f, 0.0f },
				{ -0.5f + mWinWidth, -0.5f,              1.0f, 1.0f, 1.0f, 0.0f },
				{ -0.5f,             -0.5f + mWinHeight, 1.0f, 1.0f, 0.0f, 1.0f },
				{ -0.5f + mWinWidth, -0.5f + mWinHeight, 1.0f, 1.0f, 1.0f, 1.0f }
			};
			spEffect->Begin(0, 0);
			spEffect->BeginPass(0);
			CDXGraphics::Get()->D3DDevice()->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);
			CDXGraphics::Get()->D3DDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, (const void*)&vertices[0], sizeof(Vertex));
			spEffect->EndPass();
			spEffect->End();

			// Clear technique and texture
			spEffect->SetTexture("Texture", NULL);
			spEffect->SetTechnique(NULL);

			// End scene
			CDXGraphics::Get()->EndRender(true);
		}
	}

private:
	IDirect3DVertexBuffer9* mpVB;

	CFont mFont;
	CRenderSurface mRenderSurface;
	CShader mPostProcessShader;

	bool mDebug;
	bool mFPS;
	bool mPostProcessing;
};

//====================================================================================================
// Main
//====================================================================================================

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
	// Load init file
	CIniFile::Create();
	CIniFile::Get()->Load("sge.ini");

	// Check if we are showing the splash screen
	const bool bSplashScreen = CIniFile::Get()->GetBool("SplashScreen", false);
	if (bSplashScreen)
	{
		// Splash texture name
		std::string fullname;
		fullname += CIniFile::Get()->GetString("TexturePath", "../Data/Textures/");
		fullname += "splashscreen.bmp";

		// Splash screen
		CSplashScreen splash;
		splash.Initialize(fullname.c_str());
		splash.Show(2000);
		splash.Terminate();
	}

	// Get initialization parameters
	const char* pAppName = CIniFile::Get()->GetString("AppName", "SGE");
	const int iWinWidth = CIniFile::Get()->GetInt("WinWidth", 800);
	const int iWinHeight = CIniFile::Get()->GetInt("WinHeight", 600);

	// Initialize and run the game
	CGame game;
	game.Initialize(pAppName, hInstance, iWinWidth, iWinHeight);
	game.Run();
	game.Terminate();

	// Unload init file
	CIniFile::Get()->Unload();
	CIniFile::Destroy();

	// Dump any memory leaks
	_CrtDumpMemoryLeaks();

	return 0;
}

//====================================================================================================
// Namespace
//====================================================================================================

namespace SGE
{

//====================================================================================================
// SGE Functions
//====================================================================================================

void LogMessage(const char* msg, ...)
{
	va_list args;
	va_start(args, msg);
	CLog::Get()->Write(ELogMessageType_MESSAGE, msg, args);
	va_end(args);
}

//----------------------------------------------------------------------------------------------------

void LogWarning(const char* msg, ...)
{
	va_list args;
	va_start(args, msg);
	CLog::Get()->Write(ELogMessageType_WARNING, msg, args);
	va_end(args);
}

//----------------------------------------------------------------------------------------------------

void LogError(const char* msg, ...)
{
	va_list args;
	va_start(args, msg);
	CLog::Get()->Write(ELogMessageType_ERROR, msg, args);
	va_end(args);
}

//----------------------------------------------------------------------------------------------------

int IniFile_GetInt(const char* pTag, int iDefault)
{
	return CIniFile::Get()->GetInt(pTag, iDefault);
}

//----------------------------------------------------------------------------------------------------

bool IniFile_GetBool(const char* pTag, bool bDefault)
{
	return CIniFile::Get()->GetBool(pTag, bDefault);
}

//----------------------------------------------------------------------------------------------------

float IniFile_GetFloat(const char* pTag, float fDefault)
{
	return CIniFile::Get()->GetFloat(pTag, fDefault);
}

//----------------------------------------------------------------------------------------------------

const char* IniFile_GetString(const char* pTag, const char* pDefault)
{
	return CIniFile::Get()->GetString(pTag, pDefault);
}

//----------------------------------------------------------------------------------------------------

int RandomInt(int iLower, int iUpper)
{
	return Random::GetInt(iLower, iUpper);
}

//----------------------------------------------------------------------------------------------------

float RandomFloat(float fLower, float fUpper)
{
	return Random::GetFloat(fLower, fUpper);
}

//----------------------------------------------------------------------------------------------------

SVector2 RandomVector2(const SVector2& vLower, const SVector2& vUpper)
{
	D3DXVECTOR2 lower(vLower.x, vLower.y);
	D3DXVECTOR2 upper(vUpper.x, vUpper.y);
	D3DXVECTOR2 vec(Random::GetVector2(lower, upper));

	return SVector2(vec.x, vec.y);
}

//----------------------------------------------------------------------------------------------------

void Graphics_DebugPoint(const SVector2& vPosition, unsigned int rgb)
{
	Debug::AddScreenPoint(vPosition.x, vPosition.y, rgb);
}

//----------------------------------------------------------------------------------------------------

void Graphics_DebugLine(const SVector2& vStart, const SVector2& vEnd, unsigned int rgb)
{
	Debug::AddScreenLine(vStart.x, vStart.y, vEnd.x, vEnd.y, rgb);
}

//----------------------------------------------------------------------------------------------------

void Graphics_DebugLine(const SLineSegment& rLineSegment, unsigned int rgb)
{
	Debug::AddScreenLine(rLineSegment.from.x, rLineSegment.from.y, rLineSegment.to.x, rLineSegment.to.y, rgb);
}

//----------------------------------------------------------------------------------------------------

void Graphics_DebugRect(const SVector2& vPosition, const SVector2& vDimension, unsigned int rgb)
{
	float x = vPosition.x;
	float y = vPosition.y;
	float w = vDimension.x;
	float h = vDimension.y;
	Debug::AddScreenRect(x, y, w, h, rgb);
}

//----------------------------------------------------------------------------------------------------

void Graphics_DebugRect(const SRect& rRect, unsigned int rgb)
{
	float x = rRect.min.x;
	float y = rRect.min.y;
	float w = rRect.max.x - rRect.min.x;
	float h = rRect.max.y - rRect.min.y;
	Debug::AddScreenRect(x, y, w, h, rgb);
}

//----------------------------------------------------------------------------------------------------

void Graphics_DebugCircle(const SVector2& vCenter, float fRadius, unsigned int rgb)
{
	Debug::AddScreenCircle(vCenter.x, vCenter.y, fRadius, rgb);
}

//----------------------------------------------------------------------------------------------------

void Graphics_DebugCircle(const SCircle& rCircle, unsigned int rgb)
{
	Debug::AddScreenCircle(rCircle.center.x, rCircle.center.y, rCircle.radius, rgb);
}

//----------------------------------------------------------------------------------------------------

void Graphics_DebugText(const char* pText, const SVector2& vPosition, unsigned int rgb)
{
	Debug::AddScreenText(pText, vPosition.x, vPosition.y, rgb);
}

//----------------------------------------------------------------------------------------------------

void Graphics_DebugText(const char* pText, float x, float y, unsigned int rgb)
{
	Debug::AddScreenText(pText, x, y, rgb);
}

//----------------------------------------------------------------------------------------------------

bool Input_IsKeyDown(int iKey)
{
	return CDXInput::Get()->IsKeyDown(iKey);
}

//----------------------------------------------------------------------------------------------------

bool Input_IsKeyPressed(int iKey)
{
	return CDXInput::Get()->IsKeyPressed(iKey);
}

//----------------------------------------------------------------------------------------------------

bool Input_IsMouseDown(int iMouse)
{
	return CDXInput::Get()->IsMouseDown(iMouse);
}

//----------------------------------------------------------------------------------------------------

bool Input_IsMousePressed(int iMouse)
{
	return CDXInput::Get()->IsMousePressed(iMouse);
}

//----------------------------------------------------------------------------------------------------

int Input_GetMouseScreenX(void)
{
	return CDXInput::Get()->GetMouseScreenX();
}

//----------------------------------------------------------------------------------------------------

int Input_GetMouseScreenY(void)
{
	return CDXInput::Get()->GetMouseScreenY();
}

//----------------------------------------------------------------------------------------------------

int Input_GetMouseMoveX(void)
{
	return CDXInput::Get()->GetMouseMoveX();
}

//----------------------------------------------------------------------------------------------------

int Input_GetMouseMoveY(void)
{
	return CDXInput::Get()->GetMouseMoveY();
}

//----------------------------------------------------------------------------------------------------

int Input_GetMouseMoveZ(void)
{
	return CDXInput::Get()->GetMouseMoveZ();
}

//----------------------------------------------------------------------------------------------------

bool Input_IsGamePadButtonDown(int iButton)
{
	return CDXInput::Get()->IsGamePadButtonDown(iButton);
}

//----------------------------------------------------------------------------------------------------

bool Input_IsGamePadButtonPressed(int iButton)
{
	return CDXInput::Get()->IsGamePadButtonPressed(iButton);
}

//----------------------------------------------------------------------------------------------------

bool Input_IsDPadUp(void)
{
	return CDXInput::Get()->IsDPadUp();
}

//----------------------------------------------------------------------------------------------------

bool Input_IsDPadDown(void)
{
	return CDXInput::Get()->IsDPadDown();
}

//----------------------------------------------------------------------------------------------------

bool Input_IsDPadLeft(void)
{
	return CDXInput::Get()->IsDPadLeft();
}

//----------------------------------------------------------------------------------------------------

bool Input_IsDPadRight(void)
{
	return CDXInput::Get()->IsDPadRight();
}

//----------------------------------------------------------------------------------------------------

float Input_GetLeftAnalogX(void)
{
	return CDXInput::Get()->GetLeftAnalogX();
}

//----------------------------------------------------------------------------------------------------

float Input_GetLeftAnalogY(void)
{
	return CDXInput::Get()->GetLeftAnalogY();
}

//----------------------------------------------------------------------------------------------------

float Input_GetRightAnalogX(void)
{
	return CDXInput::Get()->GetRightAnalogX();
}

//----------------------------------------------------------------------------------------------------

float Input_GetRightAnalogY(void)
{
	return CDXInput::Get()->GetRightAnalogY();
}

//----------------------------------------------------------------------------------------------------

void Shader_SetInt(const char* pName, int iValue)
{
	spEffect->SetInt(pName, iValue);
}

//----------------------------------------------------------------------------------------------------

void Shader_SetIntArray(const char* pName, const int* iValues, int iCount)
{
	spEffect->SetIntArray(pName, iValues, iCount);
}

//----------------------------------------------------------------------------------------------------

void Shader_SetFloat(const char* pName, float fValue)
{
	spEffect->SetFloat(pName, fValue);
}

//----------------------------------------------------------------------------------------------------

void Shader_SetFloatArray(const char* pName, const float* fValues, int iCount)
{
	spEffect->SetFloatArray(pName, fValues, iCount);
}

//----------------------------------------------------------------------------------------------------

void Shader_SetBool(const char* pName, BOOL bValue)
{
	spEffect->SetBool(pName, bValue);
}

//----------------------------------------------------------------------------------------------------

void Shader_SetBoolArray(const char* pName, const BOOL* bValues, int iCount)
{
	spEffect->SetBoolArray(pName, bValues, iCount);
}

//----------------------------------------------------------------------------------------------------

void Shader_SetVector(const char* pName, const D3DXVECTOR4& vector)
{
	spEffect->SetVector(pName, &vector);
}

//----------------------------------------------------------------------------------------------------

void Shader_SetVectorArray(const char* pName, const D3DXVECTOR4* vectors, int iCount)
{
	spEffect->SetVectorArray(pName, vectors, iCount);
}

} // namespace SGE