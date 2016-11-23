#include "MainScene.h"

USING_NS_CC;
using namespace std;

bool MainScene::init()
{
	if (!Layer::init()) return false;
    
    auto bg = LayerColor::create(Color4B(255, 255, 255, 255));
    addChild(bg);

	Vec2 center = Director::getInstance()->getVisibleSize() / 2;
	int size = 200;
    
    Vec2 touchPoint = Vec2(size / 2 - 50, -size / 2);
    Vec2 endPoint = Vec2(size / 2 - 120, -size / 2 + 60);
    
    /// 법선
    Vec2 norm = (endPoint - touchPoint).getPerp();

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
    
    
    auto cuts = clipLine(-norm * 200 + (touchPoint + endPoint) / 2, norm * 200 + (touchPoint + endPoint) / 2, v, 4);
    
    

	draw = DrawNode::create();
	draw->setPosition(center);
//
//	v[0] = cut1;
//	v[1] = cut2;
    
    draw->drawPolygon(v, 4, Color4F(1, 0.236, 0.148, 1), 0.4, Color4F(0.6, 0.15, 0.07, 1));
    draw->drawPoint(touchPoint, 12, Color4F(0, 0, 1, 1));
    draw->drawPoint(endPoint, 12, Color4F(0, 1, 0, 1));
    
    draw->drawLine(-norm * 200 + (touchPoint + endPoint) / 2, norm * 200 + (touchPoint + endPoint) / 2, Color4F::GREEN);
    
    for (auto i : cuts) {
        draw->drawPoint(i, 12, Color4F(0.2, 0.2, 0.2, 1));
    }

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


vector<Vec2> MainScene::clipLine(const Vec2 &p1, const Vec2 &p2, Vec2 *vertices, int vlen)
{
    vector<Vec2> ret;
    
    vector<float> tValues;
    
    ret.push_back(p1);
    tValues.push_back(0);
    
    for (int i = 0; i < vlen; i++) {
        int j = (i + 1) % vlen;
        
        bool lineIntersect, segmentIntersect;
        Vec2 intersection, closeP1, closeP2;
        
        float t1, t2;
        
        findIntersection(p1, p2, vertices[i], vertices[j], lineIntersect, segmentIntersect, intersection, closeP1, closeP2);
        
        if(segmentIntersect) {
            ret.push_back(intersection);
            tValues.push_back(t1);
        }
    }
    
    ret.push_back(p2);
    tValues.push_back(1);
    
    // sort intersections
    
    return ret;
}

void MainScene::findIntersection(const Vec2 &p1, const Vec2 &p2, const Vec2 &p3, const Vec2 &p4, bool &lineIntersect, bool &segmentIntersect, Vec2 &intersection, Vec2 &closeP1, Vec2 &closeP2)
{
    float dx12 = p2.x - p1.x;
    float dy12 = p2.y - p1.y;
    float dx34 = p4.x - p3.x;
    float dy34 = p4.y - p3.y;
    
    float denominator = (dy12 * dx34 - dx12 * dy34);
    
    float t1 = ((p1.x - p3.x) * dy34 + (p3.y - p1.y) * dx34) / denominator;

    lineIntersect = true;
    
    float t2 = ((p3.x - p1.x) * dy12 + (p1.y - p3.y) * dx12) / -denominator;
    
    // Find the point of intersection.
    intersection = Vec2(p1.x + dx12 * t1, p1.y + dy12 * t1);
    
    // The segments intersect if t1 and t2 are between 0 and 1.
    segmentIntersect = ((t1 >= 0) && (t1 <= 1) && (t2 >= 0) && (t2 <= 1));
    
    // Find the closest points on the segments.
    if (t1 < 0) {
        t1 = 0;
    }
    else if (t1 > 1) {
        t1 = 1;
    }
    
    if (t2 < 0) {
        t2 = 0;
    }
    else if (t2 > 1) {
        t2 = 1;
    }
    
    closeP1 = Vec2(p1.x + dx12 * t1, p1.y + dy12 * t1);
    closeP2 = Vec2(p3.x + dx34 * t2, p3.y + dy34 * t2);
    
}
