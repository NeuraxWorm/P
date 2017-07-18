#include "PlayState.h"
#include "PointTower.h"
#include "CircularTower.h"
#include "SplashTower.h"
#include "Enemy.h"

PlayState::PlayState(GameContext &context) :
	GameState(context), mHUD(mTowers, mMap, mPlayerHP)
{}

PlayState::~PlayState()
{
}

void PlayState::Load()
{
	// Loading level
	if (mContext.GetCurrentlLevel() == 1)
	{
		mMap.Load("level01.txt", "texturepack01.txt");
	}
	else if (mContext.GetCurrentlLevel() == 2)
	{
		mMap.Load("level02.txt", "texturepack02.txt");
	}

	mMap.GetWaypoints(mWaypoints);

	mHUD.Load("hudpack01.txt", "towerpack01.txt");

	mMessage.Load(72, false, false);

	// loading enemy wave (hardcoded)
	if(mContext.GetCurrentlLevel() == 1)
	{
		mEnemies.reserve(10);
		for(int i = 0; i < 10; ++i)
		{
			Enemy *enemy = new Enemy;
			enemy->Load("enemy.png");
			enemy->SetDamage(2);
			enemy->SetMaxHitpoint(60);
			enemy->SetSpeed(200.0f);
			enemy->SetBounty(30);
			mEnemies.push_back(enemy);
		}
	}
		
	else if(mContext.GetCurrentlLevel() == 2)
	{
		mEnemies.reserve(90);
		for (int i = 0; i < 90; ++i)
		{
			Enemy *enemy = new Enemy;
			enemy->Load("enemy.png");
			enemy->SetDamage(2);
			enemy->SetMaxHitpoint(10 + i * 5);
			enemy->SetSpeed(200.0f);
			enemy->SetBounty(60 + i * 3);
			mEnemies.push_back(enemy);
		}
	}
	

	mEnemies[0]->Spawn(mWaypoints[0], mWaypoints[1]);

	mPlayerHP = 10;

	mTimer = 0.0f;
	mOver = false;
}

void PlayState::Unload()
{
	mMap.Unload();
	mHUD.Unload();

	for(auto tower : mTowers)
	{
		tower->Unload();
		delete tower;
		tower = nullptr;
	}

	for(auto enemy : mEnemies)
	{
		enemy->Unload();
		delete enemy;
		enemy = nullptr;
	}
}

StateTransition PlayState::Update(float deltaTime)
{
	// game loop
	if(!mOver)
	{	
		mHUD.Update(deltaTime);
		mTimer += deltaTime;

		for(auto &tower : mTowers)
		{
			tower->Update(deltaTime);
		}

		int i;
		for(i = 0; i < mEnemies.size(); ++i)
		{
			// test spawner
			if(mTimer >(float)i && !mEnemies[i]->IsAlive())
			{
				mEnemies[i]->Spawn(mWaypoints[0], mWaypoints[1]);
			}

			if(mEnemies[i]->IsAlive())
			{
				bool reached = mEnemies[i]->Update(deltaTime, mWaypoints);
				if(reached)
				{
					mPlayerHP -= mEnemies[i]->DealDamage();
					if(mPlayerHP < 1) mOver = true;
				}
			} 
		}
		
		// attempt attacking enemies
		TowerAttack();
		
		for(i = 0; i < mEnemies.size(); ++i)
		{
			if(mEnemies[i]->GetHitpoint() > 0)
			{
				break;
			}
		}
		if(i == mEnemies.size())
		{
			mOver = true;
		}
	}
	else
	{
		mHUD.UpdateCursor(deltaTime);
	}

	// state change
	StateTransition transition = StateTransition::None;

	// quit game
	if(Input_IsKeyPressed(Keys::ESCAPE))
	{
		transition = StateTransition::GoToFrontEnd;
	}

	return transition;
}

void PlayState::Render()
{
	mMap.Render();	

	for(auto tower : mTowers)
	{
		if(tower->IsPlaced())
		{
			tower->Render();
		}
	}
	for(auto &enemy : mEnemies)
	{
		if(enemy->IsAlive())
		{
			enemy->Render();
		}
	}

	mHUD.Render();

	if (mOver)
	{
		mMessage.Print(mPlayerHP > 0 ? "You Win" : "You Lose", 300, 400);
		mMessage.Print("Press ESC", 270, 600);
	}
}

void PlayState::TowerAttack()
{
	bool kill = false;
	
	// check if enemy is in range
	for(auto tower : mTowers)
	{
		if(!tower->IsPlaced())
		{
			continue;
		}
		for(auto enemy : mEnemies)
		{
			if(!enemy->IsAlive())
			{
				continue;
			}

			if(Intersect(tower->GetAttackRadius(), enemy->GetHitCircle()))
			{
				// attack enemy one by one
				if(tower->Attack())
				{					
					if(typeid(*tower) != typeid(SplashTower))
					{
						kill = enemy->TakeHit(tower->GetDamage());
						if (kill)
						{
							mHUD.AddCredits(enemy->GetBounty());
						}
					}
					else
					{
						// circular AoE damage
						SCircle splash;
						splash.center = enemy->GetPosition();
						splash.radius = tower->GetSplashRadius();
						for (auto e : mEnemies)
						{                                                              
							if (!e->IsAlive())
							{
								continue;
							}

							if(Intersect(splash, e->GetHitCircle()))
							{
								kill = e->TakeHit(tower->GetDamage());
								if (kill)
								{
									mHUD.AddCredits(e->GetBounty());
								}
							}
						}
					}
				}

				// only aim and attack first enemy
				if(typeid(*tower) != typeid(CircularTower))
				{
					tower->SetTurretAngle(enemy->GetPosition());
					break;
				}
			}
		}
	}
}