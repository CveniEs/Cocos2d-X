#include "Hero.h"

Hero::Hero()
{
	SpriteFrameCache* cache = SpriteFrameCache::getInstance();
	//CCSprite::initWithSpriteFrameName("hero_idle_00.png");
	Sprite::createWithSpriteFrameName("hero_idle_00.png");
	this->retain();
	
	char str[64] = {0};
    Vector<SpriteFrame *> animFrames;

	//Idle
	for (int i = 0; i < 6; ++i) {
		sprintf(str, "hero_idle_%02d.png", i);
		SpriteFrame* frame = cache->getSpriteFrameByName(str);
		animFrames.pushBack(frame);
	}
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	Animation* idleAnimation = Animation::createWithSpriteFrames(animFrames,1.0f/12.0f);
	mIdleAction = 
		RepeatForever::create(Animate::create(idleAnimation));
	mIdleAction->retain();

	//Attack
	animFrames.clear();
	for (int i = 0; i < 3; ++i) {
		sprintf(str, "hero_attack_00_%02d.png", i);
		SpriteFrame* frame = cache->getSpriteFrameByName(str);
		animFrames.pushBack(frame);
	}
	Animation* attackAnimation = Animation::createWithSpriteFrames(animFrames, 1.0f/24.0f);

	mAttackAction = Sequence::create(
		Animate::create(attackAnimation),
		CallFunc::create(CC_CALLBACK_0(Hero::idle, this)),NULL);
	mAttackAction->retain();

	//Walk
	animFrames.clear();
	for (int i = 0; i < 8; ++i) {
		sprintf(str, "hero_walk_%02d.png", i);
		SpriteFrame* frame = cache->getSpriteFrameByName(str);
		animFrames.pushBack(frame);
	}
	Animation* walkAnimation = Animation::createWithSpriteFrames(animFrames, 1.0f/12.0f);
	mWalkAction = RepeatForever::create(Animate::create(walkAnimation));
	mWalkAction->retain();

	//Hurt
	animFrames.clear();
	for (int i = 0; i < 3; ++i) {
		sprintf(str, "hero_hurt_%02d.png", i);
		SpriteFrame* frame = cache->getSpriteFrameByName(str);
		animFrames.pushBack(frame);
	}
	Animation* hurtAnimation = Animation::createWithSpriteFrames(animFrames, 1.0f/12.0f);
	mHurtAction = Sequence::create(
		Animate::create(hurtAnimation),
        CallFunc::create(CC_CALLBACK_0(Hero::idle, this)),NULL);
    mHurtAction->retain();

	//Knocked out
	animFrames.clear();
	for (int i = 0; i < 5; ++i) {
		sprintf(str, "hero_knockout_%02d.png", i);
		SpriteFrame* frame = cache->getSpriteFrameByName(str);
		animFrames.pushBack(frame);
	}
	Animation* knockedOutAnimation = Animation::createWithSpriteFrames(animFrames, 1.0f/12.0f);
	mKnockedOutAction = Sequence::create(
		Animate::create(knockedOutAnimation),
		Blink::create(2.0f, 10.0f),NULL);
	mKnockedOutAction->retain();

	mCenterToBottom = 39.0f;
	mCenterToSide = 29.0f;
	mHitPoints = 100.0f;
	mDamage = 20.0f;
	mWalkSpeed = 80.0f;

	mHitBox = createBoundingBoxWithOrigin(
		Vec2(-mCenterToSide, -mCenterToBottom),
		Size(mCenterToSide * 2, mCenterToBottom * 2));
	mAttackBox = createBoundingBoxWithOrigin(
		Vec2(mCenterToSide, -10), Size(20, 20));

	this->scheduleUpdate();

	init();
}

bool Hero::init()
{
	ActionSprite::init();

	return true;
}
