#include "CircularTower.h"

CircularTower::CircularTower() {}

CircularTower::~CircularTower() {}

void CircularTower::Render()
{
	if (!mPlaced)
	{
		return;
	}
	
	// render turret
	const int tWidth = mTurretSprite.GetWidth();
	const int tHeight = mTurretSprite.GetHeight();
	SVector2 renderPos;
	renderPos.x = mPosition.x - (tWidth * 0.5f);
	renderPos.y = mPosition.y - (tHeight * 0.5f);

	mTurretSprite.SetPosition(renderPos);
	mTurretSprite.SetRotation(mTurretAngle);
	mTurretSprite.Render();
	mTurretSprite.SetRotation(0.0f);

	// render firing sprite once per strike
	if (mAttackAnimation > 0.0f)
	{
		const int bWidth = mFiringSprite.GetWidth();
		const int bHeight = mFiringSprite.GetHeight();
		renderPos.x = mPosition.x - (tWidth * 0.5f);
		renderPos.y = mPosition.y - (tHeight * 0.5f);

		mFiringSprite.SetPosition(renderPos);

		mFiringSprite.Render();
		mFiringSprite.SetRotation(0.0f);
	}
}