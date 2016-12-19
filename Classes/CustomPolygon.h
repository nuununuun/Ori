//
//  CustomPolygon.h
//  Ori
//
//  Created by a on 2016. 11. 27..
//
//

#ifndef CustomPolygon_h
#define CustomPolygon_h

#include "cocos2d.h"
#include "CustomDrawNode.h"
#include <vector>
#include <list>
#include <array>

class CustomPolygon : public cocos2d::Node {
private:
	struct Edge {
		Edge(const cocos2d::Vec2 &start, int side) :
			start(start),
			startSide(side),
			next(nullptr),
			prev(nullptr),
			dist(0.0f),
			visited(false) {
		}

		cocos2d::Vec2 start;
		int startSide;
		Edge *next;
		Edge *prev;
		float dist;
		bool visited;
	};

public:
    static CustomPolygon *create();
    static CustomPolygon *create(const std::vector<cocos2d::Vec2> &vertices);
    
    void drawPolygon();
    void drawPolygonPreview();
    
    void setVertices(const std::vector<cocos2d::Vec2> &vertices);
    void calculateEdges();

	void drawPreview();

	void symmetry();

	std::array<CustomPolygon *, 2> cutPolygon(const cocos2d::Vec2 &v1, const cocos2d::Vec2 &v2);
    
    std::vector<cocos2d::Vec2> vertices;
    std::vector<Edge> edges;
    std::vector<cocos2d::Vec2> previewVertices;
    CustomDrawNode *polygon;
    
    bool head = true;

private:
	int checkEdgeSide(const cocos2d::Vec2 &v1, const cocos2d::Vec2 &v2, const cocos2d::Vec2 &p);
	float calcSignedDistance(const cocos2d::Vec2 &v1, const cocos2d::Vec2 &v2, const cocos2d::Vec2 &p);

	cocos2d::Vec2 getSymmetricPoint(float a, float b, const cocos2d::Vec2 &p);

	std::list<Edge> splittedEdges;
	std::vector<Edge*> edgesOnLine; 
	std::array<CustomPolygon *, 2> previewData;
	bool isHaveSecondData = false;

	cocos2d::Vec2 cutStart, cutEnd;
    
};

#endif /* CustomPolygon_h */
