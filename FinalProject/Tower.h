#ifndef INCLUDED_TOWER_H
#define	INCLUDED_TOWER_H

#include <SGE.h>
#include "Enemy.h"
using namespace SGE;
using namespace std;

enum class Weapon
{
	Point,
	Splash,
	Circular
};

struct TAttri
{
	string mName;
	int mDamage;
	int mPrice;
	float mMuzzleDis;
	float mSplashRadius;
	float mRange;
	float mCooldown;
};

struct TData : public TAttri
{
	Weapon mType;
	char mTTexture[32];
	char mFTexture[32];
};

class Tower
{
public:
	Tower();
	virtual ~Tower();
	void Load(const char *towerTexture, const char *projectileTexture, float muzzleDis = 16.0f);
	void Load(const TData &data);
	void Unload();
	void Update(const float deltaTime);
	virtual void Render() = 0;
	
	void Place(const int px, const int py, const int tileSize);
	void Place(const SVector2& coord);
	bool Attack();

	void SetPosition(const int px, const int py, const int tileSize);
	SVector2 GetPosition() const;
	
	void SetName(const char *name);
	void GetName(std::string &name) const;
	const char *GetName() const;
	
	bool IsPlaced() const;

	void SetDamage(const int damage);
	int GetDamage() const;

	void SetPrice(const int price);
	int GetPrice() const;

	void SetRange(const float range);
	float GetRange() const;
	SCircle GetAttackRadius() const;

	void SetSplashRadius(const float radius);
	float GetSplashRadius() const;

	void SetCooldown(const float cd);
	float GetCooldown() const;

	void SetTurretAngle(SVector2 targetPos);
	float GetTurretAngle() const;

protected:
	SGE_Sprite mTurretSprite;
	SGE_Sprite mFiringSprite;
	TAttri mAttri;
	SVector2 mPosition;
	float mCurrentCD;
	float mTurretAngle;
	float mAttackAnimation;
	float mAttackDuration;
	bool mPlaced;
};
#endif