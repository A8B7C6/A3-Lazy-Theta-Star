#include "script_component.hpp"

GVAR(HeightLimitString) = "";
GVAR(HeightLimit) = 0;
GVAR(SetHeightCancelled) = false;
GVAR(SetHeightUI) = false;

GVAR(StartPos) = [];
GVAR(GoalPos) = [];
GVAR(SetStartAndGoalCancelled) = false;
GVAR(SetPositionsUI) = false;

GVAR(ChartedPath) = [];
GVAR(MarkersCount) = 0;

if (!hasInterface) exitWith {};

player addAction ["Calculate LTS path",  {[] call FUNC(mainLTS);}, []];

/*
_mapSize2 = (configFile >> "CfgWorlds" >> worldName >> "MapSize") call BIS_fnc_getCfgData;
_gridsPerEdge2 = floor(_mapSize / GRID_SIZE) + 2;
_numGrids2 = _gridsPerEdge * _gridsPerEdge;

player addAction ["Get World Info", {call FUNC(getWorldInfo);}];
*/
