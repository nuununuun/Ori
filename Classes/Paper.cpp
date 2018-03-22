//
//  Paper.cpp
//  Ori
//
//  Created by A on 2018. 3. 21..
//

#include "Paper.h"

USING_NS_CC;
using namespace std;

Paper * Paper::create(int size) {
    auto ret = new (std::nothrow) Paper();
    
    if (ret && ret->init(size)) {
        ret->autorelease();
    } else CC_SAFE_DELETE(ret);
    
    return ret;
}

bool Paper::init(int size) {
    vector<Vec2> v = {
        Vec2(-size / 2, -size / 2),
        Vec2(size / 2, -size / 2),
        Vec2(size / 2, size / 2),
        Vec2(-size / 2, size / 2)
    };
    
    auto poly = CPolygon::create(v);
    poly->draw(_fillColor, _borderColor, _width);
    this->addChild(poly);
    
    _polygons.push_back(poly);

	_debug = DrawNode::create();
	this->addChild(_debug);
    
    return true;
}

Vec2 * Paper::findVertex(const Vec2 &pos, CPolygon **polygon) {
    Vec2 * ret = nullptr;
    
    float limit = 200.0f;
    
    // 폴리곤들 내부
    float minLength = INT_MAX;
    for (auto &poly : _polygons) {
        // 정점들 중에서
        for (auto &vertex : poly->_vertices) {
            // 입력받은 점과 정점을 비교함
            float len = vertex.getDistanceSq(pos);
            // 입력받은 점에서 가장 짧은 정점을 찾음
            if (minLength > len) {
				if (polygon != nullptr) *polygon = poly;
                ret = &vertex;
                minLength = len;
            }
        }
    }
    
    if (minLength <= limit) {
        return ret;
    }

	return nullptr;
}

Edge * Paper::findEdge(const Vec2 &pos, CPolygon **polygon) {
    Edge * ret = nullptr;
    
    float limit = 200.0f;
    
    float minLength = INT_MAX;
    for (auto &poly : _polygons) {
        // edge들 중
        for (auto &edge : poly->_edges) {
            // 입력받은 점과 edge 사이의 거리를 구함
            float len = getDistanceSqPointSegment(*edge.from, *edge.to, pos);
            
            // 가장 거리가 짧은 edge를 찾음
            if (minLength > len) {
                if (polygon != nullptr) *polygon = poly;
                ret = &edge;
                minLength = len;
            }
        }
    }
    
    if (minLength <= limit) {
        return ret;
    }
    
    return nullptr;
}

void Paper::selectPolygon(const Vec2 &pos) {
    _selectMode = 0;
    
    // 정점이 선택될 경우
    _selectedVertex = this->findVertex(pos, nullptr);
    if (_selectedVertex != nullptr) {
        this->_debug->drawDot(*_selectedVertex, 8, Color4F::ORANGE);
        _selectMode = 1;
    } else {
        // 엣지가 선택될 경우
        _selectedEdge = this->findEdge(pos, nullptr);
        if (_selectedEdge != nullptr) {
            this->_debug->drawSegment(*(*_selectedEdge).from, *(*_selectedEdge).to, 2, Color4F::GREEN);
            _selectMode = 2;
        }
    }
}

//http://www.randygaul.net/2014/07/23/distance-point-to-line-segment/
float Paper::getDistanceSqPointSegment(const Vec2 &v1, const Vec2 &v2, const Vec2 &p) {
    auto n = v2 - v1, pa = v1 - p;
    
    float c = n.dot(pa);
    
    // Closest point is a
    if ( c > 0.0f )
        return pa.dot(pa);
    
    auto bp = p - v2;
    
    // Closest point is b
    if ( n.dot(bp) > 0.0f )
        return bp.dot(bp);
    
    // Closest point is between a and b
    auto e = pa - n * (c / n.dot(n));
    
    return e.dot(e);
}

void Paper::splitPolygon(const Vec2 &v1, const Vec2 &v2) {
    for (auto &poly : _polygons) {
        for (auto &edge : poly->_edges) {
            if (Vec2::isSegmentIntersect(*edge.from, *edge.to, v1, v2)) {
                Vec2 p = Vec2::getIntersectPoint(*edge.from, *edge.to, v1, v2);
                _debug->drawDot(p, 4, Color4F(0, 0.5f, 1.0f, 1.0f));
            }
        }
    }
}
