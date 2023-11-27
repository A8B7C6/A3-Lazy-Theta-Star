#include "script_component.hpp"

if (!hasInterface) exitWith {};

private _mapSize = worldSize;
private _gridsPerEdge = ceil(_mapSize / GRID_SIZE);
private _numGrids = _gridsPerEdge * _gridsPerEdge;

hintSilent format["GPE: %1, NG: %2", _gridsPerEdge, _numGrids];
//sleep(1);
"lazyThetaStar" callExtension format["init:%1:%2:%3", GRID_SIZE, _gridsPerEdge, _numGrids];

private _stepSize = 25; //_stepSize should be a factor of GRID_SIZE
for "_i" from 0 to _gridsPerEdge - 1 do {
	private _xi = GRID_SIZE * _i;
	for "_j" from 0 to _gridsPerEdge - 1 do {
		private _yj = GRID_SIZE * _j;
		private _hasObstacle = 0;

		if((_i == 0) || (_i == _gridsPerEdge - 1)) then {
			_hasObstacle = 1;
			"LazyThetaStar" callExtension format["set:%1", _hasObstacle];
			continue;
		};
		if((_j == 0) || (_j == _gridsPerEdge - 1)) then {
			_hasObstacle = 1;
			"LazyThetaStar" callExtension format["set:%1", _hasObstacle];
			continue;
		};
		
		for "_k" from 0 to (GRID_SIZE - _stepSize) step _stepSize do {
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

		"lazyThetaStar" callExtension format["set:%1", _hasObstacle];
	};
};

//sleep(1);
hintSilent "Terrain data collection done";
//sleep(1);
