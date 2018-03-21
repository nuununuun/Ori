//
//  CPolygon.h
//  Ori
//
//  Created by A on 2018. 3. 21..
//

#ifndef CPolygon_h
#define CPolygon_h

#include "cocos2d.h"

class CPolygon : public cocos2d::Node {
public:
    static CPolygon * create(const std::vector<cocos2d::Vec2> &vertices);
    
    bool initWithVertices(const std::vector<cocos2d::Vec2> &vertices);
    
    void draw(const cocos2d::Color4F &fill, const cocos2d::Color4F &border, float width);

	void symmetry(const cocos2d::Vec2 &start, const cocos2d::Vec2 &end);
    
public:
    std::vector<cocos2d::Vec2> _vertices;
    
    cocos2d::DrawNode * _drawer;

protected:
	cocos2d::Vec2 getSymmetricVertex(float a, float b, const cocos2d::Vec2 &p);
    
};

#endif /* CPolygon_h */
