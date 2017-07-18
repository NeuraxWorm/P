// WARNING: Tilemap width hard-coded to 800

#include "Hud.h"
#include "SplashTower.h"
#include "CircularTower.h"
#include "Tilemap.h"

namespace
{
	// get selected tile
	SVector2 MouseTile(const int tSize, bool coord)
	{
		ASSERT(Input_GetMouseScreenX() < 800, "Position not inside tilemap");
		

		int x = Input_GetMouseScreenX();
		int y = Input_GetMouseScreenY();

		x /= tSize;
		y /= tSize;

		if (coord)
		{
			x *= tSize;
			y *= tSize;
			x += tSize / 2;
			y += tSize / 2;
		}

		return SVector2(x, y);
	}
}

Hud::Hud(vector<Tower*> &towers, Tilemap &map, int &hp) :
	mTowers(towers), mState(States::Normal), mMap(map), mCredits(0), 
	mTCurrent(-1), mTimer(0.0), mBonus(0.0f), mSMulti(0.5f), mSPrice(0.0f),
	mUMulti(1.5f), mUPrice(0.0f), mRScale(1.0f), mPlayerHP(hp)
{

}

Hud::~Hud()
{

}

void Hud::Load(const char *hudpack, const char *towerpack)
{
	FILE *file = nullptr;
	char sBuf1[32], sBuf2[32], sBuf3[32];
	float fBuf = 0.0f;
	
	mText.Load(24);
	mText.SetColor(0, 0, 0);

	mTSprite.SetAlpha(0.6f);

	// Load HUD pack
	errno_t error = fopen_s(&file, hudpack, "r");
	ASSERT(error == 0, "failed to load hud pack");
	
	// background
	fscanf_s(file, "%s ", sBuf1, 32);
	ASSERT(strcmp(sBuf1, "Background:") == 0, "Invalid file format (Background)");
	fscanf_s(file, "%s\n", sBuf2, 32);
	mBackground.SetPosition(800, 0);
	mBackground.Load(sBuf2);

	// Tower sell button
	fscanf_s(file, "%s ", sBuf1, 32);
	ASSERT(strcmp(sBuf1, "TButtonSell_on:") == 0, "Invalid file format (TButtonSell_on)");
	fscanf_s(file, "%s\n", sBuf2, 32);

	fscanf_s(file, "%s ", sBuf1, 32);
	ASSERT(strcmp(sBuf1, "TButtonSell_off:") == 0, "Invalid file format (TButtonSell_off)");
	fscanf_s(file, "%s\n", sBuf3, 32);

	fscanf_s(file, "%s ", sBuf1, 32);
	ASSERT(strcmp(sBuf1, "TButtonSell_size:") == 0, "Invalid file format (TButtonSell_size)");
	fscanf_s(file, "%f\n", &fBuf);
	ASSERT(fBuf > 0.0f, "Invalid font size (TButtonSell)")
	mTSell.Load(sBuf2, sBuf3, (int)fBuf);
	mTSell.SetPosition(868, 250);

	// Tower upgrade button
	fscanf_s(file, "%s ", sBuf1, 32);
	ASSERT(strcmp(sBuf1, "TButtonUp_on:") == 0, "Invalid file format (TButtonUp_on)");
	fscanf_s(file, "%s\n", sBuf2, 32);

	fscanf_s(file, "%s ", sBuf1, 32);
	ASSERT(strcmp(sBuf1, "TButtonUp_off:") == 0, "Invalid file format (TButtonUp_off)");
	fscanf_s(file, "%s\n", sBuf3, 32);

	fscanf_s(file, "%s ", sBuf1, 32);
	ASSERT(strcmp(sBuf1, "TButtonUp_size:") == 0, "Invalid file format (TButtonUp_size)");
	fscanf_s(file, "%f\n", &fBuf);
	ASSERT(fBuf > 0.0f, "Invalid font size (TButtonUp)")
	mTUp.Load(sBuf2, sBuf3, (int)fBuf);
	mTUp.SetPosition(868, 500);

	// Range Sprite
	fscanf_s(file, "%s ", sBuf1, 32);
	ASSERT(strcmp(sBuf1, "RangeSprite:") == 0, "Invalid file format (RangeSprite)");
	fscanf_s(file, "%s\n", sBuf2, 32);
	mRSprite.Load(sBuf2);

	// Range Sprite Scale
	fscanf_s(file, "%s ", sBuf1, 32);
	ASSERT(strcmp(sBuf1, "RangeScale:") == 0, "Invalid file format (RangeScale)");
	fscanf_s(file, "%f\n", &fBuf);
	mRScale = fBuf;

	// Cursor
	fscanf_s(file, "%s ", sBuf1, 32);
	ASSERT(strcmp(sBuf1, "Cursor:") == 0, "Invalid file format (Cursor)");
	fscanf_s(file, "%s", sBuf2, 32);
	mCursor.Load(sBuf2);

	fclose(file);

	// Load tower pack
	error = fopen_s(&file, towerpack, "r");
	ASSERT(error == 0, "failed to load tower pack");
	int size = 0;
	
	// Tower number
	fscanf_s(file, "%s ", sBuf1, 32);
	ASSERT(strcmp(sBuf1, "Towers:") == 0, "Invalid file format (Towers)");
	fscanf_s(file, "%d\n", &size);
	ASSERT(size > 0, "Invalid size value");

	mTowerSets.reserve(size);
	TData t;
	SGE_Button *tButton;
	for(int i = 0; i < size; ++i)
	{	
		// name
		fscanf_s(file, "%s ", sBuf1, 32);
		ASSERT(strcmp(sBuf1, "Name:") == 0, "Invalid file format (Name)");
		fscanf_s(file, "%s\n", sBuf3, 32);
		t.mName = sBuf3;

		// tower sprite
		fscanf_s(file, "%s ", sBuf1, 32);
		ASSERT(strcmp(sBuf1, "TurretSprite:") == 0, "Invalid file format (TurretSprite)");
		fscanf_s(file, "%s\n", sBuf2, 32);

		strcpy(t.mTTexture, sBuf2);

		// button
		tButton = new SGE_Button();
		tButton->Load(sBuf2, sBuf2, 10);
		tButton->SetPosition(868, 250 + i * 50);

		// firing sprite
		fscanf_s(file, "%s ", sBuf1, 32);
		ASSERT(strcmp(sBuf1, "FiringSprite:") == 0, "Invalid file format (FiringSprite)");
		fscanf_s(file, "%s\n", sBuf2, 32);

		strcpy(t.mFTexture, sBuf2);

		// muzzle distance
		fscanf_s(file, "%s ", sBuf1, 32);
		ASSERT(strcmp(sBuf1, "MuzzleDis:") == 0, "Invalid file format (MuzzleDis)");
		fscanf_s(file, "%f\n", &fBuf);
		t.mMuzzleDis = (int)fBuf;

		// type
		fscanf_s(file, "%s ", sBuf1, 32);
		ASSERT(strcmp(sBuf1, "Type:") == 0, "Invalid file format (Type)");
		fscanf_s(file, "%c\n", &sBuf2[0]);
		switch(sBuf2[0])
		{
		case 'c': t.mType = Weapon::Circular; break;
		case 'p': t.mType = Weapon::Point; break;
		case 's': t.mType = Weapon::Splash; break;
		default: ASSERT(false, "Invalid tower type");
		}

		// splash radius (for splash towers only)
		if (t.mType == Weapon::Splash)
		{
			fscanf_s(file, "%s ", sBuf1, 32);
			ASSERT(strcmp(sBuf1, "SplashRadius:") == 0, "Invalid file format (SplashRadius)");
			fscanf_s(file, "%f\n", &fBuf);
			ASSERT(fBuf > 0.0f, "Invalid splash radius value");
			t.mSplashRadius = fBuf;
		}
		
		// damage
		fscanf_s(file, "%s ", sBuf1, 32);
		ASSERT(strcmp(sBuf1, "Damage:") == 0, "Invalid file format (Damage)");
		fscanf_s(file, "%f\n", &fBuf);
		ASSERT(fBuf > 0.0f, "Invalid damage value");
		t.mDamage = (int)fBuf;
		
		// price
		fscanf_s(file, "%s ", sBuf1, 32);
		ASSERT(strcmp(sBuf1, "Price:") == 0, "Invalid file format (Price)");
		fscanf_s(file, "%f\n", &fBuf);
		ASSERT(fBuf > 0.0f, "Invalid price value");
		t.mPrice = (int)fBuf;
		
		// range
		fscanf_s(file, "%s ", sBuf1, 32);
		ASSERT(strcmp(sBuf1, "Range:") == 0, "Invalid file format (Range)");
		fscanf_s(file, "%f\n", &fBuf);
		ASSERT(fBuf > 0.0f, "Invalid range value");
		t.mRange = fBuf;
		
		// cooldown
		fscanf_s(file, "%s ", sBuf1, 32);
		ASSERT(strcmp(sBuf1, "CD:") == 0, "Invalid file format (CD)");
		fscanf_s(file, "%f\n", &fBuf);
		ASSERT(fBuf > 0.0f, "Invalid CD value");
		t.mCooldown = fBuf;
		
		mTowerSets.push_back(t);
		mTButtons.push_back(tButton);
	}
	fclose(file);

	mCredits = 600;
}


