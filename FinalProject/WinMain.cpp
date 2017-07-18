#include "FrontEndState.h"
#include "PlayState.h"

GameContext gameContext;
GameState *currentState = nullptr;
StateTransition transition = StateTransition::GoToFrontEnd;
bool quit = false;

void SGE_Initialize()
{
}

void SGE_Terminate()
{
}

bool SGE_Update(float deltaTime)
{
	if(transition != StateTransition::None)
	{
		if(currentState != nullptr)
		{
			currentState->Unload();
			delete currentState;
			currentState = nullptr;
		}


		switch(transition)
		{
		case StateTransition::GoToFrontEnd:
			currentState = new FrontEndState(gameContext);
			currentState->Load();
			break;
		case StateTransition::GoToGameplay:
			currentState = new PlayState(gameContext);
			currentState->Load();
			break;
		case StateTransition::Quit:
			quit = true;	
			break;
		}
	}

	if(currentState != nullptr)
	{
		transition = currentState->Update(deltaTime);
	}
	//if(Input_IsKeyPressed(Keys::ESCAPE))
	//{
	//	quit = true;
	//}
	return quit;
}

void SGE_Render()
{
	if(currentState != nullptr)
	{
		currentState->Render();
	}
}

//	Note: SGE requires DirectX SDK (June 2010) x86