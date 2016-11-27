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
    
    //-- 기본 사각형 버텍스 --//
    vector<Vec2> v;
    v.push_back(Vec2(-size / 2, -size / 2));
    v.push_back(Vec2(size / 2, -size / 2));
    v.push_back(Vec2(size / 2, size / 2));
    v.push_back(Vec2(-size / 2, size / 2));
    
    auto paper = CustomPolygon::create(v);
    
    paper->head = true;
    paper->calculateEdges();
    paper->drawPolygon();
    
    addChild(paper);
    
    auto other = CustomPolygon::create();
    other->head = false;
    addChild(other);
    
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan = CC_CALLBACK_2(MainScene::onTouchesBegan, this);
    listener->onTouchesMoved = CC_CALLBACK_2(MainScene::onTouchesMoved, this);
    listener->onTouchesEnded = CC_CALLBACK_2(MainScene::onTouchesEnded, this);
    
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    Vec2 touchPoint = Vec2(size / 2 - 50, -size / 2);
    Vec2 endPoint = Vec2(size / 2 - 70, -size / 2 + 90);
    
    startDraw = CustomDrawNode::create();
    startDraw->drawPoint(Vec2::ZERO, 12, Color4F::BLACK);
    addChild(startDraw);
    
    endDraw = CustomDrawNode::create();
    endDraw->drawPoint(Vec2::ZERO, 12, Color4F::BLUE);
    addChild(endDraw);
    
    test = Label::createWithSystemFont("0", "", 20);
    test->setTextColor(Color4B::BLACK);
    test->setAnchorPoint((Vec2(0, 1)));
    test->setPosition(10, center.y * 2 - 10);
    addChild(test);
    
    papers.push_back(paper);
    papers.push_back(other);
    
	return true;
}

void MainScene::onTouchesBegan(const vector<Touch*> &t, cocos2d::Event *e) {
    
    startPoint = t[0]->getLocation();
    
    startDraw->setPosition(startPoint);
}

void MainScene::onTouchesMoved(const vector<Touch*> &t, cocos2d::Event *e) {

    endPoint = t[0]->getLocation();
    
    endDraw->setPosition(endPoint);
    
    Vec2 center = Director::getInstance()->getVisibleSize() / 2;
    
    Vec2 a = startPoint - center;
    Vec2 b = endPoint - center;
    
    if (startPoint.fuzzyEquals(endPoint, 1)) return;
    
    foldPaperPreview(papers[0], papers[1], a, b);

}

void MainScene::onTouchesEnded(const vector<Touch*> &t, cocos2d::Event *e) {
    
    endPoint = t[0]->getLocation();
    
    endDraw->setPosition(endPoint);
    
    Vec2 center = Director::getInstance()->getVisibleSize() / 2;
    
    Vec2 a = startPoint - center;
    Vec2 b = endPoint - center;
    
    if (startPoint.fuzzyEquals(endPoint, 1)) return;
    
    foldPaperPreview(papers[0], papers[1], a, b);
    
}

