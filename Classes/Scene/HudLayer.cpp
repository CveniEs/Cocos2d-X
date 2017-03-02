#include "HudLayer.h"

HudLayer::HudLayer()
{
	SpriteFrameCache* cache =
		SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("UI.plist", "UI.png");

	mJoystick = NULL;
	mJoystick = new SneakyJoystick();
    mJoystick->initWithRect(Rect::ZERO);
	mJoystick->setAutoCenter(true);
	mJoystick->setHasDeadzone(true);
	mJoystick->setDeadRadius(10);
    
	SneakyJoystickSkinnedBase* jstickSkin = new SneakyJoystickSkinnedBase();
	jstickSkin->autorelease();
	jstickSkin->init();
	jstickSkin->setBackgroundSprite(Sprite::createWithSpriteFrameName("JoyStick-base.png"));
	jstickSkin->setThumbSprite(Sprite::createWithSpriteFrameName("JoyStick-thumb.png"));
	//jstickSkin->getThumbSprite()->setScale(0.5f);
	jstickSkin->setPosition(Vec2(50, 50));
	jstickSkin->setJoystick(mJoystick);
	this->addChild(jstickSkin);

	mButtonA = NULL;
	mButtonA = new SneakyButton();
    mButtonA->initWithRect(Rect::ZERO);
	mButtonA->setIsToggleable(false);
	mButtonA->setIsHoldable(true);
	SneakyButtonSkinnedBase* btnASkin = new SneakyButtonSkinnedBase();
	btnASkin->autorelease();
	btnASkin->init();
	btnASkin->setPosition(Vec2(430, 50));
	btnASkin->setDefaultSprite(Sprite::createWithSpriteFrameName("button-default.png"));
	btnASkin->setPressSprite(Sprite::createWithSpriteFrameName("button-pressed.png"));
	btnASkin->setActivatedSprite(Sprite::createWithSpriteFrameName("button-activated.png"));
	//btnASkin->setDisabledSprite(Sprite::createWithSpriteFrameName("button-disabled.png"));
	btnASkin->setButton(mButtonA);
	this->addChild(btnASkin);

	init();
}

bool HudLayer::init()
{
	Layer::init();
	return true;
}

/*void HudLayer::inputUpdate(float dt)
{
}*/
