//
//  CustomPolygon.h
//  Ori
//
//  Created by a on 2016. 11. 27..
//
//

#ifndef CustomPolygon_h
#define CustomPolygon_h

#include "cocos2d.h"
#include "CustomDrawNode.h"
#include <vector>

class CustomPolygon : public cocos2d::Node {
public:
    static CustomPolygon *create();
    static CustomPolygon *create(const std::vector<cocos2d::Vec2> &vertices);
    
    void drawPolygon(const cocos2d::Color4B &fill, const cocos2d::Color4B &border);
    
    std::vector<cocos2d::Vec2> vertices;
    std::vector<cocos2d::Vec2> previewVertices;
    CustomDrawNode *polygon;
};

#endif /* CustomPolygon_h */
