#ifndef INCLUDED_GAMECONTEXT_H
#define	INCLUDED_GAMECONTEXT_H


class GameContext
{
public:
	GameContext();
	~GameContext();

	void SetCurrentLevel(const int level)	{mCurrentLevel = level;}
	int GetCurrentlLevel() const			{return mCurrentLevel;}

private:

	int mCurrentLevel;
};

#endif