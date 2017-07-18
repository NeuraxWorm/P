#ifndef INCLUDED_FRONTENDSTATE_H
#define	INCLUDED_FRONTENDSTATE_H

#include "GameState.h"
#include <SGE.h>
using namespace SGE;

class FrontEndState : public GameState
{
public:
	FrontEndState(GameContext &context);
	virtual ~FrontEndState();

	virtual void Load();
	virtual void Unload();
	virtual StateTransition Update(float deltaTime);
	virtual void Render();

private:
	SGE_Sprite mBackground;
	SGE_Button mButtonLevel1;
	SGE_Button mButtonLevel2;
	SGE_Button mButtonQuit;
	SGE_Cursor mCursor;

};

#endif