void Hud::Unload()
{
	mBackground.Unload();
	mText.Unload();
	for (auto b : mTButtons)
	{
		b->Unload();
		delete b;
	}
	mTSell.Unload();
	mTUp.Unload();
	mCursor.Unload();
}

void Hud::Update(float deltaTime)
{
	mPressed = false;

	mTimer += deltaTime;
	mBonus += deltaTime;
	
	// currency over time
	if (mBonus > 0.5f)
	{
		mCredits++;
		mBonus = 0.0f;
	}

	// back to normal state
	if (Input_IsMousePressed(Mouse::RBUTTON))
	{
		ToNormal();
		mPressed = true;
	}

	if (mState != States::Tower)
	{
		for (auto &b : mTButtons)
		{
			b->Update(deltaTime);
		}

		// panel
		if (Input_GetMouseScreenX() > 800)
		{
			if (!mPressed)
			{
				for (int i = 0; i < mTowerSets.size(); ++i)
				{
					if (mTButtons[i]->IsPressed())
					{
						// check tower price
						if (mCredits >= mTowerSets[i].mPrice)
						{
							mState = States::Build;
							// load tower sprite
							mTSprite.Load(mTowerSets[i].mTTexture);

							// re-scale range sprite
							float rSpriteMulti = mTowerSets[i].mRange * mRScale;
							mRSprite.SetScale(SVector2(rSpriteMulti, rSpriteMulti));

							mTCurrent = i;
							mCursor.Hide();
						}
						mPressed = true;
						break;
					}
				}
			}
		}
		else
		{
			// select tower
			if (mState != States::Build && 
				Input_IsMousePressed(Mouse::LBUTTON))
			{
				mPressed = true;
				for (int i = 0; i < mTowers.size(); ++i)
				{
					if (MouseTile(mMap.GetTileSize(), true) ==
						mTowers[i]->GetPosition())
					{
						mTCurrent = i;
						if (mState == States::Tower)
						{
							mSPrice = 0.0f;
							mUPrice = 0.0f;
						}
						else
						{
							mState = States::Tower;
						}
						break;
					}
				}
			}
		}
	}

	// run state specific codes
	switch (mState)
	{
	case States::Build:
		BuildState(deltaTime);
		break;
	case States::Tower:
		TowerState(deltaTime);
		break;
	default:
		break;
	}
	
	mCursor.Update(deltaTime);
}

