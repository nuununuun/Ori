//
//  Paper.h
//  Ori
//
//  Created by A on 2018. 3. 21..
//

#ifndef Paper_h
#define Paper_h

#include "cocos2d.h"

#include "CPolygon.h"

class Paper : public cocos2d::Node {
public:
    static Paper * create(int size);
    
    bool init(int size);
    
    void selectPolygon(const cocos2d::Vec2 &pos);
    
    void splitPolygon(const cocos2d::Vec2 &v1, const cocos2d::Vec2 &v2);

	cocos2d::DrawNode *	_debug;
    
protected:
    float getDistanceSqPointSegment(const cocos2d::Vec2 &v1, const cocos2d::Vec2 &v2, const cocos2d::Vec2 &p);
    
    cocos2d::Vec2 * findVertex(const cocos2d::Vec2 &pos, CPolygon **polygon);
    Edge * findEdge(const cocos2d::Vec2 &pos, CPolygon **polygon);
    
protected:
    std::vector<CPolygon *> _polygons;
    
    cocos2d::Color4F \
        _fillColor = cocos2d::Color4F(1.0f, 0.3f, 0.3f, 1.0f),
        _borderColor = cocos2d::Color4F(0.75f, 0.25f, 0.25f, 1.0f);
    
    float _width = 1.0f;
    
public:
    // 0 = nothing, 1 = vertex, 2 = edge
    int _selectMode = 0;
    
    cocos2d::Vec2 * _selectedVertex;
    Edge * _selectedEdge;
    
};

#endif /* Paper_h */
