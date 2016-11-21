#ifndef _MAIN_SCENE_H_
#define _MAIN_SCENE_H_

#include "cocos2d.h"

class MainScene : public cocos2d::Layer {
public:
	static cocos2d::Scene *createScene() {
		auto scene = cocos2d::Scene::create();
		auto layer = MainScene::create();

		scene->addChild(layer);
		return scene;
	}

	virtual bool init();

	cocos2d::Vec2 symmetry(float a, float b, const cocos2d::Vec2 &point);

	CREATE_FUNC(MainScene);
};

#endif