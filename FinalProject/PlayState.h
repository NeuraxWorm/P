#ifndef INCLUDED_PLAYSTATE_H
#define	INCLUDED_PLAYSTATE_H

#include <vector>
#include "GameState.h"
#include "Hud.h"
#include "Tilemap.h"

using namespace std;

class Tower;
class Enemy;

class PlayState : public GameState
{
public:
	PlayState(GameContext &context);
	virtual ~PlayState();

	virtual void Load();
	virtual void Unload();
	virtual StateTransition Update(float deltaTime);
	virtual void Render();

	void TowerAttack();
private:
	Tilemap mMap;
	Hud mHUD;
	SGE_Font mMessage;
	vector<Tower*> mTowers;
	vector<Enemy*> mEnemies;
	vector<SVector2> mWaypoints;
	float mTimer;
	int mPlayerHP;
	bool mOver;
};

#endif