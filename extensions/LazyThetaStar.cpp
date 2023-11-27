/*
init: retrieve edge length of map, number of grids per edge (A3 maps are
squares), and total number of grids including padded walls

set: retrieve the ASL height of each grid's corner, and determine whether if an
obstacle (terrain feature with ASL height >= height limit) exists in each grid

calc: evaluates the LTS path between starting location and goal location
 */

#include "pch.h"
#include "LTSAlgo.hpp"

std::vector<std::string> contextInfo;

const float WEIGHT{ 100.0f };

LTSAlgo::PathingAlgo pathAlgo;

/* HELPER FUNCTIONS */
//Implement pathfinding algorithm helper stuff here

/* MAIN BODY OF EXTENSION */
extern "C" {
//--- User entry point
__declspec(dllexport) void __stdcall RVExtension(char *output, int outputSize, const char *function);
//--- Engine passed context
__declspec(dllexport) void __stdcall RVExtensionContext(const char **args, int argsCnt);
};

//--- name callExtension function
void __stdcall RVExtension(char *output, int outputSize, const char *function) {
  char *input{_strdup(function)};
  const char *delim{":"};
  char *nextTok{nullptr};
  char *mode{ strtok_s(input, delim, &nextTok)};

  std::stringstream ss;

  if (strcmp(mode, "init") == 0) {
    int gridSize{std::stoi(strtok_s(nullptr, ":", &nextTok), nullptr)};
    int gridsPerEdge{std::stoi(strtok_s(nullptr, ":", &nextTok), nullptr)};
    int numGrids{std::stoi(strtok_s(nullptr, ":", &nextTok), nullptr)};

    pathAlgo.gridSize = gridSize;
    pathAlgo.gridsPerEdge = gridsPerEdge;
    pathAlgo.numGrids = numGrids;

    strncpy_s(output, outputSize, ss.str().c_str(), _TRUNCATE);
    return;

  } else if (strcmp(mode, "set") == 0) {
    //int originHeight{std::stoi(strtok_s(nullptr, ":", &nextTok), nullptr)};
    int hasObstacle{std::stoi(strtok_s(nullptr, ":", &nextTok), nullptr)};

    //pathfinder->mapHeights.push_back(originHeight);
    pathAlgo.hasObstacle.push_back(hasObstacle);
    
    strncpy_s(output, outputSize, ss.str().c_str(), _TRUNCATE);
    return;

  } else if (strcmp(mode, "calc") == 0) {
    //int heightLimit{std::stoi(strtok_s(nullptr, ":", &nextTok), nullptr)};
    LTSAlgo::id startLoc{(LTSAlgo::id)std::stoi(strtok_s(nullptr, ":", &nextTok), nullptr)};
    LTSAlgo::id goalLoc{(LTSAlgo::id)std::stoi(strtok_s(nullptr, ":", &nextTok), nullptr)};

    pathAlgo.start = startLoc;
    pathAlgo.goal = goalLoc;

    LTSAlgo LTS(pathAlgo, WEIGHT);

    std::vector<std::pair<int, int>> path = LTS.findPath(pathAlgo.start, pathAlgo.goal, pathAlgo.gridsPerEdge);
    
    bool firstPairPassed{ false };
    ss << "[";
    for (auto& posPair : path) {
        if (firstPairPassed) { ss << ","; }
        firstPairPassed = true;

        posPair.first *= pathAlgo.gridSize;
        posPair.second *= pathAlgo.gridSize;

        ss << "[" << posPair.first << "," << posPair.second << "]";
    }
    ss << "]";

    strncpy_s(output, outputSize, ss.str().c_str(),  _TRUNCATE);
    return;
  }
  strncpy_s(output, outputSize, ss.str().c_str(), _TRUNCATE);
  return;
};

//--- Context is executed first, copy it
void __stdcall RVExtensionContext(const char **args, int argsCnt) {
  contextInfo.assign(args, std::next(args, argsCnt));
};
