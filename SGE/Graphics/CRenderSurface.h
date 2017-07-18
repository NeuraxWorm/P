#ifndef INCLUDED_RENDERSURFACE_H
#define INCLUDED_RENDERSURFACE_H

//====================================================================================================
// Filename:	CRenderSurface.h
// Created by:	Peter Chan
// Description:	Class for a render surface.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Resource/CResource.h"

#include <d3dx9.h>

//====================================================================================================
// Class Declarations
//====================================================================================================

class CRenderSurface
{
public:
	// Constructor
	CRenderSurface(void);

	// Destructor
	~CRenderSurface(void);
	
	// Function to create/destroy a render surface
	void Create(int iWidth, int iHeight);
	void Destroy();
	
	// Function to bind/unbind the render surface
	void Bind();
	void UnBind();

	// Function to get the Direct3D texture interface
	IDirect3DTexture9* GetTexture(void) const;

private:
	IDirect3DTexture9* mpRenderTexture;
	IDirect3DSurface9* mpRenderSurface;
	IDirect3DSurface9* mpBackBuffer;
};

#endif // #ifndef INCLUDED_RENDERSURFACE_H