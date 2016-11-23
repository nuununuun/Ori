#ifndef _CUSTOM_DRAW_NODE_H_
#define _CUSTOM_DRAW_NODE_H_

#include "cocos2d/cocos/2d/CCDrawNode.h"
#include <vector>

class CustomDrawNode : public cocos2d::DrawNode
{
public:
	CustomDrawNode(int lineWidth);
	~CustomDrawNode();
	static CustomDrawNode* create(int defaultLineWidth = 2);

public:
	void drawPolygon(std::vector<cocos2d::Vec2> verts, const cocos2d::Color4B &fillColor, float borderWidth, const cocos2d::Color4B &borderColor);

};

#endif
