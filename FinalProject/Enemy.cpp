#include "Enemy.h"

Enemy::Enemy()
: mSpawned(false), mHitpoint(0), mMaxHP(10), mDamage(1)
, mStage(1), mBounty(0), mSize(15.0f), mSpeed(300.0f)
{}

Enemy::~Enemy(){}

void Enemy::Load(const char *texture)
{
	mSprite.Load(texture);
}

void Enemy::Unload()
{
	mSprite.Unload();
}

bool Enemy::Update(const float deltaTime, const vector<SVector2> &waypoints)
{
	if(mHitpoint < 1 || !mSpawned) // better to check this at state side
	{
		return false;
	}
	SVector2 velocity = mDisplacement;
	velocity.Normalize();
	velocity *= mSpeed * deltaTime;

	mPosition += velocity;

	if((mPosition - waypoints[mStage - 1]).LengthSquared() >= mDisplacement.LengthSquared())
	{
		mPosition = waypoints[mStage];
		mStage++;
		if(mStage == waypoints.size())
		{
			return true;	// enemy reks ur home
		}

		SetDisplacement(waypoints[mStage- 1], waypoints[mStage]);
	}

	return false;	// u are not rekt... yet
}

void Enemy::Render()
{
	if(mHitpoint > 0 && mSpawned)
	{	
		const int kWidth = mSprite.GetWidth();
		const int kHeight = mSprite.GetHeight();
		
		SVector2 renderPosition;
		renderPosition.x = mPosition.x - (kWidth * 0.5f);
		renderPosition.y = mPosition.y - (kHeight * 0.5f);

		mSprite.SetPosition(renderPosition);
		mSprite.SetRotation(mAngle);
		mSprite.Render();
	}
}

void Enemy::Spawn(const SVector2 &spawnPos, const SVector2 &firstWP)
{
	if(mSpawned) return;
	mSpawned = true;
	mPosition = spawnPos;
	mStage = 1;
	mHitpoint = mMaxHP;

	SetDisplacement(spawnPos, firstWP);
}

bool Enemy::TakeHit(const int damage)
{
	mHitpoint -= damage;
	
	if(mHitpoint < 1)
	{
		// add some explosion and screaming here
		return true;
	}
	return false;
}


void Enemy::SetPosition(const SVector2 &position)
{
	mPosition = position;
}

SVector2 Enemy::GetPosition() const
{
	return mPosition;
}

bool Enemy::IsAlive() const
{
	return mHitpoint > 0 && mSpawned;
}

void Enemy::SetMaxHitpoint(const int hp)
{
	mMaxHP = hp;
}

int Enemy::GetMaxHitpoint() const
{
	return mMaxHP;
}

int Enemy::GetHitpoint() const
{
	return mHitpoint;
}

void Enemy::SetDamage(const int damage)
{
	mDamage = damage;
}

int Enemy::DealDamage()
{
	mHitpoint = 0;	// Game logic: suicide attack all the time
	return mDamage;
}

void Enemy::SetBounty(const int bounty)
{
	mBounty = bounty;
}

int Enemy::GetBounty() const
{
	return mBounty;
}

void Enemy::SetSize(const float size)
{
	mSize = size;
}

float Enemy::GetSize() const
{
	return mSize;
}

SCircle Enemy::GetHitCircle() const
{
	return SCircle(mPosition, mSize);
}

void Enemy::SetSpeed(const float speed)
{
	mSpeed = speed;
}

float Enemy::GetSpeed() const
{
	return mSpeed;
}

void Enemy::SetDisplacement(const SVector2 &start, const SVector2 &end)
{
	mDisplacement = end - start;
	SetAngle(mDisplacement);
}

void Enemy::SetAngle(SVector2 direction)
{
	direction.Normalize();	
	// -y = direction Dot upward vector(0, -1)
	mAngle = acos(-direction.y);

	if(direction.x < 0)
	{
		mAngle = -mAngle;
	}
}