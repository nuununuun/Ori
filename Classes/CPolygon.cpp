//
//  CPolygon.cpp
//  Ori
//
//  Created by A on 2018. 3. 21..
//

#include "CPolygon.h"

USING_NS_CC;
using namespace std;

CPolygon * CPolygon::create(const vector<Vec2> &vertices) {
    auto ret = new (std::nothrow) CPolygon();
    
    if (ret && ret->initWithVertices(vertices)) {
        ret->autorelease();
    } else CC_SAFE_DELETE(ret);
    
    return ret;
}

bool CPolygon::initWithVertices(const vector<Vec2> &vertices) {
    _vertices = vertices;
    
    _drawer = DrawNode::create();
    this->addChild(_drawer);
    
    createEdges();
    
    return true;
}

void CPolygon::draw(const Color4F &fill, const Color4F &border, float width) {
    _drawer->clear();
    _drawer->drawPolygon(_vertices.data(), _vertices.size(), fill, width, border);
}

void CPolygon::symmetry(const Vec2 &start, const Vec2 &end) {
	float a = tan((end - start).getAngle());
	float b = -a * start.x + start.y;

	for (auto &v : _vertices) {
		Vec2 sym = getSymmetricVertex(a, b, v);
		v = Vec2(round(sym.x), round(sym.y));
	}
}

Vec2 CPolygon::getSymmetricVertex(float a, float b, const Vec2 &p) {
	return Vec2((2 * a * p.y - (a * a - 1) * p.x - 2 * a * b) / (a * a + 1), (2 * a * p.x + (a * a - 1) * p.y + 2 * b) / (a * a + 1));
}

void CPolygon::createEdges() {
    int vSize = _vertices.size();
    for (int i = 0; i < vSize; i++) {
        _edges.push_back(Edge(&_vertices[i], &_vertices[(i + 1) % vSize]));
    }
}
