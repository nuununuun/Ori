#include "MainScene.h"

#include "InputManager.h"

USING_NS_CC;
using namespace std;

bool MainScene::init() {
    auto bg = LayerColor::create(Color4B::WHITE);
    this->addChild(bg);

	Vec2 center = Director::getInstance()->getVisibleSize() / 2;
    
    int size = 200;
    
    _paper = Paper::create(size);
    _paper->setPosition(center);
    this->addChild(_paper);
    
    InputManager::getInstance()->initialize(this);
    InputManager::getInstance()->setTouchesEventCallback(CC_CALLBACK_2(MainScene::onTouchesBegan, this), "began");
    InputManager::getInstance()->setTouchesEventCallback(CC_CALLBACK_2(MainScene::onTouchesMoved, this), "moved");
    InputManager::getInstance()->setTouchesEventCallback(CC_CALLBACK_2(MainScene::onTouchesEnded, this), "ended");
    
	return true;
}

void MainScene::onTouchesBegan(const vector<Touch*> &touches, cocos2d::Event *e) {
    
    for (auto &t : touches) {
        _paper->findVertex(_paper->convertToNodeSpace(t->getLocation()));
    }
    
}

void MainScene::onTouchesMoved(const vector<Touch*> &touches, cocos2d::Event *e) {

    for (auto &t : touches) {
//        endPoint = i->getLocation();
//
//        Vec2 center = Director::getInstance()->getVisibleSize() / 2;
//
//        Vec2 a = startPoint - center;
//        Vec2 b = endPoint - center;
//
//        if (startPoint.fuzzyEquals(endPoint, 1)) continue;
//
//        Vec2 normalVector = (a - b).getPerp();
//
//        auto c = a - b;
//        if (action == -1) {
//            if (c.x > 0 && c.y < 0) action = 0;
//            if (c.x < 0 && c.y < 0) action = 1;
//            if (c.x > 0 && c.y > 0) action = 2;
//            if (c.x < 0 && c.y > 0) action = 3;
//        }
//        if (action == 0) test->setString("leftup");
//        else if (action == 1) test->setString("rightup");
//        else if (action == 2) test->setString("leftdown");
//        else if (action == 3) test->setString("rightdown");
            
//        for (auto j : papers) {
//            j->cutPolygonPreview(action, -normalVector * 1000 + (a + b) / 2, normalVector * 1000 + (a + b) / 2, a, b);
//            j->drawPreview();
//        }
    }
}

void MainScene::onTouchesEnded(const vector<Touch*> &touches, cocos2d::Event *e) {
    for (auto &t : touches) {
//        endPoint = i->getLocation();
        
//        Vec2 center = Director::getInstance()->getVisibleSize() / 2;
//        
//        Vec2 a = startPoint - center;
//        Vec2 b = endPoint - center;
//        
//        if (startPoint.fuzzyEquals(endPoint, 1)) continue;
//        
//        
//        auto c = a - b;
//        
//        Vec2 normalVector = c.getPerp();
//        
//        vector<CustomPolygon *> splittedPolygons;
//        
//        for (auto &j : papers) {
//            auto cutted = j->cutPolygon(action, -normalVector * 1000 + (a + b) / 2, normalVector * 1000 + (a + b) / 2, a, b);
//            j->drawPreview();
//            
////            for (auto j : cutted) {
////                splittedPolygons.push_back(j);
//            //            }
//            papers.push_back(cutted[0]);
//            papers.push_back(cutted[1]);
//        }
//        
////        for (auto i : papers) {
////            i->;
////        }
//        
////        papers.clear();
////        
////        for (auto &i : splittedPolygons) {
////            papers.push_back(i);
////        }

        
//        action = -1;
    }
}
