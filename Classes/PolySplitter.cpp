#include <iterator>
#include <cassert>
#include <queue>

#include "PolySplitter.hpp"

USING_NS_CC;

static LineSide GetSideOfLine(const Vec2 &start, const Vec2 &end, const Vec2 &pt) {
    const float d = (pt.x - start.x) * (end.y - start.y) - (pt.y - start.y) * (end.x - start.x);
    return (d > 0.1f ? LineSide::Right : (d < -0.1f ? LineSide::Left : LineSide::On));
}

static double CalcSignedDistance(const Vec2 &start, const Vec2 &end, const Vec2 &p) {
    // scalar projection on line. in case of co-linear
    // vectors this is equal to the signed distance.
    return (p.x - start.x) * (end.x - start.x) + (p.y - start.y) * (end.y - start.y);
}

// -----------------------------------------------------------------------

PolySplitter PolySplitter::_instance = PolySplitter();

PolySplitter * PolySplitter::getInstance() {
    return &_instance;
}

SplittedPolygon PolySplitter::Split(const Vertices &poly, const Vec2 &start, const Vec2 &end) {
    SplitEdges(poly, start, end);
    SortEdges(start, end);
    SplitPolygon();
    return CollectPolys();
}

void PolySplitter::SplitEdges(const Vertices &poly, const Vec2 &start, const Vec2 &end) {
    splitPoly.clear();
    edgesOnLine.clear();

    for (int i = 0; i < poly.size(); i++) {
        const LineSide edgeStartSide = GetSideOfLine(start, end, poly[i]);
        const LineSide edgeEndSide = GetSideOfLine(start, end, poly[(i + 1) % poly.size()]);
        splitPoly.push_back(PolyEdge{ poly[i], edgeStartSide });

        if (edgeStartSide == LineSide::On) {
            edgesOnLine.push_back(&splitPoly.back());
        } else if (edgeStartSide != edgeEndSide && edgeEndSide != LineSide::On) {
            if (Vec2::isSegmentIntersect(poly[i], poly[(i + 1) % poly.size()], start, end)) {
                Vec2 ip = Vec2::getIntersectPoint(poly[i], poly[(i + 1) % poly.size()], start, end);
                splitPoly.push_back(PolyEdge{ ip, LineSide::On });
                edgesOnLine.push_back(&splitPoly.back());
            }
        }
    }

    // connect doubly linked list, except
    // first->prev and last->next
    for (auto iter = splitPoly.begin(); iter != std::prev(splitPoly.end()); iter++) {
        auto nextIter = std::next(iter);
        iter->Next = &(*nextIter);
        nextIter->Prev = &(*iter);
    }

    // connect first->prev and last->next
    splitPoly.back().Next = &splitPoly.front();
    splitPoly.front().Prev = &splitPoly.back();
}

void PolySplitter::SortEdges(const Vec2 &start, const Vec2 &end) {
    // sort edges by start position relative to
    // the start position of the split line
    std::sort(edgesOnLine.begin(), edgesOnLine.end(), [&](PolyEdge *e0, PolyEdge *e1) {
        // it's important to take the signed distance here,
        // because it can happen that the split line starts/ends
        // inside the polygon. in that case intersection points
        // can fall on both sides of the split line and taking
        // an unsigned distance metric will result in wrongly
        // ordered points in EdgesOnLine.
        return CalcSignedDistance(start, end, e0->StartPos) < CalcSignedDistance(start, end, e1->StartPos);
    });

    // compute distance between each edge's start
    // position and the first edge's start position
    for (size_t i = 1; i < edgesOnLine.size(); i++)
        edgesOnLine[i]->DistOnLine = (edgesOnLine[i]->StartPos - edgesOnLine[0]->StartPos).getLength();
}

void PolySplitter::SplitPolygon() {
    PolyEdge *useSrc = nullptr;

    for (size_t i = 0; i < edgesOnLine.size(); i++) {
        // find source
        PolyEdge *srcEdge = useSrc;
        useSrc = nullptr;

        for (; !srcEdge && i < edgesOnLine.size(); i++) {
            PolyEdge *curEdge = edgesOnLine[i];
            const auto curSide = curEdge->StartSide;
            const auto prevSide = curEdge->Prev->StartSide;
            const auto nextSide = curEdge->Next->StartSide;
            assert(curSide == LineSide::On);

            if ((prevSide == LineSide::Left && nextSide == LineSide::Right) ||
                (prevSide == LineSide::Left && nextSide == LineSide::On && curEdge->Next->DistOnLine < curEdge->DistOnLine) ||
                (prevSide == LineSide::On && nextSide == LineSide::Right && curEdge->Prev->DistOnLine < curEdge->DistOnLine)) {
                srcEdge = curEdge;
                srcEdge->IsSrcEdge = true;
            }
        }

        // find destination
        PolyEdge *dstEdge = nullptr;

        for (; !dstEdge && i < edgesOnLine.size(); ) {
            PolyEdge *curEdge = edgesOnLine[i];
            const auto curSide = curEdge->StartSide;
            const auto prevSide = curEdge->Prev->StartSide;
            const auto nextSide = curEdge->Next->StartSide;
            assert(curSide == LineSide::On);

            if ((prevSide == LineSide::Right && nextSide == LineSide::Left)  ||
                (prevSide == LineSide::On && nextSide == LineSide::Left)     ||
                (prevSide == LineSide::Right && nextSide == LineSide::On)    ||
                (prevSide == LineSide::Right && nextSide == LineSide::Right) ||
                (prevSide == LineSide::Left && nextSide == LineSide::Left)) {
                dstEdge = curEdge;
                dstEdge->IsDstEdge = true;
            }
            else
                i++;
        }

        // bridge source and destination
        if (srcEdge && dstEdge) {
            CreateBridge(srcEdge, dstEdge);
            VerifyCycles();

            // is it a configuration in which a vertex
            // needs to be reused as source vertex?
            if (srcEdge->Prev->Prev->StartSide == LineSide::Left) {
                useSrc = srcEdge->Prev;
                useSrc->IsSrcEdge = true;
            } else if (dstEdge->Next->StartSide == LineSide::Right) {
                useSrc = dstEdge;
                useSrc->IsSrcEdge = true;
            }
        }
    }
}

SplittedPolygon PolySplitter::CollectPolys() {
    SplittedPolygon resPolys;

    for (auto &e : splitPoly) {
        if (!e.Visited) {
            std::vector<Vec2> poly;
            auto *curSide = &e;

            do {
                curSide->Visited = true;
                poly.push_back(curSide->StartPos);
                curSide = curSide->Next;
            } while (curSide != &e);

            resPolys.push_back(poly);
        }
    }

    return resPolys;
}

void PolySplitter::VerifyCycles() const {
    for (auto &edge : splitPoly) {
        const auto *curSide = &edge;
        size_t count = 0;

        do {
            assert(count < splitPoly.size());
            curSide = curSide->Next;
            count++;
        } while (curSide != &edge);
    }
}

void PolySplitter::CreateBridge(PolyEdge *srcEdge, PolyEdge *dstEdge) {
    splitPoly.push_back(*srcEdge);
    PolyEdge *a = &splitPoly.back();
    splitPoly.push_back(*dstEdge);
    PolyEdge *b = &splitPoly.back();
    a->Next = dstEdge;
    a->Prev = srcEdge->Prev;
    b->Next = srcEdge;
    b->Prev = dstEdge->Prev;
    srcEdge->Prev->Next = a;
    srcEdge->Prev = b;
    dstEdge->Prev->Next = b;
    dstEdge->Prev = a;
}