void Hud::UpdateCursor(const float deltaTime)
{
	mCursor.Update(deltaTime);
}

// TODO: store values that don't change every frame in static variables to increase efficiency
void Hud::Render()
{
	// panel bg
	mBackground.Render();

	// panel buttons
	if (mState == States::Tower)
	{
		mTSell.Render();
		mTUp.Render();
	}
	else
	{
		for (auto b : mTButtons)
		{
			b->Render();
		}
	}

	// HUD sprites
	if (mState == States::Build && Input_GetMouseScreenX() < 800)
	{
		mTSprite.CRender();
		mRSprite.CRender();
	}
	else if (mState == States::Tower)
	{
		mRSprite.CRender();

		// tower attributes
		mStr = "damage: ";
		mStr += to_string(mTowers[mTCurrent]->GetDamage());
		mText.Print(mStr.c_str(), 820, 100);

		mStr = "type: ";
		const unsigned int type = typeid(*mTowers[mTCurrent]).hash_code();
		if (type == typeid(PointTower).hash_code())
		{
			mStr += "Point";
		}
		else if (type == typeid(CircularTower).hash_code())
		{
			mStr += "Circular";
		}
		else if(type == typeid(SplashTower).hash_code())
		{
			mStr += "Splash";
		}
		else
		{
			ASSERT(false, "Invalid tower type for selected tower.");
		}
		
		mText.Print(mStr.c_str(), 820, 120);
	}	

	// player stats
	mStr = "HP: ";
	mStr += to_string((int)mPlayerHP);
	mText.Print(mStr.c_str(), 820, 40);
	mStr = "time: ";
	mStr += to_string((int)mTimer);
	mText.Print(mStr.c_str(), 820, 60);
	mStr = "credits: ";
	mStr += to_string(mCredits);
	mText.Print(mStr.c_str(), 820, 80);
	
	// cursor
	mCursor.Render();
}

void Hud::SetCredits(const int credits)
{
	mCredits = credits;
}

int Hud::GetCredits() const
{
	return mCredits;
}

void Hud::AddCredits(const int credits)
{
	mCredits += credits;
}

void Hud::SetSellMultiplier(const float multiplier)
{
	mSMulti = multiplier;
}

