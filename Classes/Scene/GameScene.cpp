#include "GameScene.h"
#include "Robot.h"

GameScene::GameScene()
{
	mHudLayer = new HudLayer();
    // 按钮放在视图的最上层
	this->addChild(mHudLayer, 1);
	mGameLayer = new GameLayer();
    // 地图背景放在视图的最底层
	this->addChild(mGameLayer, 0);

	init();

	this->schedule(schedule_selector(GameScene::inputUpdate));
}

bool GameScene::init()
{
	Scene::init();

	return true;
}

float attackInterval = -1.0f;

void GameScene::inputUpdate(float dt)
{
	Point velocity = mHudLayer->mJoystick->getVelocity();
	//std::cout << velocity.x << std::endl;

	if (velocity.x >= 0.4f || velocity.x <= -0.4f ||
		velocity.y >= 0.4f || velocity.y <= -0.4f) {
		mGameLayer->mHero->walkWithDirection(velocity);
	} else if (mGameLayer->mHero->mActionState == kActionStateWalk) {
		mGameLayer->mHero->idle();
	}

	if (attackInterval <= 0.0f) {
		if (mHudLayer->mButtonA->getIsActive()) {
			mGameLayer->mHero->attack();
			attackInterval = 0.5f;

			if (mGameLayer->mHero->mActionState == kActionStateAttack) {
                for(auto &item : mGameLayer->mRobots)
                {
                    Robot* robot = dynamic_cast<Robot*>(item);
                    if (robot->mActionState != kActionStateKnockedOut) {
                        if (fabs(mGameLayer->mHero->getPositionY() - robot->getPositionY()) < 10) {
                            //std::cout << "yew" << std::endl;
                            if (mGameLayer->mHero->mAttackBox.actual.intersectsRect(robot->mHitBox.actual)) {
                                robot->hurtWithDamage(mGameLayer->mHero->mDamage);
                                //std::cout << "yew!!!!!!!!!!!!" << std::endl;
                            }
                        }
                    }
                }
			}
		}
	} else {
		attackInterval -= dt;
	}
}
