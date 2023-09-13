#include "pch.h"
#include "LTSAlgo.hpp"

/*Implementation of struct GridMap*/


/*Implementation of struct PathingAlgo*/
LTSAlgo::PathingAlgo::PathingAlgo() {

}

LTSAlgo::cost LTSAlgo::PathingAlgo::calcDistance(const id& u, const id& v) const {
    std::valarray<int> uvDist{ nameToPos(u) - nameToPos(v) };

    return std::sqrt(((float)uvDist[0] * uvDist[0]) + ((float)uvDist[1] * uvDist[1]));
};

bool LTSAlgo::PathingAlgo::lineOfSight(const id& u, const id& v) const {
    std::valarray<int> uPos{ nameToPos(u) };
    std::valarray<int> vPos{ nameToPos(v) };

    std::valarray<int> uvDist{ vPos - uPos };
    std::valarray<int> uvDir(2);

    if (uvDist[0] >= 0) {
        uvDir[0] = 1;
    }
    else {
        uvDir[0] = -uvDir[0];
        uvDir[0] = -1;
    }

    if (uvDist[1] >= 0) {
        uvDir[1] = 1;
    }
    else {
        uvDir[1] = -uvDir[1];
        uvDir[1] = -1;
    }

    float cursorX{ ((float)uvDist[0] / uvDist[1] * uvDir[0] / 2) + uPos[0] + 0.5f };
    float cursorY{ ((float)uvDist[1] / uvDist[0] * uvDir[1] / 2) + uPos[1] + 0.5f };

    if (uvDist[0] >= uvDist[1]) {
        while (uPos[0] != vPos[0]) {
            if (hasObstacle[posToName(uPos)]) return false;
            if (abs(cursorY - uPos[1]) >= EPSILON) {
                uPos[1] += uvDir[1];
                if (hasObstacle[posToName(uPos)]) return false;
            }
            uPos[0] += uvDir[0];
            cursorY += (float)uvDist[1] / uvDist[0] * uvDir[1] / 2;
        }
    }
    else {
        while (uPos[1] != vPos[1]) {
            if (hasObstacle[posToName(uPos)]) return false;
            if (abs(cursorY - uPos[0]) >= EPSILON) {
                uPos[0] += uvDir[0];
                if (hasObstacle[posToName(uPos)]) return false;
            }
            uPos[1] += uvDir[1];
            cursorY += (float)uvDist[0] / uvDist[1] * uvDir[0] / 2;
        }
    }
    return true;
};

std::vector<std::pair<LTSAlgo::id, LTSAlgo::cost>> LTSAlgo::PathingAlgo::getNeighbors(const id &name) const{
    std::vector<std::pair<id, cost>> neighbors;

    std::valarray<int> uPos{ nameToPos(name) };
    id eastNeigh{ posToName({uPos[0] + 1, uPos[1]}) };
    if (uPos[0] != gridsPerEdge - 1 && !hasObstacle[eastNeigh]) {
        neighbors.push_back({ eastNeigh, 1.0f });
    }

    id westNeigh{ posToName({uPos[0] - 1, uPos[1]}) };
    if (uPos[0] != 0 && !hasObstacle[westNeigh]) {
        neighbors.push_back({ westNeigh, 1.0f });
    }
    
    id northNeigh{ posToName({uPos[0], uPos[1] + 1}) };
    if (uPos[1] != gridsPerEdge - 1 && !hasObstacle[northNeigh]) {
        neighbors.push_back({ northNeigh, 1.0f });
    }

    id southNeigh{ posToName({uPos[0], uPos[1] - 1}) };
    if (uPos[1] != 0 && !hasObstacle[southNeigh]) {
        neighbors.push_back({ southNeigh, 1.0f });
    }
    return neighbors;
};

LTSAlgo::id LTSAlgo::PathingAlgo::posToName(const std::valarray<int>& pos) const {
    return static_cast<id>(gridsPerEdge * pos[0]) + pos[1];
};

std::valarray<int> LTSAlgo::PathingAlgo::nameToPos(const id& name) const {
    int init[]{ static_cast<int>(name / gridsPerEdge), static_cast<int>(name % gridsPerEdge) };
    return std::valarray<int>(init, 2);
}

