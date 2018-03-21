//
//  CPolygon.cpp
//  Ori
//
//  Created by A on 2018. 3. 21..
//

#include "CPolygon.h"

USING_NS_CC;
using namespace std;

CPolygon * CPolygon::create(const vector<Vec2> &vertices) {
    auto ret = new (std::nothrow) CPolygon();
    
    if (ret && ret->initWithVertices(vertices)) {
        ret->autorelease();
    } else CC_SAFE_DELETE(ret);
    
    return ret;
}

bool CPolygon::initWithVertices(const vector<Vec2> &vertices) {
    _vertices = vertices;
    
    _drawer = DrawNode::create();
    this->addChild(_drawer);
    
    return true;
}

void CPolygon::draw(const Color4F &fill, const Color4F &border, float width) {
    _drawer->clear();
    _drawer->drawPolygon(_vertices.data(), _vertices.size(), fill, width, border);
}
