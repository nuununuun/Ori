//
//  Paper.cpp
//  Ori
//
//  Created by A on 2018. 3. 21..
//

#include "Paper.h"

#include "PolySplitter.hpp"

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
    poly->draw(_frontColor, _borderColor, _width);
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
    _selectMode = NOTHING;
    
    Vec3 a;
    
    a.lengthSquared();
    
    // 정점이 선택될 경우
    _selectedVertex = this->findVertex(pos, nullptr);
    if (_selectedVertex != nullptr) {
        this->_debug->drawDot(*_selectedVertex, 8, Color4F::ORANGE);
        _selectMode = VERTEX;
    } else {
        // 엣지가 선택될 경우
        _selectedEdge = this->findEdge(pos, nullptr);
        if (_selectedEdge != nullptr) {
            this->_debug->drawSegment(*(*_selectedEdge).from, *(*_selectedEdge).to, 2, Color4F::GREEN);
            _selectMode = EDGE;
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

bool Paper::isEqualEdges(const Vec2 &fedge1, const Vec2 &tedge1, const Vec2 &fedge2, const Vec2 &tedge2) {
    
    return true;
}

void Paper::splitPolygon(const Vec2 &v1, const Vec2 &v2) {
    for (auto it = _polygons.begin(); it != _polygons.end(); ) {
        
        // 폴리곤과 자르는 선 사이의 충돌점을 찾음.
        std::vector<Vec2> points;
        for (auto &edge : (*it)->_edges) {
            if (Vec2::isSegmentIntersect(*edge.from, *edge.to, v1, v2)) {
                Vec2 p = Vec2::getIntersectPoint(*edge.from, *edge.to, v1, v2);
                _debug->drawDot(p, 4, Color4F(0, 0.5f, 1.0f, 1.0f));
                points.push_back(p);
            }
        }
        
        auto spPoly = PolySplitter::getInstance()->Split((*it)->_vertices, points[0], points[1]);
        
        int index = 0;
        bool exit = false;
        // 선택된 점이나 엣지가 포함된 폴리곤을 찾음.
        for (int i = 0; i < spPoly.size() && !exit; i++) {
            int vSize = spPoly[i].size();
            // 폴리곤의 정점을 하나씩 탐색함.
            for (int j = 0; j < vSize; j++) {
                bool isEqual = false;
                
                // 정점이 선택되어 있으면, 현재 폴리곤의 정점과 비교함.
                if (_selectMode == VERTEX) {
                    isEqual = (*_selectedVertex).equals(spPoly[i].at(j));
                
                // 엣지가 선택되어 있으면, 현재 폴리곤의 정점 두개를 합쳐서 엣지랑 비교함.
                } else if (_selectMode == EDGE) {
                    auto from = *(*_selectedEdge).from, to = *(*_selectedEdge).to;
//                    isEqual = from.equals(spPoly[i].at(j)) && to.equals(spPoly[i].at((j + 1) % vSize));
                    isEqual = isEqualEdges(from, to, spPoly[i].at(j), spPoly[i].at((j + 1) % vSize));
                }
                
                // 포함되어 있는 폴리곤의 순서를 저장함.
                if (isEqual) {
                    exit = true;
                    index = i;
                    break;
                }
                
            }
        }
        
        if (true) {
            (*it)->runAction(RemoveSelf::create());
            it = _polygons.erase(it);
            
            CPolygon * p2 = CPolygon::create(spPoly[1 - index]);
            p2->draw(_frontColor, _borderColor, 1);
            this->addChild(p2);
            
            CPolygon * p1 = CPolygon::create(spPoly[index]);
            p1->symmetry(v1, v2);
            p1->draw(_backColor, _borderColor, 1);
            this->addChild(p1);
            
//            _polygons.push_back(p1);
//            _polygons.push_back(p2);
        } else it++;
    }
}