/*Implementation of LTSAlgo's remaining helper functions*/
/*public*/
LTSAlgo::LTSAlgo(PathingAlgo& pathAlgo, float heuristicWeight) : pathAlgo(pathAlgo), heuristicWeight(heuristicWeight) {
    initVertices();
}

std::vector<std::pair<int, int>> LTSAlgo::findPath(const id start, const id goal, const int gridsPerEdge) {
    openList.clear();
    
    numSearches++;
    initVertexState(start, goal);
    initVertexState(goal, goal);

    vertices[start].parent = start;
    vertices[start].gVal = 0;
    
    addToOpen(start);
    bool hasValidPath{ false };

    while (!openList.empty() && !hasValidPath) {
        id curr{ getMin().name };
        popMin();

        checkParent(curr, goal);

        for (const auto& neighborPair : vertices[curr].neighbors) {
            id newParent{ vertices[curr].parent };
            id neighbor{ neighborPair.first };
            initVertexState(neighbor, goal);

            if (vertices[neighbor].status != CLOSED) {
                cost newGVal{ vertices[newParent].gVal + pathAlgo.calcDistance(newParent, neighbor) };

                if (vertices[neighbor].gVal - EPSILON >= newGVal) {
                    vertices[neighbor].parent = newParent;
                    vertices[neighbor].gVal = newGVal;
                    addToOpen(neighbor);
                }
            }
        }

        if (curr == goal && pathAlgo.lineOfSight(vertices[curr].parent, curr)) {
            hasValidPath = true;
        }
    }

    std::vector<std::pair<int, int>> path;

    if (vertices[goal].gVal < MAX_COST) {
        id curr{ goal };
            
        while (curr != start) {
            path.push_back({ curr % gridsPerEdge, curr / gridsPerEdge });
            curr = vertices[curr].parent;
        }

        path.push_back({ curr % gridsPerEdge, curr / gridsPerEdge });
        std::reverse(path.begin(), path.end());
    }
    return path;
};

void LTSAlgo::initVertices() {
    vertices.clear();
    vertices.resize(pathAlgo.numGrids);

    int i { 0 };
    for (auto& vertex : vertices) {
        vertex.neighbors = pathAlgo.getNeighbors(i);
        i++;
    }
};

/*private*/

void LTSAlgo::initVertexState(const id name, const id goal) {
    if (vertices[name].lastSearched != numSearches) {
        vertices[name].status = NONE;
        vertices[name].lastSearched = numSearches;
        vertices[name].gVal = MAX_COST;
        vertices[name].cVal = heuristicWeight * pathAlgo.calcDistance(name, goal);
    }
};

void LTSAlgo::checkParent(const id name, const id goal) {
    if (!pathAlgo.lineOfSight(vertices[name].parent, name)) {
        vertices[name].gVal = MAX_COST;

        for (const auto& neighborPair : vertices[name].neighbors) {
            id newParent{ neighborPair.first };
            initVertexState(newParent, goal);

            if (vertices[newParent].status == CLOSED) {
                float newGVal{ vertices[newParent].gVal + neighborPair.second };

                if (newGVal < vertices[name].gVal) {
                    vertices[name].parent = newParent;
                    vertices[name].gVal = newGVal;
                }
            }
        }
    }
};

void LTSAlgo::addToOpen(const id name) {
    if (vertices[name].status == OPEN) {
        for (size_t i{ 0 }; i < openList.size(); i++) {
            if (openList[i].name == name) {
                openList.erase(openList.begin() + i);
                heapInsert(openList, { name, vertices[name].gVal, vertices[name].gVal + vertices[name].cVal });
                break;
            }
        }
    }
    else {
        vertices[name].status = OPEN;
        heapInsert(openList, { name, vertices[name].gVal, vertices[name].gVal + vertices[name].cVal });
    }
};

LTSAlgo::HeapElement LTSAlgo::getMin() const {
    return openList.back();
};

void LTSAlgo::popMin() {
    vertices[openList.back().name].status = CLOSED;
    openList.pop_back();
};

std::vector<LTSAlgo::HeapElement>::iterator LTSAlgo::heapInsert
    (std::vector<LTSAlgo::HeapElement>& heap, const LTSAlgo::HeapElement& element) {
    return heap.insert(std::upper_bound(heap.begin(), heap.end(), element), element);
};
