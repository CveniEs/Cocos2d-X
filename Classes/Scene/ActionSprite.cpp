#include "ActionSprite.h"
#include "Defines.h"

ActionSprite::ActionSprite()
{
	Sprite::initWithSpriteFrameName("hero_idle_00.png");
	mHitBox.actual = Rect::ZERO;
	mHitBox.original = Rect::ZERO;
	mAttackBox.actual = Rect::ZERO;
	mAttackBox.original = Rect::ZERO;
	init();
}

bool ActionSprite::init()
{
	this->setTag(ACTION_SPRITE_TAG);
	Sprite::init();

	return true;
}

void ActionSprite::idle()
{
	if (mActionState != kActionStateIdle) {
		this->stopAllActions();
		this->runAction(mIdleAction);
		mActionState = kActionStateIdle;
        mVelocity = Point::ZERO;
	}
}

void ActionSprite::attack()
{
	if (mActionState == kActionStateIdle || 
		mActionState == kActionStateAttack || 
		mActionState == kActionStateWalk) {
		this->stopAllActions();
		this->runAction(mAttackAction);
		mActionState = kActionStateAttack;
	}
}

void ActionSprite::hurtWithDamage(float damage)
{
	if (mActionState != kActionStateKnockedOut) {
		this->stopAllActions();
		this->runAction(mHurtAction);
		mActionState = kActionStateHurt;
		mHitPoints -= damage;

		if (mHitPoints <= 0.0f) {
			this->knockout();
		}
	}
}

void ActionSprite::knockout()
{
	this->stopAllActions();
	this->runAction(mKnockedOutAction);
	mHitPoints = 0.0f;
	mActionState = kActionStateKnockedOut;
}

void ActionSprite::walkWithDirection(Point direction)
{
	if (mActionState == kActionStateIdle) {
		this->stopAllActions();
		this->runAction(mWalkAction);
		mActionState = kActionStateWalk;
	}
	if (mActionState == kActionStateWalk) {
		mVelocity = 
			Vec2(direction.x * mWalkSpeed, direction.y * mWalkSpeed);
	}
	if (mVelocity.x >= 0)
		//this->setScaleX(1.0f);
		this->setFlippedX(false);
	else
		//this->setScaleX(-1.0f);
		this->setFlippedX(true);
}

void ActionSprite::update(float dt)
{
	//std::cout << mVelocity.x << std::endl;
	if (mActionState == kActionStateWalk) {
		mDesiredPosition = ccpAdd(this->getPosition(), 
			ccpMult(mVelocity, dt));
	}

	Sprite::update(dt);
}

BoundingBox ActionSprite::createBoundingBoxWithOrigin(Point origin, Size size)
{
	BoundingBox bBox;
	bBox.original.origin = origin;
	bBox.original.size = size;
	bBox.actual.origin = ccpAdd(this->getPosition(), 
		Vec2(bBox.original.origin.x, bBox.original.origin.y));
	bBox.actual.size = size;

	return bBox;
}

void ActionSprite::transformBoxes()
{
	float xfactor = (this->isFlippedX() ? -1 : 1);
	float xfactor2 = (this->isFlippedX() ? 1 : 0);
	float yfactor = (this->isFlippedX() ? -1 : 1);
	float yfactor2 = (this->isFlippedX() ? 1 : 0);

	mHitBox.actual.origin = ccpAdd(this->getPosition(), 
		Vec2(mHitBox.original.origin.x * xfactor -
			mHitBox.original.size.width * xfactor2, 
		mHitBox.original.origin.y * yfactor - 
			mHitBox.original.size.height * yfactor2));
	mHitBox.actual.size = Size(
		mHitBox.original.size.width, 
		mHitBox.original.size.height);

	mAttackBox.actual.origin = ccpAdd(this->getPosition(), 
		Vec2(mAttackBox.original.origin.x * xfactor -
			mAttackBox.original.size.width * xfactor2, 
		mAttackBox.original.origin.y * yfactor - 
			mAttackBox.original.size.height * yfactor2));
	mAttackBox.actual.size = Size(
		mAttackBox.original.size.width, 
		mAttackBox.original.size.height);
}

void ActionSprite::setPosition(const Point& pos)
{
	Sprite::setPosition(pos);
	this->transformBoxes();
}
