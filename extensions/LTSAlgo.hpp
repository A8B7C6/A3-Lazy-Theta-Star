#ifndef LAZY_THETA_STAR_H
#define LAZY_THETA_STAR_H

#include "pch.h"

class LTSAlgo {
public:
    static constexpr float EPSILON { 0.00001f };
    static constexpr float MAX_COST { FLT_MAX };
    typedef uint32_t id;
    typedef float cost;

    struct PathingAlgo {
        friend LTSAlgo;
        //std::vector<int> mapHeights;
        std::vector<bool> hasObstacle;
        int gridSize { -1 };
        int gridsPerEdge { -1 };
        int numGrids { -1 };
        id start { 0 };
        id goal { 0 };

        cost calcDistance(const id& u, const id& v) const;

        bool lineOfSight(const id& u, const id& v) const;

        std::vector<std::pair<id, cost>> getNeighbors(const id &name) const;

        id posToName(const std::valarray<int> &pos) const;

        std::valarray<int> nameToPos(const id& name) const;

    };

    //void initPathingAlgo(PathingAlgo pathAlgo);

    LTSAlgo(PathingAlgo& pathAlgo, float heuristicWeight);

    std::vector<std::pair<int, int>> findPath(const id start, const id goal, const int gridsPerEdge);

    void initVertices();

private:
    enum ListStatus { OPEN, CLOSED, NONE };

    struct Vertex {
        std::vector<std::pair<id, cost>> neighbors;
        ListStatus status;
        id parent;
        int lastSearched;
        cost gVal;
        cost cVal;
    };

    struct HeapElement {
        id name;
        cost gVal;
        cost fVal;

        bool operator < (const HeapElement& rhs) const {
            if (abs(fVal - rhs.fVal) >= EPSILON) {
                return fVal > rhs.fVal;
            }
            else return gVal < rhs.gVal;
        };
    };

    int numSearches{ 0 };
    const float heuristicWeight;
    std::vector<Vertex> vertices;
    std::vector<HeapElement> openList;
    PathingAlgo &pathAlgo;

    void initVertexState(const id name, const id goal);

    void checkParent(const id name, const id goal);

    void addToOpen(const id name);

    HeapElement getMin() const;

    void popMin();

    std::vector<HeapElement>::iterator heapInsert
        (std::vector<HeapElement>& list, const HeapElement& element);
};

#endif