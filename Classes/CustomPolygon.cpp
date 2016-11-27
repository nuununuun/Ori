//
//  CustomPolygon.cpp
//  Ori
//
//  Created by a on 2016. 11. 27..
//
//

#include "CustomPolygon.h"

USING_NS_CC;
using namespace std;

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

CustomPolygon *CustomPolygon::create(const vector<cocos2d::Vec2> &v) {
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

void CustomPolygon::setVertices(const vector<cocos2d::Vec2> &v)
{
    vertices = v;
}

void CustomPolygon::sortVertices(const Vec2 &center) {
    std::sort(vertices.begin(), vertices.end(), [&](const Vec2 &a, const Vec2 &b)->bool {   
        return atan2(center.y - a.y, center.x - a.x) > atan2(center.y - b.y, center.x - b.x);
    });
}

void CustomPolygon::calculateEdges() {
    for (int i = 0; i < vertices.size() - 1; i++) {
        edges.push_back(make_pair(vertices[i], vertices[i + 1]));
    }
    
    edges.push_back(make_pair(vertices.back(), vertices.front()));
    
}

void CustomPolygon::drawPolygon() {
    polygon->clear();
    if (head)
        polygon->drawPolygon(vertices, Color4B(250, 32, 24, 255), 0.4, Color4B(80, 80, 80, 255));
    else
        polygon->drawPolygon(vertices, Color4B(255, 90, 72, 255), 0.4, Color4B(80, 80, 80, 255));
}

void CustomPolygon::drawPolygonPreview() {
    polygon->clear();
    if (head)
        polygon->drawPolygon(previewVertices, Color4B(250, 32, 24, 255), 0.4, Color4B(80, 80, 80, 255));
    else
        polygon->drawPolygon(previewVertices, Color4B(255, 90, 72, 255), 0.4, Color4B(80, 80, 80, 255));
}
