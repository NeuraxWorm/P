#ifndef INCLUDED_GAMESTATE_H
#define	INCLUDED_GAMESTATE_H

#include "GameContext.h"

enum StateTransition
{
	None,
	GoToFrontEnd,
	GoToGameplay,
	Quit
};

class GameState
{
public:
	GameState(GameContext &context);
	virtual ~GameState();

	virtual void Load() = 0;
	virtual void Unload() = 0;
	virtual StateTransition Update(float deltaTime) = 0;
	virtual void Render() = 0;
protected:
	GameContext &mContext;
};

#endif