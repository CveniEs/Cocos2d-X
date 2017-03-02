#ifndef _ACTION_SPRITE_H_
#define _ACTION_SPRITE_H_

#include "cocos2d.h"
using namespace cocos2d;

#include "Defines.h"

class ActionSprite : public Sprite
{
public :
	ActionSprite();
	virtual bool init();

	void idle();
	void attack();
	void hurtWithDamage(float damage);
	void knockout();
	void walkWithDirection(Point direction);

	virtual void update(float dt);

//protected :
	Action* mIdleAction;
	Action* mAttackAction;
	Action* mWalkAction;
	Action* mHurtAction;
	Action* mKnockedOutAction;

	ActionState mActionState;

	float mWalkSpeed;
	float mHitPoints;
	float mDamage;

	Point mVelocity;
	Point mDesiredPosition;

	float mCenterToSide;
	float mCenterToBottom;

	BoundingBox mHitBox;
	BoundingBox mAttackBox;

	BoundingBox createBoundingBoxWithOrigin(Point origin, Size size);

	void transformBoxes();
	void setPosition(const Point& pos);
};

#endif
