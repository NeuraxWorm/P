#ifndef INCLUDED_HUD_H
#define	INCLUDED_HUD_H

#include <SGE.h>
#include <vector>
using namespace SGE;
using namespace std;

class TData;
class Tower;
class Tilemap;
class Hud
{
public:
	enum class States
	{
		Normal,
		Build,
		Tower
	};
	
	Hud(vector<Tower*> &towers, Tilemap &map, int &hp);
	~Hud();

	void Load(const char *hudpack, const char *towerpack);
	void Unload();
	void Update(const float deltaTime);
	void UpdateCursor(const float deltaTime);
	void Render();

	void SetCredits(const int credits);
	int GetCredits() const;
	void AddCredits(const int credits);

	void SetSellMultiplier(const float multiplier);
	float GetSellMultiplier() const;

	void SetUpgradeMultiplier(const float multiplier);
	float GetUpgradeMultiplier() const;

private:
	NONCOPYABLE(Hud);

	void ToNormal();
	void BuildState(float deltaTime);
	void TowerState(float deltaTime);
	Tower *BuildTower();

	vector<Tower*> &mTowers;
	vector<TData> mTowerSets;
	vector<SGE_Button*> mTButtons;
	SGE_Sprite mBackground;
	SGE_Sprite mTSprite;
	SGE_Sprite mRSprite;
	SGE_Font mText;
	SGE_Button mTSell;
	SGE_Button mTUp;
	SGE_Cursor mCursor;
	string mStr;
	States mState;
	Tilemap &mMap;
	float mTimer;
	float mBonus;
	float mSMulti;
	float mSPrice;
	float mUMulti;
	float mUPrice;
	float mRScale;
	int mCredits;
	int mTCurrent;
	int &mPlayerHP;
	bool mPressed;
};

#endif