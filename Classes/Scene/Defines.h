#ifndef _DEFINE_H_
#define _DEFINE_H_

#include "cocos2d.h"
using namespace cocos2d;

#define SCREEN (Director::getInstance()->getWinSize())
#define CENTER (Vec2(SCREEN.width/2, SCREEN.height/2))
//#define CURTIME CACurrentMediaTime()
//#define CURTIME 1
//#define CURTIME time(NULL)

inline int random_range(int low, int high)
{
	return ((rand() % (high - low + 1)) + low);
}

inline float frandom_range(float low, float high, float precision)
{
	float tmp = 1.0f / precision;
	return random_range(low*tmp, high*tmp) * precision;
}

inline float GetMAX(float x, float y)
{
	return (x > y ? x : y);
}

inline float GetMIN(float x, float y)
{
	return (x < y ? x : y);
}

enum ActionState {
	kActionStateNone = 0, kActionStateIdle, 
	kActionStateAttack, kActionStateWalk, 
	kActionStateHurt, kActionStateKnockedOut};

typedef struct _BoundingBox
{
	Rect actual;
	Rect original;
} BoundingBox;

#define ACTION_SPRITE_TAG 100

#endif
