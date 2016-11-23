#include "MainScene.h"

USING_NS_CC;

bool MainScene::init()
{
	if (!Layer::init()) return false;
    
    auto bg = LayerColor::create(Color4B(255, 255, 255, 255));
    addChild(bg);

	Vec2 center = Director::getInstance()->getVisibleSize() / 2;
	int size = 200;
    
    Vec2 touchPoint = Vec2(size / 2 - 50, -size / 2);
    Vec2 endPoint = Vec2(size / 2 - 120, -size / 2 + 60);
    
    (endPoint - touchPoint).getPerp();

	Vec2 cut1 = Vec2(-size / 2, -size / 2);
	Vec2 cut2 = Vec2(size / 2, 0);
	//Vec2 cut = Vec2(size / 2, 0) - Vec2(-size / 2, -size / 2);

	Vec2 r = symmetry(0.5f, -50, Vec2(size / 2, -size / 2));

	DrawNode *draw;

	Vec2 *v = new Vec2[4];

	////////////////

	v[0] = Vec2(-size / 2, -size / 2);
	v[1] = Vec2(size / 2, -size / 2);
	v[2] = Vec2(size / 2, size / 2);
	v[3] = Vec2(-size / 2, size / 2);

	draw = DrawNode::create();
	draw->setPosition(center);
//
//	v[0] = cut1;
//	v[1] = cut2;
    
    draw->drawPolygon(v, 4, Color4F(1, 0.236, 0.148, 1), 0.4, Color4F(0.6, 0.15, 0.07, 1));
    draw->drawPoint(touchPoint, 12, Color4F(0, 0, 1, 1));
    draw->drawPoint(endPoint, 12, Color4F(0, 1, 0, 1));

	addChild(draw);

//	delete[] v;

	////////////////

//	v[0] = Vec2(-size / 2, -size / 2);
//	v[1] = Vec2(size / 2, -size / 2);
//	v[2] = Vec2(size / 2, size / 2);
//	v[3] = Vec2(-size / 2, size / 2);
//
//	draw = DrawNode::create();
//	draw->setPosition(center);
//	 
//	v[1] = symmetry(0.5, -50, cut2);
//	v[2] = symmetry(0.5, -50, Vec2(size / 2, -size / 2));
//	v[3] = symmetry(0.5, -50, v[0]);
//
//	draw->drawPolygon(v, 4, Color4F(0.854, 0.236, 0.118, 1), 0.4, Color4F(0.6, 0.15, 0.07, 1));
//
//	addChild(draw);

//	delete[] v;

	return true;
}

Vec2 MainScene::symmetry(float a, float b, const Vec2 & point)
{
	return Vec2((2 * a * point.y - (a * a - 1) * point.x - 2 * a * b) / (a * a + 1), (2 * a * point.x + (a * a - 1) * point.y + 2 * b) / (a * a + 1));
}
