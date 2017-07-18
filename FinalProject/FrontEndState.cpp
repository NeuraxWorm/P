#include "FrontEndState.h"

FrontEndState::FrontEndState(GameContext &context)
: GameState(context)
{}

FrontEndState::~FrontEndState()
{
}

void FrontEndState::Load()
{
	mBackground.Load("frontend_1024x768.png");

	mButtonLevel1.Load("s_button_on.png", "s_button_off.png");
	mButtonLevel1.SetText("Level 1", 100, 100, 100);
	mButtonLevel1.SetPosition(155.0f, 300.0f);

	mButtonLevel2.Load("s_button_on.png", "s_button_off.png");
	mButtonLevel2.SetText("Level 2", 100, 100, 100);
	mButtonLevel2.SetPosition(155.0f, 350.0f);

	mButtonQuit.Load("s_button_on.png", "s_button_off.png");
	mButtonQuit.SetText("Quit", 100, 100, 100);
	mButtonQuit.SetPosition(155.0f, 500.0f);

	mCursor.Load("sword.png");
}

void FrontEndState::Unload()
{
	mBackground.Unload();
	mButtonLevel1.Unload();
	mButtonLevel2.Unload();
	mButtonQuit.Unload();
	mCursor.Unload();
}

StateTransition FrontEndState::Update(float deltaTime)
{
	mCursor.Update(deltaTime);	
	mButtonLevel1.Update(deltaTime);
	mButtonLevel2.Update(deltaTime);
	mButtonQuit.Update(deltaTime);

	StateTransition transition = StateTransition::None;

	if(mButtonLevel1.IsPressed())
	{
		mContext.SetCurrentLevel(1);
		transition = StateTransition::GoToGameplay;
	}
	else if(mButtonLevel2.IsPressed())
	{
		mContext.SetCurrentLevel(2);
		transition = StateTransition::GoToGameplay;
	}
	else if(mButtonQuit.IsPressed() || Input_IsKeyPressed(Keys::ESCAPE))
	{

		transition = StateTransition::Quit;
	}

	return transition;
}

void FrontEndState::Render()
{
	mBackground.Render();
	mButtonLevel1.Render();
	mButtonLevel2.Render();
	mButtonQuit.Render();
	mCursor.Render();
	
}
