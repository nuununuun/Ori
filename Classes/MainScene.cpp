#include "MainScene.h"

#include "clipper.hpp"

USING_NS_CC;
using namespace std;
using namespace ClipperLib;

bool MainScene::init()
{
	if (!Layer::init()) return false;
    
    auto bg = LayerColor::create(Color4B(255, 255, 255, 255));
    addChild(bg);

	Vec2 center = Director::getInstance()->getVisibleSize() / 2;
	int size = 200;
    
    //-- 기본 사각형 버텍스 --//
    vector<Vec2> v;
    v.push_back(Vec2(-size / 2, -size / 2));
    v.push_back(Vec2(size / 2, -size / 2));
    v.push_back(Vec2(size / 2, size / 2));
    v.push_back(Vec2(-size / 2, size / 2));
    
    auto paper = CustomPolygon::create(v);
    
    paper->head = true;
    paper->drawPolygon();
    
    addChild(paper);
    
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan = CC_CALLBACK_2(MainScene::onTouchesBegan, this);
    listener->onTouchesMoved = CC_CALLBACK_2(MainScene::onTouchesMoved, this);
    listener->onTouchesEnded = CC_CALLBACK_2(MainScene::onTouchesEnded, this);
    
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    test = Label::createWithSystemFont("0", "", 20);
    test->setTextColor(Color4B::BLACK);
    test->setAnchorPoint((Vec2(0, 1)));
    test->setName("test");
    test->setPosition(10, center.y * 2 - 10);
    addChild(test);
    
    papers.push_back(paper);

	/// Debug ///
    
	return true;
}

void MainScene::onTouchesBegan(const vector<Touch*> &t, cocos2d::Event *e) {
    
    for (auto &i : t) {
        startPoint = i->getLocation();
    }
}

void MainScene::onTouchesMoved(const vector<Touch*> &t, cocos2d::Event *e) {

    for (auto &i : t) {
        endPoint = i->getLocation();
        
        Vec2 center = Director::getInstance()->getVisibleSize() / 2;
        
        Vec2 a = startPoint - center;
        Vec2 b = endPoint - center;
        
        if (startPoint.fuzzyEquals(endPoint, 1)) continue;
            
        Vec2 normalVector = (a - b).getPerp();
            
        auto c = a - b;
        if (action == -1) {
            if (c.x > 0 && c.y < 0) action = 0;
            if (c.x < 0 && c.y < 0) action = 1;
            if (c.x > 0 && c.y > 0) action = 2;
            if (c.x < 0 && c.y > 0) action = 3;
        }
//        if (action == 0) test->setString("leftup");
//        else if (action == 1) test->setString("rightup");
//        else if (action == 2) test->setString("leftdown");
//        else if (action == 3) test->setString("rightdown");
            
        for (auto j : papers) {
            j->cutPolygonPreview(action, -normalVector * 1000 + (a + b) / 2, normalVector * 1000 + (a + b) / 2, a, b);
            j->drawPreview();
        }
    }
}

void MainScene::onTouchesEnded(const vector<Touch*> &t, cocos2d::Event *e) {
    for (auto &i : t) {
        endPoint = i->getLocation();
        
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

        
        action = -1;
    }
}
