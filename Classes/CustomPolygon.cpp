//
//  CustomPolygon.cpp
//  Ori
//
//  Created by a on 2016. 11. 27..
//
//

#include "CustomPolygon.h"

USING_NS_CC;
using namespace std;

CustomPolygon *CustomPolygon::create() {
    CustomPolygon * ret = new (std::nothrow) CustomPolygon();
    if (ret && ret->init())
    {
        Vec2 center = Director::getInstance()->getVisibleSize() / 2;
        
        ret->autorelease();
        
        ret->polygon = CustomDrawNode::create();
        ret->polygon->setPosition(center);
        
        ret->addChild(ret->polygon);
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

CustomPolygon *CustomPolygon::create(vector<cocos2d::Vec2> v) {
    CustomPolygon * ret = new (std::nothrow) CustomPolygon();
    if (ret && ret->init())
    {
        Vec2 center = Director::getInstance()->getVisibleSize() / 2;
        
        ret->autorelease();
        
        ret->polygon = CustomDrawNode::create();
        ret->polygon->setPosition(center);
        
        ret->addChild(ret->polygon);
        
		ret->setVertices(v);

		for (int i = 0; i < 2; i++) {
			ret->previewData[i] = CustomPolygon::create();
			ret->previewData[i]->setVisible(false);
			ret->addChild(ret->previewData[i]);
		}
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void CustomPolygon::setVertices(vector<Vec2> v)
{
    vertices = v;
	calculateEdges();
}

void CustomPolygon::calculateEdges() {
    for (int i = 0; i < vertices.size(); i++) {
        edges.push_back(Edge(vertices[i], 0));
    }
}

void CustomPolygon::symmetry() {
	float a = tan((cutEnd - cutStart).getAngle());
	float b = -a * cutStart.x + cutStart.y;

	for (int i = 0; i < vertices.size(); i++) {
		Vec2 sym = getSymmetricPoint(a, b, vertices[i]);
		vertices[i] = Vec2(round(sym.x), round(sym.y));
	}
}

void CustomPolygon::drawPreview() {
	polygon->setVisible(false);

	previewData[0]->setVisible(true);
	previewData[0]->drawPolygon();

	if (previewData[1]->vertices.size() > 0) {
		previewData[1]->setVisible(true);
		previewData[1]->head = false;
		previewData[1]->symmetry();
		previewData[1]->drawPolygon();
	} else {
		previewData[1]->polygon->clear();
	}
}

Vec2 CustomPolygon::getSymmetricPoint(float a, float b, const Vec2 &p) {
	return Vec2((2 * a * p.y - (a * a - 1) * p.x - 2 * a * b) / (a * a + 1), (2 * a * p.x + (a * a - 1) * p.y + 2 * b) / (a * a + 1));
}

// 만드??�?.
void CustomPolygon::splitPolygon(const cocos2d::Vec2 &v1, const cocos2d::Vec2 &v2, const cocos2d::Vec2 &p1, const cocos2d::Vec2 &p2) {
    vector<Vec2> splitVertices[2];
    
    polygon->clear();
    
    for (int i = 0; i < vertices.size(); i++) {
        auto interVert = Vec2::getIntersectPoint(vertices[(i + 1) % vertices.size()], vertices[i], v2, v1);
        
        
        if (Vec2::isSegmentIntersect(vertices[(i + 1) % vertices.size()], vertices[i], v2, v1)) { // 겹치???�이 ?�으�?
            splitVertices[0].push_back(interVert);
            splitVertices[1].push_back(interVert);
            
            polygon->drawPoint(interVert, 12, Color4F::BLUE);
        }
    }
    
    if (splitVertices[0].size() > 1) {
        auto center = (splitVertices[0].at(0) + splitVertices[0].at(1)) / 2;
        float centerAngle = CC_RADIANS_TO_DEGREES((v1 - v2).getAngle());
        float corr = 0;
        
        if (centerAngle > 90) {
            corr = 180 - centerAngle;
        } else {
            corr = -centerAngle;
        }
        
        for (int i = 0; i < vertices.size(); i++) {
            float angle = (CC_RADIANS_TO_DEGREES((center - vertices[i]).getAngle()) + 180 + corr); // 0 ~ 360?�로 만듦
            if (angle >= 360) angle -= 360;
            
            if (angle > 180) {
                splitVertices[0].push_back(vertices[i]);
            } else {
                splitVertices[1].push_back(vertices[i]);
            }
        }
        
        for (auto i : splitVertices[0]) {
            polygon->drawPoint(i, 16, Color4F::GRAY);
        }
        
        for (auto i : splitVertices[1]) {
            polygon->drawPoint(i, 16, Color4F::YELLOW);
        }
        
        for (int i = 0; i < 2; i++) {
            sort(splitVertices[i].begin(), splitVertices[i].end(), [&](const Vec2 &v1, const Vec2 &v2)->bool {
                float angle1 = (CC_RADIANS_TO_DEGREES((center - v1).getAngle()) + 180 + corr); // 0 ~ 360?�로 만듦
                if (angle1 >= 360) angle1 -= 360;
            
                float angle2 = (CC_RADIANS_TO_DEGREES((center - v2).getAngle()) + 180 + corr); // 0 ~ 360?�로 만듦
                if (angle2 >= 360) angle2 -= 360;
            
                return angle1 < angle2;
            });
            previewData[i]->setVertices(splitVertices[i]);
            previewData[i]->cutStart = cutStart;
            previewData[i]->cutEnd = cutEnd;
        }
    }
}

array<CustomPolygon *, 2> CustomPolygon::cutPolygon(int action, const Vec2 & v1, const Vec2 & v2, const cocos2d::Vec2 &p1, const cocos2d::Vec2 &p2) {
    auto ret = array<CustomPolygon*, 2>();

	splittedEdges.clear();
	edgesOnLine.clear();

	/// øß¡? ?�∏µÈ±??
	for (int i = 0; i < vertices.size(); i++) {
		int edgeStartSide = checkEdgeSide(v1, v2, vertices[i]);
		int edgeEndSide = checkEdgeSide(v1, v2, vertices[(i + 1) % vertices.size()]);
		splittedEdges.push_back(Edge(vertices[i], edgeStartSide));

		if (edgeStartSide == 0) {
			edgesOnLine.push_back(&splittedEdges.back());
		} else if (edgeStartSide != edgeEndSide && edgeEndSide != 0) {
			auto contact = Vec2::getIntersectPoint(vertices[(i + 1) % vertices.size()], vertices[i], v2, v1);
			splittedEdges.push_back	(Edge(contact, 0));
			edgesOnLine.push_back(&splittedEdges.back());
		}
	}

	for (auto iter = splittedEdges.begin(); iter != std::prev(splittedEdges.end()); iter++) {
		auto ni = std::next(iter);
		iter->next = &(*ni);
		ni->prev = &(*iter);
	}

	splittedEdges.back().next = &splittedEdges.front();
	splittedEdges.front().prev = &splittedEdges.back();

	/// øß¡? ¡§?�ƒ«œ±�?
	std::sort(edgesOnLine.begin(), edgesOnLine.end(), [&](Edge *e1, Edge *e2) {
		return calcSignedDistance(v1, v2, e1->start) <= calcSignedDistance(v1, v2, e2->start);
	});

	for (int i = 1; i < edgesOnLine.size(); i++) {
		edgesOnLine[i]->dist = edgesOnLine[i]->start.getDistance(edgesOnLine[0]->start);
	}

	/// ¥Ÿ?��??�∏µÈ±??
	Edge *useSrc = nullptr;
	for (int i = 0; i < edgesOnLine.size(); i++) {

		Edge *srcEdge = useSrc;
		useSrc = nullptr;
		for (; !srcEdge && i < edgesOnLine.size(); i++) {
			Edge *curEdge = edgesOnLine[i];
			const auto curSide = curEdge->startSide;
			const auto prevSide = curEdge->prev->startSide;
			const auto nextSide = curEdge->next->startSide;

			if ((prevSide == -1 && nextSide == 1) ||
				(prevSide == -1 && nextSide == 0 && curEdge->next->dist < curEdge->dist) ||
				(prevSide == 0 && nextSide == 1 && curEdge->prev->dist < curEdge->dist)) {
				srcEdge = curEdge;
			}
		}

		Edge *dstEdge = nullptr;
		for (; !dstEdge && i < edgesOnLine.size(); ) {
			Edge *curEdge = edgesOnLine[i];
			const auto curSide = curEdge->startSide;
			const auto prevSide = curEdge->prev->startSide;
			const auto nextSide = curEdge->next->startSide;

			if ((prevSide == 1 && nextSide == -1) ||
				(prevSide == 0 && nextSide == -1) ||
				(prevSide == 1 && nextSide == 0) ||
				(prevSide == 1 && nextSide == 1) ||
				(prevSide == -1 && nextSide == -1)) {
				dstEdge = curEdge;
			}
			else
				i++;
		}

		if (srcEdge && dstEdge) {
			splittedEdges.push_back(*srcEdge);
			Edge *a = &splittedEdges.back();
			splittedEdges.push_back(*dstEdge);
			Edge *b = &splittedEdges.back();
			a->next = dstEdge;
			a->prev = srcEdge->prev;
			b->next = srcEdge;
			b->prev = dstEdge->prev;
			srcEdge->prev->next = a;
			srcEdge->prev = b;
			dstEdge->prev->next = b;
			dstEdge->prev = a;

			cutStart = a->start;
			cutEnd = b->start;

			if (srcEdge->prev->prev->startSide == -1) {
				useSrc = srcEdge->prev;
			} else if (dstEdge->next->startSide == 1) {
				useSrc = dstEdge;
			}
		}
	}
    
    int idx = 0;
    
    if (action == 0) idx = 1;
    if (action == 1) idx = 1;
    
	for (auto &i : splittedEdges) {
		if (!i.visited) {
			auto *curSide = &i;
            vector<Vec2> vert;
			do {
				curSide->visited = true;
				vert.push_back(curSide->start);
				curSide = curSide->next;
            } while (curSide != &i);
            
            CustomPolygon *polygon = CustomPolygon::create(vert);
			ret[idx] = polygon;
			ret[idx]->cutStart = cutStart;
			ret[idx]->cutEnd = cutEnd;
			idx = (idx + 1) % 2;
		}
	}

	for (int i = 0; i < 2; i++) {
		if (ret[i] != nullptr) {
			previewData[i]->setVertices(ret[i]->vertices);
			previewData[i]->cutStart = ret[i]->cutStart;
			previewData[i]->cutEnd = ret[i]->cutEnd;
		}
	}

	return ret;
}

void CustomPolygon::cutPolygonPreview(int action, const Vec2 & v1, const Vec2 & v2, const cocos2d::Vec2 &p1, const cocos2d::Vec2 &p2) {
    splittedEdges.clear();
    edgesOnLine.clear();
    
    /// øß¡? ?�∏µÈ±??
    for (int i = 0; i < vertices.size(); i++) {
        int edgeStartSide = checkEdgeSide(v1, v2, vertices[i]);
        int edgeEndSide = checkEdgeSide(v1, v2, vertices[(i + 1) % vertices.size()]);
        splittedEdges.push_back(Edge(vertices[i], edgeStartSide));
        
        if (edgeStartSide == 0) {
            edgesOnLine.push_back(&splittedEdges.back());
        } else if (edgeStartSide != edgeEndSide && edgeEndSide != 0) {
            auto contact = Vec2::getIntersectPoint(vertices[(i + 1) % vertices.size()], vertices[i], v2, v1);
            splittedEdges.push_back	(Edge(contact, 0));
            edgesOnLine.push_back(&splittedEdges.back());
        }
    }
    
    for (auto iter = splittedEdges.begin(); iter != std::prev(splittedEdges.end()); iter++) {
        auto ni = std::next(iter);
        iter->next = &(*ni);
        ni->prev = &(*iter);
    }
    
    splittedEdges.back().next = &splittedEdges.front();
    splittedEdges.front().prev = &splittedEdges.back();
    
    /// øß¡? ¡§?�ƒ«œ±�?
    std::sort(edgesOnLine.begin(), edgesOnLine.end(), [&](Edge *e1, Edge *e2) {
        return calcSignedDistance(v1, v2, e1->start) <= calcSignedDistance(v1, v2, e2->start);
    });
    
    for (int i = 1; i < edgesOnLine.size(); i++) {
        edgesOnLine[i]->dist = edgesOnLine[i]->start.getDistance(edgesOnLine[0]->start);
    }
    
    /// ¥Ÿ?��??�∏µÈ±??
    Edge *useSrc = nullptr;
    for (int i = 0; i < edgesOnLine.size(); i++) {
        
        Edge *srcEdge = useSrc;
        useSrc = nullptr;
        for (; !srcEdge && i < edgesOnLine.size(); i++) {
            Edge *curEdge = edgesOnLine[i];
            const auto curSide = curEdge->startSide;
            const auto prevSide = curEdge->prev->startSide;
            const auto nextSide = curEdge->next->startSide;
            
            if ((prevSide == -1 && nextSide == 1) ||
                (prevSide == -1 && nextSide == 0 && curEdge->next->dist < curEdge->dist) ||
                (prevSide == 0 && nextSide == 1 && curEdge->prev->dist < curEdge->dist)) {
                srcEdge = curEdge;
            }
        }
        
        Edge *dstEdge = nullptr;
        for (; !dstEdge && i < edgesOnLine.size(); ) {
            Edge *curEdge = edgesOnLine[i];
            const auto curSide = curEdge->startSide;
            const auto prevSide = curEdge->prev->startSide;
            const auto nextSide = curEdge->next->startSide;
            
            if ((prevSide == 1 && nextSide == -1) ||
                (prevSide == 0 && nextSide == -1) ||
                (prevSide == 1 && nextSide == 0) ||
                (prevSide == 1 && nextSide == 1) ||
                (prevSide == -1 && nextSide == -1)) {
                dstEdge = curEdge;
            }
            else
                i++;
        }
        
        if (srcEdge && dstEdge) {
            splittedEdges.push_back(*srcEdge);
            Edge *a = &splittedEdges.back();
            splittedEdges.push_back(*dstEdge);
            Edge *b = &splittedEdges.back();
            a->next = dstEdge;
            a->prev = srcEdge->prev;
            b->next = srcEdge;
            b->prev = dstEdge->prev;
            srcEdge->prev->next = a;
            srcEdge->prev = b;
            dstEdge->prev->next = b;
            dstEdge->prev = a;
            
            cutStart = a->start;
            cutEnd = b->start;
            
            if (srcEdge->prev->prev->startSide == -1) {
                useSrc = srcEdge->prev;
            } else if (dstEdge->next->startSide == 1) {
                useSrc = dstEdge;
            }
        }
    }
    
    int idx = 0;
    
        if (action == 0) idx = 1;
        if (action == 1) idx = 1;
    
    vector<Vec2> polygons[2];
    
    for (auto &i : splittedEdges) {
        if (!i.visited) {
            auto *curSide = &i;
            
            do {
                curSide->visited = true;
                polygons[idx].push_back(curSide->start);
                curSide = curSide->next;
            } while (curSide != &i);
            idx = (idx + 1) % 2;
        }
    }
    
    for (int i = 0; i < 2; i++) {
        previewData[i]->setVertices(polygons[i]);
        previewData[i]->cutStart = cutStart;
        previewData[i]->cutEnd = cutEnd;
    }
}

/// Right = 1, Left = -1, On = 0
int CustomPolygon::checkEdgeSide(const cocos2d::Vec2 &v1, const cocos2d::Vec2 &v2, const Vec2 &p) {
    const float d = (p.x - v1.x) * (v2.y - v1.y) - (p.y - v1.y) * (v2.x - v1.x);
	return (d > 0.1f ? 1 : (d < -0.1f ? -1 : 0));
}

float CustomPolygon::calcSignedDistance(const cocos2d::Vec2 &v1, const cocos2d::Vec2 &v2, const cocos2d::Vec2 &p) {
	return (p.x - v1.x) * (v2.x - v1.x) + (p.y - v1.y) * (v2.y - v1.y);
}

void CustomPolygon::drawPolygon() {
    polygon->clear();
    if (head)
        polygon->drawPolygon(vertices, Color4B(255, 77, 77, 255), 1, Color4B(196, 64, 64, 255));
    else
        polygon->drawPolygon(vertices, Color4B(255, 100, 91, 255), 1, Color4B(196, 64, 64, 255));
}

void CustomPolygon::drawPolygonPreview() {
    polygon->clear();
    if (head)
        polygon->drawPolygon(previewVertices, Color4B(250, 32, 24, 255), 0.4, Color4B(80, 80, 80, 255));
    else
        polygon->drawPolygon(previewVertices, Color4B(255, 90, 72, 255), 0.4, Color4B(80, 80, 80, 255));
}
