#ifndef _MAIN_SCENE_H_
#define _MAIN_SCENE_H_

#include "cocos2d.h"

#include "Paper.h"

class MainScene : public cocos2d::Scene {
public:
	virtual bool init();
    
    void onTouchesBegan(const std::vector<cocos2d::Touch*> &t, cocos2d::Event *e);
    void onTouchesMoved(const std::vector<cocos2d::Touch*> &t, cocos2d::Event *e);
    void onTouchesEnded(const std::vector<cocos2d::Touch*> &t, cocos2d::Event *e);
    
    Paper * _paper;
    
    cocos2d::Vec2 startPoint, endPoint;
    
    cocos2d::Label *test;
    
    int buttonId = -1;
    
    int action = -1; // 0: lu, 1: ru, 2: ld, 3: rd
    
	CREATE_FUNC(MainScene);
};

#endif
