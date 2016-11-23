#include "MainScene.h"

USING_NS_CC;

bool MainScene::init()
{
	if (!Layer::init()) return false;


	Vec2 center = Director::getInstance()->getVisibleSize() / 2;
	int size = 200;

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

	v[0] = cut1;
	v[1] = cut2;

	draw->drawSolidPoly(v, 4, Color4F::RED);

	addChild(draw);

	delete[] v;

	////////////////

	v[0] = Vec2(-size / 2, -size / 2);
	v[1] = Vec2(size / 2, -size / 2);
	v[2] = Vec2(size / 2, size / 2);
	v[3] = Vec2(-size / 2, size / 2);

	draw = DrawNode::create();
	draw->setPosition(center);
	 
	//v[1] = symmetry(0.5, -50, v[0]);
	v[2] = symmetry(0.5, -50, Vec2(size / 2, -size / 2));
	v[3] = symmetry(0.5, -50, v[0]);

	draw->drawSolidPoly(v, 4, Color4F(0, 0, 1, 0.5));

	addChild(draw);

	delete[] v;

	/*auto r1 = DrawNode::create();
	r1->setPosition(center);

	Vec2 *v = new Vec2[4];

	v[0] = Vec2(-size / 2, -size / 2);
	v[1] = Vec2(size / 2, 0);
	v[2] = Vec2(size / 2, size / 2);
	v[3] = Vec2(-size / 2, size / 2);

	r1->drawSolidPoly(v, 4, Color4F(1, 0, 0, 1));

	addChild(r1);

	delete[] v;

	auto r2 = DrawNode::create();
	r2->setPosition(center);

	v = new Vec2[4];

	v[0] = Vec2(-size / 2, -size / 2);
	v[1] = Vec2(size / 2, 0);
	v[2] = r;
	v[3] = Vec2(-size / 2, -size / 2);

	r2->drawSolidPoly(v, 4, Color4F(0, 0, 1, 1));

	addChild(r2);

	delete[] v;*/


	return true;
}

Vec2 MainScene::symmetry(float a, float b, const Vec2 & point)
{
	return Vec2((2 * a * point.y - (a * a - 1) * point.x - 2 * a * b) / (a * a + 1), (2 * a * point.x + (a * a - 1) * point.y + 2 * b) / (a * a + 1));
}