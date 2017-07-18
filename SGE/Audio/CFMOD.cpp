//====================================================================================================
// Filename:	CFMOD.cpp
// Created by:	Peter Chan
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Audio/CFMOD.h"

#include <assert.h>

#include "Core/CLog.h"

//====================================================================================================
// Statics
//====================================================================================================

CFMOD* CFMOD::s_pInstance = 0;

//====================================================================================================
// Class Definitions
//====================================================================================================

void CFMOD::Create()
{
	assert(s_pInstance == NULL);
	s_pInstance = new CFMOD();
}

//----------------------------------------------------------------------------------------------------

void CFMOD::Destroy()
{
	assert(s_pInstance != NULL);
	delete s_pInstance;
	s_pInstance = NULL;
}

//----------------------------------------------------------------------------------------------------

CFMOD* CFMOD::Get(void)
{
	assert(s_pInstance != NULL);
	return s_pInstance;
}

//----------------------------------------------------------------------------------------------------

CFMOD::CFMOD(void) :
	mpFMODSystem(0),
	mInitialized(false)
{
	// Empty
}

//----------------------------------------------------------------------------------------------------

CFMOD::~CFMOD(void)
{
	// Terminate the audio system
	Terminate();
}

//----------------------------------------------------------------------------------------------------

void CFMOD::Initialize(int iMaxChannels)
{
	// Check if we have already initialized the system
	if (mInitialized)
	{
		// Write to log
		CLog::Get()->Write(ELogMessageType_WARNING, "[FMOD] System already initialized.");
		return;
	}

	// Write to log
	CLog::Get()->Write(ELogMessageType_MESSAGE, "[FMOD] Initializing...");

	// Create FMOD system
	if (FMOD_OK != FMOD_System_Create(&mpFMODSystem))
	{
		// Write to log
		CLog::Get()->Write(ELogMessageType_ERROR, "[FMOD] Failed to create FMOD system.");
		return;
	}

	// Initialize FMOD system
	if (FMOD_OK != FMOD_System_Init(mpFMODSystem, iMaxChannels, FMOD_INIT_NORMAL, 0))
	{
		// Write to log
		CLog::Get()->Write(ELogMessageType_ERROR, "[FMOD] Failed to initialize FMOD system.");
		return;
	}

	// Set flag
	mInitialized = true;

	// Write to log
	CLog::Get()->Write(ELogMessageType_MESSAGE, "[FMOD] System initialized.");
}

//----------------------------------------------------------------------------------------------------

void CFMOD::Terminate(void)
{
	// Check if we have already terminated the manager
	if (!mInitialized)
	{
		// Write to log
		CLog::Get()->Write(ELogMessageType_WARNING, "[FMOD] System already terminated.");
		return;
	}

	// Write to log
	CLog::Get()->Write(ELogMessageType_MESSAGE, "[FMOD] Terminating...");

	// Release FMOD
	if (0 != mpFMODSystem)
	{
		FMOD_System_Release(mpFMODSystem);
		mpFMODSystem = 0;
	}

	// Set flag
	mInitialized = false;

	// Write to log
	CLog::Get()->Write(ELogMessageType_MESSAGE, "[FMOD] System terminated");
}

//----------------------------------------------------------------------------------------------------

void CFMOD::Update(void)
{
	// Check if we have already initialized the system
	if (!mInitialized)
	{
		// Write to log
		CLog::Get()->Write(ELogMessageType_WARNING, "[FMOD] Failed to update audio. System not initialized.");
		return;
	}

	// Update FMOD
	FMOD_System_Update(mpFMODSystem);
}

//----------------------------------------------------------------------------------------------------

FMOD_SYSTEM* CFMOD::FMODSystem(void) const
{
	// Check if we have already initialized the system
	if (!mInitialized)
	{
		// Write to log
		CLog::Get()->Write(ELogMessageType_WARNING, "[FMOD] Failed to access audio. System not initialized.");
		return 0;
	}

	return mpFMODSystem;
}