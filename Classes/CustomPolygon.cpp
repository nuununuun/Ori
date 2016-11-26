//
//  CustomPolygon.cpp
//  Ori
//
//  Created by a on 2016. 11. 27..
//
//

#include "CustomPolygon.h"

USING_NS_CC;

CustomPolygon *CustomPolygon::create() {
    CustomPolygon * ret = new (std::nothrow) CustomPolygon();
    if (ret && ret->init())
    {
        Vec2 center = Director::getInstance()->getVisibleSize() / 2;
        
        ret->autorelease();
        
        ret->polygon = CustomDrawNode::create();
        ret->polygon->setPosition(center);
        
        ret->addChild(ret->polygon);
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

CustomPolygon *CustomPolygon::create(const std::vector<cocos2d::Vec2> &v) {
    CustomPolygon * ret = new (std::nothrow) CustomPolygon();
    if (ret && ret->init())
    {
        Vec2 center = Director::getInstance()->getVisibleSize() / 2;
        
        ret->autorelease();
        
        ret->polygon = CustomDrawNode::create();
        ret->polygon->setPosition(center);
        
        ret->addChild(ret->polygon);
        
        ret->vertices = v;
        ret->previewVertices = v;
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void CustomPolygon::drawPolygon(const cocos2d::Color4B &fill, const cocos2d::Color4B &border) {
    polygon->clear();
    polygon->drawPolygon(vertices, fill, 0.4, border);
}
