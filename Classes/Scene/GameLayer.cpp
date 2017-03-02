#include "GameLayer.h"
#include "Defines.h"
#include "Robot.h"
#include "ActionSprite.h"
#include "GameScene.h"

GameLayer::GameLayer()
{
	initTileMap();
	
	SpriteFrameCache* cache =
		SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("pd_sprites.plist", "pd_sprites.png");

	/************************************************************************/
	/*                                                                      */
	/************************************************************************/

	Texture2D *texture = TextureCache::getInstance()->getTextureForKey("pd_sprites.png");
	mActors = SpriteBatchNode::createWithTexture(texture);
	mActors->getTexture()->setAliasTexParameters();
	this->addChild(mActors, 5);
	mHero = new Hero();

	//mActors->addChild(mHero);
	this->addChild(mHero);
	//std::cout << mActors->getChildrenCount() << std::endl;
	//this->schedule(schedule_selector(GameLayer::updatePosition));
	this->scheduleUpdate();

	initRobots();
	init();
}

bool GameLayer::init()
{
	Layer::init();

	//initTileMap();
	initHero();

	//this->setTouchEnabled(true);

	return true;
}

void GameLayer::initTileMap()
{
    // Size winSize = Director::getInstance()->getWinSize();
	mTileMap = TMXTiledMap::create("pd_tilemap.tmx");
    // mTileMap->setPosition(Vec2(0, winSize.height/2));

    for(auto &item : mTileMap->getChildren())
    {
        TMXLayer* tmxLayer = dynamic_cast<TMXLayer*>(item);
        tmxLayer->getTexture()->setAliasTexParameters();
    }
	this->addChild(mTileMap, -6);
}

void GameLayer::initHero()
{
	mHero->setPosition(Vec2(mHero->mCenterToSide, 50));
	mHero->mDesiredPosition = mHero->getPosition();
	mHero->idle();
}

void GameLayer::update(float dt)
{
	Layer::update(dt);
	updatePosition(dt);
	reorderActors();
	updateRobots(dt);
}

void GameLayer::updatePosition(float dt)
{
	float posX = GetMIN(mTileMap->getMapSize().width * mTileMap->getTileSize().width - mHero->mCenterToSide, 
		GetMAX(mHero->mCenterToSide, mHero->mDesiredPosition.x));
	float posY = GetMIN(3 * mTileMap->getTileSize().height + mHero->mCenterToBottom, 
		GetMAX(mHero->mCenterToBottom, mHero->mDesiredPosition.y));

	//std::cout << mHero->mDesiredPosition.x << std::endl;
	mHero->setPosition(Vec2(posX, posY));
    
    for(auto &item : mRobots)
    {
        Robot* robot = dynamic_cast<Robot*>(item);
        posX = GetMIN(mTileMap->getMapSize().width * mTileMap->getTileSize().width - robot->mCenterToSide,
                      GetMAX(robot->mCenterToSide, robot->mDesiredPosition.x));
        posY = GetMIN(3 * mTileMap->getTileSize().height + robot->mCenterToBottom,
                      GetMAX(robot->mCenterToBottom, robot->mDesiredPosition.y));
        
        robot->setPosition(Vec2(posX, posY));
    }

	this->setViewpointCenter(mHero->getPosition());
}

void GameLayer::setViewpointCenter(Point position)
{
	Size winSize = Director::getInstance()->getWinSize();

	int x = GetMAX(position.x, winSize.width / 2);
	int y = GetMAX(position.y, winSize.height / 2);

	x = GetMIN(x, (mTileMap->getMapSize().width * 
		mTileMap->getTileSize().width - winSize.width / 2));
	y = GetMIN(y, (mTileMap->getMapSize().height * 
		mTileMap->getTileSize().height - winSize.height / 2));

	Point actualPosition = Vec2(x, y);
	Point centerOfView = Vec2(winSize.width / 2, winSize.height / 2);
    Point viewPoint = Vec2(centerOfView.x - actualPosition.x, centerOfView.y - actualPosition.y);

	this->setPosition(viewPoint);
}

// 初始化机器人个数以及位置
void GameLayer::initRobots()
{
	int robotCount = 50;
//    mRobots = Vector<Ref *> array;

	for (int i = 0; i < robotCount; ++i) {
		Robot* robot = new Robot();
		//mActors->addChild(robot);
		this->addChild(robot);
		mRobots.pushBack(robot);

		int minX = SCREEN.width + robot->mCenterToSide;
		int maxX = mTileMap->getMapSize().width * mTileMap->getTileSize().width 
			- robot->mCenterToSide;
		int minY = robot->mCenterToBottom;
		int maxY = 3 * mTileMap->getTileSize().height + robot->mCenterToBottom;

		if (rand() % 2)
			robot->setFlippedX(true);
		robot->setPosition(
			Vec2(random_range(minX, maxX), random_range(minY, maxY)));
		robot->mDesiredPosition = robot->getPosition();
		robot->idle();
	}
}

void GameLayer::reorderActors()
{
    for(auto &item : mActors->getChildren())
    {
        Node *node = dynamic_cast<Node *>(item);
        if (node->getTag() == ACTION_SPRITE_TAG) {
            ActionSprite *sprite = dynamic_cast<ActionSprite *>(item);
            this->reorderChild(sprite, mTileMap->getMapSize().height * mTileMap->getTileSize().height - sprite->getPositionY());
        }
    }
}

void GameLayer::updateRobots(float dt)
{
	int alive = 0;
	double distanceSQ;
	int randomChoice = 0;

    for(auto &item : mRobots)
    {
        Robot* robot = dynamic_cast<Robot*>(item);
        robot->update(dt);
        
        if (robot->mActionState != kActionStateKnockedOut) {
            ++alive;
            robot->mNextDecisionTime -= dt;
            if (robot->mNextDecisionTime <= 0.0f) {
                distanceSQ = ccpDistanceSQ(robot->getPosition(), mHero->getPosition());
                if (distanceSQ <= 50*50) {
                    robot->mNextDecisionTime = frandom_range(0.5f, 1.0f, 0.1f);
                    randomChoice = random_range(0, 1);
                    if (randomChoice == 0) {
                        if (mHero->getPositionX() > robot->getPositionX()) {
                            robot->setFlippedX(false);
                        } else {
                            robot->setFlippedX(true);
                        }
                        
                        robot->attack();
                        if (robot->mActionState == kActionStateAttack) {
                            if (fabs(mHero->getPositionY() - robot->getPositionY()) < 10) {
                                if (mHero->mHitBox.actual.intersectsRect(robot->mAttackBox.actual)) {
                                    mHero->hurtWithDamage(robot->mDamage);
                                }
                            }
                        }
                    } else {
                        robot->idle();
                    }
                } else if (distanceSQ <= SCREEN.width * SCREEN.width) {
                    robot->mNextDecisionTime = frandom_range(1.0f, 1.5f, 0.1f);
                    robot->mNextDecisionTime = random_range(5, 10) * 0.1f;
                    randomChoice = random_range(0, 2);
                    if (randomChoice == 0) {
                        Point moveDirection = ccpNormalize(ccpSub(mHero->getPosition(), robot->getPosition()));
                        robot->walkWithDirection(moveDirection);
                    } else {
                        robot->idle();
                    }
                }
            }
        }
        else
        {
            // 清除已经死亡的机器人
        }
    }
}
