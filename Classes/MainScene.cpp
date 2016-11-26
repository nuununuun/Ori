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
    Vec2 endPoint = Vec2(size / 2 - 70, -size / 2 + 90);
    
    /// 법선
    Vec2 norm = (endPoint - touchPoint).getPerp();

	CustomDrawNode *draw;

	vector<Vec2> v;

	////////////////

    //-- 기본 사각형 버텍스 --//
	v.push_back(Vec2(-size / 2, -size / 2));
	v.push_back(Vec2(size / 2, -size / 2));
	v.push_back(Vec2(size / 2, size / 2));
	v.push_back(Vec2(-size / 2, size / 2));

    //-- 터치 시작점과 끝점을 입력받아 폴리곤을 자름 --//
	auto cut = clipLine(-norm * 320 + (touchPoint + endPoint) / 2, norm * 320 + (touchPoint + endPoint) / 2, v);
	float a = tan((cut[2] - cut[1]).getAngle());
	float b = -a * cut[1].x + cut[1].y;
    
    //-- 잘린 두 폴리곤을 만듦 --// // poly0 = 고정 폴리곤, poly1 = 비고정 폴리곤
    vector<Vec2> poly[2];
    Vec2 cp = (cut[1] + cut[2]) / 2;
    
    float n = norm.getAngle();
    n = n >= 0 ? n : 3.141592 + n;
    
    for (const auto &i : v) {
        
        float angle = round(CC_RADIANS_TO_DEGREES(atan2(i.y - cp.y, i.x - cp.x) - n));
        angle = angle > 180 ? -360 + angle : angle < -180 ? 360 + angle : angle;
        
        if (angle >= 0 || angle < -180)
            poly[0].push_back(i);
        else
            poly[1].push_back(i);
    }
    
    poly[0].push_back(cut[1]);
    poly[0].push_back(cut[2]);
    
    std::sort(poly[0].begin(), poly[0].end(), [&](const Vec2 &a, const Vec2 &b)->bool {
        return atan2(0 - a.y, 0 - a.x) > atan2(0 - b.y, 0 - b.x);
    });

	draw = CustomDrawNode::create();
	draw->setPosition(center);
    
    draw->drawPolygon(poly[0], Color4B(255, 30, 20, 255), 0.4, Color4B(80, 80, 80, 255));

	addChild(draw);

	////////////////
    
    for (int i = 0; i < poly[1].size(); i++) {
        Vec2 sym = symmetry(a, b, poly[1][i]);
        poly[1][i] = Vec2(round(sym.x), round(sym.y));
    }
    
    poly[1].push_back(cut[1]);
    poly[1].push_back(cut[2]);
    
    
    std::sort(poly[1].begin(), poly[1].end(), [&](const Vec2 &a, const Vec2 &b)->bool{
        return atan2(0 - a.y, 0 - a.x) > atan2(0 - b.y, 0 - b.x);
    });
    
    draw = CustomDrawNode::create();
    draw->setPosition(center);
    
    draw->drawPolygon(poly[1], Color4B(255, 80, 60, 255), 0.4, Color4B(80, 80, 80, 255));
    draw->drawPoint(touchPoint, 9, Color4F::BLACK);
    draw->drawPoint(endPoint, 9, Color4F::BLACK);
    
    addChild(draw);

	return true;
}

Vec2 MainScene::symmetry(float a, float b, const Vec2 & point)
{
	return Vec2((2 * a * point.y - (a * a - 1) * point.x - 2 * a * b) / (a * a + 1), (2 * a * point.x + (a * a - 1) * point.y + 2 * b) / (a * a + 1));
}


vector<Vec2> MainScene::clipLine(const Vec2 &p1, const Vec2 &p2, vector<Vec2> vertices)
{
    vector<Vec2> ret;
    
    vector<float> tValues;
    
    ret.push_back(p1);
    tValues.push_back(0);
    
    for (int i = 0; i < vertices.size(); i++) {
        int j = (i + 1) % vertices.size();
        
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

	/// tValues사용해서 정렬하기.
    
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
    
    intersection = Vec2(p1.x + dx12 * t1, p1.y + dy12 * t1);
    
    segmentIntersect = ((t1 >= 0) && (t1 <= 1) && (t2 >= 0) && (t2 <= 1));
    
    if (t1 < 0) t1 = 0;
    else if (t1 > 1) t1 = 1;
    
    if (t2 < 0) t2 = 0;
    else if (t2 > 1) t2 = 1;
    
    closeP1 = Vec2(p1.x + dx12 * t1, p1.y + dy12 * t1);
    closeP2 = Vec2(p3.x + dx34 * t2, p3.y + dy34 * t2);
   
}
