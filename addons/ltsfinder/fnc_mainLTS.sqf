#include "script_component.hpp"
/*

*/

//Implement SetHeight
GVAR(SetHeightCancelled) = false;
GVAR(SetHeightUI) = true;

call FUNC(createHeightDialog);
//((uiNamespace getVariable "RscSetHeight") displayCtrl 1400) ctrlSetText GVAR(HeightLimitString);
waitUntil{!GVAR(SetHeightUI);};

if(!GVAR(SetHeightCancelled)) then {
	//Ask player for start and end points
	GVAR(SetStartAndGoalCancelled) = false;
	GVAR(SetPositionsUI) = true;

	call FUNC(setStartAndGoal);
	waitUntil{!GVAR(SetPositionsUI);};

	if(!GVAR(SetStartAndGoalCancelled)) then {

		call FUNC(getWorldInfo);

		private _startLoc = floor((GVAR(StartPos) select 0) / GRID_SIZE * _gridsPerEdge) + floor((GVAR(StartPos) select 1) / GRID_SIZE);
		private _goalLoc = floor((GVAR(GoalPos) select 0) / GRID_SIZE * _gridsPerEdge) + floor((GVAR(GoalPos) select 1) / GRID_SIZE);
		
		GVAR(ChartedPath) = [];
		GVAR(ChartedPath) = parseSimpleArray ("lazyThetaStar" callExtension format["calc:%1:%2", _startLoc, _goalLoc]);
		
		for "_a" from 0 to (count GVAR(ChartedPath)) - 1 do {
			private _markerName = "LTSMarker" + (str (_a));
			private _localMarker = createMarkerLocal[_markerName, (GVAR(ChartedPath) select _a)];
			_localMarker setMarkerShapeLocal "ICON";
			_localMarker setMarkerTypeLocal "MOVE";
		};
		
	} else {
		hintSilent "Start and/or goal not chosen, exiting";
	};
} else {
	hintSilent "Height limit not set, exiting";
};
