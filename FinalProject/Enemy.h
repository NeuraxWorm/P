#ifndef INCLUDED_ENEMY_H
#define	INCLUDED_ENEMY_H

#include <SGE.h>
#include <vector>
using namespace SGE;
using namespace std;

class Enemy
{
public:
	Enemy();
	//NONCOPYABLE(Enemy);
	~Enemy();
	void Load(const char *texture);
	void Unload();
	bool Update(const float deltaTime, const vector<SVector2> &waypoints);
	void Render();

	void Spawn(const SVector2 &spawnPos, const SVector2 &firstWP);
	bool TakeHit(const int damage);

	void SetPosition(const SVector2 &position);
	SVector2 GetPosition() const;

	bool IsAlive() const;

	void SetMaxHitpoint(const int hp);
	int GetMaxHitpoint() const;
	int GetHitpoint() const;

	void SetDamage(const int damage);
	int DealDamage();

	void SetBounty(const int bounty);
	int GetBounty() const;

	void SetSize(const float size);
	float GetSize() const;
	SCircle GetHitCircle() const;

	void SetSpeed(const float speed);
	float GetSpeed() const;

private:
	SGE_Sprite mSprite;
	SVector2 mPosition;
	SVector2 mDisplacement;
	bool mSpawned;
	int mHitpoint;
	int mMaxHP;
	int mDamage;
	int mStage;
	int mBounty;
	float mSize;
	float mSpeed;
	float mAngle;

	inline void SetDisplacement(const SVector2 &start, const SVector2 &end);
	void SetAngle(SVector2 direction);
};

#endif