void MainScene::foldPaper(CustomPolygon *original, CustomPolygon *splitted, const Vec2 &start, const Vec2 &end) {
    
    Vec2 center = Director::getInstance()->getVisibleSize() / 2;
    
    /// 법선
    Vec2 normalVector = (end - start).getPerp();
    
    ////////////////
    
    //-- 터치 시작점과 끝점을 입력받아 폴리곤을 자름 --//
    auto cut = clipLine(-normalVector * 600 + (start + end) / 2, normalVector * 600 + (start + end) / 2, original->vertices);
    float a = tan((cut[2] - cut[1]).getAngle());
    float b = -a * cut[1].x + cut[1].y;
    
    //-- 잘린 두 폴리곤을 만듦 --// // poly0 = 고정 폴리곤, poly1 = 비고정 폴리곤
    vector<Vec2> poly[2];
    Vec2 cp = (cut[1] + cut[2]) / 2;
    
    float n = normalVector.getAngle();
    n = n >= 0 ? n : 3.141592 + n;
    
    int i1 = 0, i2 = 1;
    if (end.x > start.x) {
        i1 = 1;
        i2 = 0;
    }
    
    float testAngle[4];
    int cnt = 0;
    for (const auto &i : original->vertices) {
        
        float angle = round(CC_RADIANS_TO_DEGREES(atan2(i.y - cp.y, i.x - cp.x) - n));
        angle = angle > 180 ? -360 + angle : angle < -180 ? 360 + angle : angle;
        
        if (angle >= 0 || angle < -180)
            poly[i1].push_back(i);
        else
            poly[i2].push_back(i);
        
        testAngle[cnt++] = angle;
    }
    
    test->setString("0: " + std::to_string(testAngle[0]) + ", 1: " + std::to_string(testAngle[1]) + "\n2: " + std::to_string(testAngle[2]) + ", 3: " + std::to_string(testAngle[3]));
    
    poly[0].push_back(cut[1]);
    poly[0].push_back(cut[2]);
    
    original->setVertices(poly[0]);
    original->sortVertices(cp);
    original->calculateEdges();
    
    original->drawPolygon();
    
    
    ////////////////
    
    for (int i = 0; i < poly[1].size(); i++) {
        Vec2 sym = symmetry(a, b, poly[1][i]);
        poly[1][i] = Vec2(round(sym.x), round(sym.y));
    }
    
    poly[1].push_back(cut[1]);
    poly[1].push_back(cut[2]);
    
    splitted->setVertices(poly[1]);
    splitted->sortVertices(cp);
    splitted->calculateEdges();
    
    splitted->drawPolygon();
}

void MainScene::foldPaperPreview(CustomPolygon *original, CustomPolygon *splitted, const Vec2 &start, const Vec2 &end) {
    
    Vec2 center = Director::getInstance()->getVisibleSize() / 2;
    
    /// 법선
    Vec2 normalVector = (end - start).getPerp();
    
    ////////////////
    
    //-- 터치 시작점과 끝점을 입력받아 폴리곤을 자름 --//
    auto cut = clipLine(-normalVector * 600 + (start + end) / 2, normalVector * 600 + (start + end) / 2, original->previewVertices);
    float a = tan((cut[2] - cut[1]).getAngle());
    float b = -a * cut[1].x + cut[1].y;
    
    //-- 잘린 두 폴리곤을 만듦 --// // poly0 = 고정 폴리곤, poly1 = 비고정 폴리곤
    vector<Vec2> poly[2];
    Vec2 cp = (cut[1] + cut[2]) / 2;
    
    float n = normalVector.getAngle();
    n = n >= 0 ? n : 3.141592 + n;
    
    int i1 = 0, i2 = 1;
    if (end.x > start.x) {
        i1 = 1;
        i2 = 0;
    }
    
    float testAngle[4];
    int cnt = 0;
    for (const auto &i : original->previewVertices) {
        
        float angle = round(CC_RADIANS_TO_DEGREES(atan2(i.y - cp.y, i.x - cp.x) - n));
        angle = angle > 180 ? -360 + angle : angle < -180 ? 360 + angle : angle;
        
        if (angle >= 0 || angle < -180)
            poly[i1].push_back(i);
        else
            poly[i2].push_back(i);
        
        testAngle[cnt++] = angle;
    }
    
    test->setString("0: " + std::to_string(testAngle[0]) + ", 1: " + std::to_string(testAngle[1]) + "\n2: " + std::to_string(testAngle[2]) + ", 3: " + std::to_string(testAngle[3]));
    
    poly[0].push_back(cut[1]);
    poly[0].push_back(cut[2]);
    
    std::sort(poly[0].begin(), poly[0].end(), [&](const Vec2 &a, const Vec2 &b)->bool {
        return atan2(cp.y - a.y, cp.x - a.x) > atan2(cp.y - b.y, cp.x - b.x);
    });
    
    original->setVertices(poly[0]);
    original->sortVertices(cp);
    original->drawPolygon();
    
    
    ////////////////
    
    for (int i = 0; i < poly[1].size(); i++) {
        Vec2 sym = symmetry(a, b, poly[1][i]);
        poly[1][i] = Vec2(round(sym.x), round(sym.y));
    }
    
    poly[1].push_back(cut[1]);
    poly[1].push_back(cut[2]);
    
    splitted->setVertices(poly[1]);
    splitted->sortVertices(cp);
    
    splitted->drawPolygon();
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
