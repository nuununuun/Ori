#ifndef OBB_SUBDIV_HPP
#define OBB_SUBDIV_HPP

#include <array>
#include <vector>
#include <list>

#include "cocos2d.h"

#include "CPolygon.h"

typedef std::vector<std::vector<cocos2d::Vec2> > SplittedPolygon;
typedef std::vector<cocos2d::Vec2> Vertices;

enum class LineSide {
    On,
    Left,
    Right,
};

// splits convex and (!) concanve polygons along a line.
// all sorts of evil configurations tested, except things
// like non-manifold vertices, crossing edges and alike.
class PolySplitter {
public:
    struct PolyEdge {
        PolyEdge(const cocos2d::Vec2 &startPos, LineSide side) :
            StartPos(startPos),
            StartSide(side),
            Next(nullptr),
            Prev(nullptr),
            DistOnLine(0.0f),
            IsSrcEdge(false),
            IsDstEdge(false),
            Visited(false) {
        }

        cocos2d::Vec2       StartPos;   // start position on edge
        LineSide            StartSide;  // start position's side of split line
        PolyEdge *          Next;       // next polygon in linked list
        PolyEdge *          Prev;       // previous polygon in linked list
        float               DistOnLine; // distance relative to first point on split line
        bool                IsSrcEdge;  // for visualization
        bool                IsDstEdge;  // for visualization
        bool                Visited;    // for collecting split polygons
    };
    
public:
    static PolySplitter * getInstance();

public:
    SplittedPolygon Split(const Vertices &poly, const cocos2d::Vec2 &start, const cocos2d::Vec2 &end);

private:
    void            SplitEdges(const Vertices &poly, const cocos2d::Vec2 &start, const cocos2d::Vec2 &end);
    void            SortEdges(const cocos2d::Vec2 &start, const cocos2d::Vec2 &end);
    void            SplitPolygon();
    SplittedPolygon CollectPolys();
    void            VerifyCycles() const;
    void            CreateBridge(PolyEdge *srcEdge, PolyEdge *dstEdge);
    
    PolySplitter() {}
    static PolySplitter _instance;

public:
    std::list<PolyEdge> splitPoly;
    std::vector<PolyEdge *> edgesOnLine;
};

#endif
