//
//  Paper.cpp
//  Ori
//
//  Created by A on 2018. 3. 21..
//

#include "Paper.h"

USING_NS_CC;
using namespace std;

Paper * Paper::create(int size) {
    auto ret = new (std::nothrow) Paper();
    
    if (ret && ret->init(size)) {
        ret->autorelease();
    } else CC_SAFE_DELETE(ret);
    
    return ret;
}

bool Paper::init(int size) {
    vector<Vec2> v = {
        Vec2(-size / 2, -size / 2),
        Vec2(size / 2, -size / 2),
        Vec2(size / 2, size / 2),
        Vec2(-size / 2, size / 2)
    };
    
    auto poly = CPolygon::create(v);
    poly->draw(_fillColor, _borderColor, _width);
    this->addChild(poly);
    
    _polygons.push_back(poly);
    
    return true;
}

void Paper::findVertex(const cocos2d::Vec2 &pos) {
    CPolygon *polygon = nullptr;
    Vec2 *minVertex;
    float minLength = INT_MAX;
    // 폴리곤들 내부
    for (auto &poly : _polygons) {
        // 정점들 중에서
        for (auto &vertex : poly->_vertices) {
            // 입력받은 점과 정점을 비교함
            float len = (vertex - pos).getLengthSq();
            // 입력받은 점에서 가장 짧은 정점을 찾음
            if (minLength > len) {
                polygon = poly;
                minVertex = &vertex;
                minLength = len;
            }
        }
    }
    
    if (minLength <= 400) {
        *minVertex = pos;
        polygon->draw(_fillColor, _borderColor, _width);
    }
}
