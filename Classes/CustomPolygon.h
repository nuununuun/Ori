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
    
    void drawPolygon();
    void drawPolygonPreview();
    
    void setVertices(const std::vector<cocos2d::Vec2> &vertices);
    void sortVertices(const cocos2d::Vec2 &center);
    
    std::vector<cocos2d::Vec2> vertices;
    std::vector<std::pair<cocos2d::Vec2, cocos2d::Vec2>> edges;
    std::vector<cocos2d::Vec2> previewVertices;
    CustomDrawNode *polygon;
    
    bool head = true;
    
private:
};

#endif /* CustomPolygon_h */
