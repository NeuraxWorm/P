#ifndef INCLUDED_SHADER_H
#define INCLUDED_SHADER_H

//====================================================================================================
// Filename:	CShader.h
// Created by:	Peter Chan
// Description:	Class for a Direct3D effect.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Resource/CResource.h"

#include <d3dx9.h>

//====================================================================================================
// Class Declarations
//====================================================================================================

class CShader : public CResource
{
public:
	// Constructor
	CShader(void);

	// Destructor
	virtual ~CShader(void);

	//==================================================
	// CResource Implementation
	//==================================================
	// Function to load/unload a new effect
	virtual void Load(const char* pFilename);
	virtual void Unload(void);
	//==================================================

	// Function to get the Direct3D effect interface
	ID3DXEffect* GetEffect(void) const;

private:
	ID3DXEffect* mpEffect;
};

#endif // #ifndef INCLUDED_TEXTURE_H