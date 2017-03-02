#ifndef _HUD_LAYER_H_
#define _HUD_LAYER_H_

#include "cocos2d.h"
using namespace cocos2d;

#include "SneakyButton.h"
#include "SneakyButtonSkinnedBase.h"
#include "SneakyJoystick.h"
#include "SneakyJoystickSkinnedBase.h"

class HudLayer : public Layer
{
public :
	HudLayer();
	bool init();

	SneakyButton* mButtonA;             // 攻击按钮
	SneakyJoystick* mJoystick;          // 移动按钮

	//void inputUpdate(float dt);
};

#endif
