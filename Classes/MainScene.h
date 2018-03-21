#ifndef _MAIN_SCENE_H_
#define _MAIN_SCENE_H_

#include "cocos2d.h"

#include "Paper.h"

class MainScene : public cocos2d::Scene {
public:
	CREATE_FUNC(MainScene);

	virtual bool init();
    
    void onTouchesBegan(const std::vector<cocos2d::Touch*> &t, cocos2d::Event *e);
    void onTouchesMoved(const std::vector<cocos2d::Touch*> &t, cocos2d::Event *e);
    void onTouchesEnded(const std::vector<cocos2d::Touch*> &t, cocos2d::Event *e);

public:
	Paper * _paper;

	cocos2d::Vec2 _startPoint = cocos2d::Vec2::ZERO, _endPoint = cocos2d::Vec2::ZERO;

	cocos2d::Label *test;

	int _touchId = -1;

};

#endif
