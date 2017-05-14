#ifndef _MAIN_SCENE_H_
#define _MAIN_SCENE_H_

#include "cocos2d.h"

#include "CustomPolygon.h"

#include <vector>

class MainScene : public cocos2d::Layer {
public:
	static cocos2d::Scene *createScene() {
		auto scene = cocos2d::Scene::create();
		auto layer = MainScene::create();

		scene->addChild(layer);
		return scene;
	}

	virtual bool init();

	std::vector<cocos2d::Vec2> clipLine(const cocos2d::Vec2 &p1, const cocos2d::Vec2 &p2, std::vector<cocos2d::Vec2> vertices);
    void findIntersection(const cocos2d::Vec2 &p1, const cocos2d::Vec2 &p2, const cocos2d::Vec2 &p3, const cocos2d::Vec2 &p4, bool &lineIntersect, bool &segmentIntersect, cocos2d::Vec2 &intersection, cocos2d::Vec2 &closeP1, cocos2d::Vec2 &closeP2);
    
    void onTouchesBegan(const std::vector<cocos2d::Touch*> &t, cocos2d::Event *e);
    void onTouchesMoved(const std::vector<cocos2d::Touch*> &t, cocos2d::Event *e);
    void onTouchesEnded(const std::vector<cocos2d::Touch*> &t, cocos2d::Event *e);
    
    cocos2d::Vec2 startPoint, endPoint;
    
    cocos2d::Label *test;
    
    std::vector<CustomPolygon*> papers;
    
    int buttonId = -1;
    
    int action = -1; // 0: lu, 1: ru, 2: ld, 3: rd
    
	CREATE_FUNC(MainScene);
};

#endif
