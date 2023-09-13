#include "script_component.hpp"

if (!hasInterface) exitWith {};

private _mapSize = (configFile >> "CfgWorlds" >> worldName >> "MapSize") call BIS_fnc_getCfgData;
private _gridsPerEdge = floor(_mapSize / GRID_SIZE) + 2;
private _numGrids = _gridsPerEdge * _gridsPerEdge;

hintSilent "GWI checkpoint 0";
sleep(1);
"lazyThetaStar" callExtension format["init:%1:%2:%3", GRID_SIZE, _gridsPerEdge, _numGrids];
sleep(1);
hintSilent "GWI checkpoint 1";

private _stepSize = 5;
for "_i" from 0 to _gridsPerEdge do {
	private _xi = GRID_SIZE * "_i";
	for "_j" from 0 to _gridsPerEdge do {
		private _yj = GRID_SIZE * "_j";
		private _hasObstacle = 0;

		if((_i == 0) || (_i == _gridsPerEdge)) then {
			_hasObstacle = 1;
			"LazyThetaStar" callExtension format["set:%1", _hasObstacle];
			continue;
		};
		if((_j == 0) || (_j == _gridsPerEdge)) then {
			_hasObstacle = 1;
			"LazyThetaStar" callExtension format["set:%1", _hasObstacle];
			continue;
		};
		
		for "_k" from 0 to _gridSize step _stepSize do {
			if(getTerrainHeightASL[_xi + _k, _yj] >= GVAR(HeightLimit)) then {
				_hasObstacle = 1;
				break;
			};
			if(getTerrainHeightASL[_xi, _yj + _k] >= GVAR(HeightLimit)) then {
				_hasObstacle = 1;
				break;
			};
			if(getTerrainHeightASL[_xi + _k, _yj + (GRID_SIZE/2)] >= GVAR(HeightLimit)) then {
				_hasObstacle = 1;
				break;
			};
			if(getTerrainHeightASL[_xi + (GRID_SIZE/2), _yj + _k] >= GVAR(HeightLimit)) then {
				_hasObstacle = 1;
				break;
			};
		};
		
		sleep(1);
		hintSilent "GWI checkpoint 2";
		"lazyThetaStar" callExtension format["set:%1", _hasObstacle];
	};
};

hintSilent "Terrain data collection done";