float Hud::GetSellMultiplier() const
{
	return mSMulti;
}

void Hud::SetUpgradeMultiplier(const float multiplier)
{
	mUMulti = multiplier;
}

float Hud::GetUpgradeMultiplier() const
{
	return mUMulti;
}

void Hud::ToNormal()
{
	// return to normal state
	if (mState == States::Normal)
	{
		return;
	}
	mState = States::Normal;
	mTCurrent = -1;
	mSPrice = 0.0f;
	mUPrice = 0.0f;
	mCursor.Show();
	mTSprite.Unload();
}

void Hud::BuildState(float deltaTime)
{
	ASSERT(mTCurrent > -1, "Error: No tower selected.");

	if (Input_GetMouseScreenX() < 800)
	{
		mCursor.Hide();
		

		if (mPressed)
		{
			return;
		}

		SVector2 renderPos = MouseTile(mMap.GetTileSize(), true);
		mRSprite.SetPosition(renderPos);
		mTSprite.SetPosition(renderPos);

		if (Input_IsMousePressed(Mouse::LBUTTON))
		{
			Tower *t = BuildTower();
			if (t)
			{
				mTowers.push_back(t);
			}
			
			mPressed = true;
			// Lctrl: stay in state as long as cash is enough
			if (!t || !Input_IsKeyDown(Keys::LCONTROL) ||
				mCredits < mTowerSets[mTCurrent].mPrice)
			{
				ToNormal();
			}
		}
	}
	else
	{
		mCursor.Show();
	}
	// on the next click, if button selected, go back to first step
	// if empty ground selected, place tower & go back to normal cursor
	// if cancel button or esc clicked, go back to normal cursor
}

void Hud::TowerState(float deltaTime)
{
	ASSERT(mTCurrent > -1, "Error: No tower selected.");
	if (mSPrice == 0.0f)
	{
		// set prices
		ASSERT(mUPrice == 0.0f, "Error: mUPrice must be set to 0.");
		mSPrice = mTowers[mTCurrent]->GetPrice() * mSMulti;
		mStr = "Sell " + to_string((int)mSPrice);
		mTSell.SetText(mStr.c_str(), 0, 0, 0);

		mUPrice = mTowers[mTCurrent]->GetPrice() * mUMulti;
		mStr = "Upgrade " + to_string((int)mUPrice);
		mTUp.SetText(mStr.c_str(), 0, 0, 0);

		// set range sprite
		mRSprite.SetPosition(mTowers[mTCurrent]->GetPosition());
		float rSpriteMulti = mTowers[mTCurrent]->GetRange() * mRScale;
		mRSprite.SetScale(SVector2(rSpriteMulti, rSpriteMulti));
	}

	if (mPressed)
	{
		return;
	}

	if (Input_GetMouseScreenX() > 800)
	{
		if (mTSell.IsPressed())
		{
			mCredits += mSPrice;
			mTowers[mTCurrent]->Unload();
			mTowers.erase(mTowers.begin() + mTCurrent);
			mPressed = true;
			ToNormal();
		}
		else if (mTUp.IsPressed())
		{
			float priceP = mTowers[mTCurrent]->GetPrice();
			if (mCredits >= mUPrice)
			{
				mCredits -= mUPrice;
				mTowers[mTCurrent]->SetPrice(priceP + mUPrice);
				mTowers[mTCurrent]->SetDamage(mTowers[mTCurrent]->GetDamage() * (mUMulti + 1));
				ToNormal();
			}
			mPressed = true;
		}
		
	}
}

Tower *Hud::BuildTower()
{
	if (mState != States::Build)
	{
		return nullptr;
	}
	
	// check tile
	int tSize = mMap.GetTileSize();
	SVector2 coord = MouseTile(tSize, false);
	int tile = mMap.GetTile(coord.x, coord.y, Tilemap::Layer::Collision);
	if (tile != '-')
	{
		return nullptr;
	}
	
	// check if another tower is built on the tile
	coord *= tSize;
	coord.x += tSize / 2;
	coord.y += tSize / 2;
	for (auto t : mTowers)
	{
		if (t->GetPosition() == coord)
		{
			return nullptr;
		}
	}

	Tower *t;
	switch (mTowerSets[mTCurrent].mType)
	{
		case Weapon::Point:
			t = new PointTower;
			break;
		case Weapon::Splash:
			t = new SplashTower;
			break;
		case Weapon::Circular:
			t = new CircularTower;
			break;
	}

	t->Load(mTowerSets[mTCurrent]);
	t->Place(coord);
	mCredits -= mTowerSets[mTCurrent].mPrice;
	return t;
}