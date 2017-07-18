#include "Tower.h"

// default values
Tower::Tower():
mCurrentCD(0.0f), mTurretAngle(0.0f),
mAttackAnimation(0.0f), mAttackDuration(0.1f),
mPlaced(false)
{
	mAttri.mDamage = 1;
	mAttri.mPrice = 100;
	mAttri.mMuzzleDis = 15.0f;
	mAttri.mRange = 50.0f;
	mAttri.mSplashRadius = 0.0f;
	mAttri.mCooldown = 1.0f;
}

Tower::~Tower(){}

void Tower::Load(const char *towerTexture, const char *projectileTexture, float muzzleDis)
{
	mTurretSprite.Load(towerTexture);
	mFiringSprite.Load(projectileTexture);
	mAttri.mMuzzleDis = muzzleDis;
}

void Tower::Load(const TData &data)
{
	mAttri = data;
	mTurretSprite.Load(data.mTTexture);
	mFiringSprite.Load(data.mFTexture);
}

void Tower::Unload()
{
	mTurretSprite.Unload();
	mFiringSprite.Unload();
}

void Tower::Update(const float deltaTime)
{
	if (!mPlaced)
	{
		return;
	}

	mCurrentCD -= deltaTime;
	mAttackAnimation -= deltaTime;
}

void Tower::Place(const int px, const int py, const int tileSize)
{
	mPlaced = true;
	mPosition.x = (float)(px + 0.5f) * (float)tileSize;
	mPosition.y = (float)(py + 0.5f) * (float)tileSize;
}

void Tower::Place(const SVector2& coord)
{
	mPlaced = true;
	mPosition = coord;
}

/*
TODO:
Consider moving some codes to from PlayState::TowerAttack() to here.
Might require &PlayState::mEnemies as input.
*/
bool Tower::Attack()
{
	if (!mPlaced)
	{
		return false;
	}

	if(mCurrentCD <= 0.0f)
	{
		mAttackAnimation = mAttackDuration;
		mCurrentCD = mAttri.mCooldown;
		return true;
	}
	return false;
}

void Tower::SetPosition(const int px, const int py, const int tileSize)
{
	mPosition.x = (float)(px + 0.5f) * (float)tileSize;
	mPosition.y = (float)(py + 0.5f) * (float)tileSize;
}

SVector2 Tower::GetPosition() const
{
	return mPosition;
}

void Tower::SetName(const char *name)
{
	mAttri.mName = name;
}

void Tower::GetName(std::string &name) const
{
	name = mAttri.mName;
}

const char *Tower::GetName() const
{
	return mAttri.mName.c_str();
}

bool Tower::IsPlaced() const
{
	return mPlaced;
}

void Tower::SetDamage(const int damage)
{
	mAttri.mDamage = damage;
}

int Tower::GetDamage() const
{
	return mAttri.mDamage;
}

void Tower::SetPrice(const int price)
{
	mAttri.mPrice = price;
}

int Tower::GetPrice() const
{
	return mAttri.mPrice;
}

void Tower::SetRange(const float range)
{
	mAttri.mRange = range;
}

float Tower::GetRange() const
{
	return mAttri.mRange;
}

SCircle Tower::GetAttackRadius() const
{
	return SCircle(mPosition, mAttri.mRange);
}

void Tower::SetSplashRadius(const float radius)
{
	mAttri.mSplashRadius = radius;
}

float Tower::GetSplashRadius() const
{
	return mAttri.mSplashRadius;
}

void Tower::SetCooldown(const float cd)
{
	mAttri.mCooldown = cd;
}

float Tower::GetCooldown() const
{
	return mAttri.mCooldown;
}

void Tower::SetTurretAngle(SVector2 targetPos)
{
	targetPos -= mPosition;
	targetPos.Normalize();	
	// -y == targetPos Dot upward vector(0, -1)
	mTurretAngle = acos(-targetPos.y);

	if(targetPos.x < 0)
	{
		mTurretAngle = -mTurretAngle;
	}
}

float Tower::GetTurretAngle() const
{
	return mTurretAngle;
}