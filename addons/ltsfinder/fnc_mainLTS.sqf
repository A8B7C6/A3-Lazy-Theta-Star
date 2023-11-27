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

		private _mapSize = worldSize;
		private _gridsPerEdge = ceil(_mapSize / GRID_SIZE);

		private _startLoc = floor((GVAR(StartPos) select 0) / GRID_SIZE) * _gridsPerEdge + floor((GVAR(StartPos) select 1) / GRID_SIZE);
		private _goalLoc = floor((GVAR(GoalPos) select 0) / GRID_SIZE) * _gridsPerEdge + floor((GVAR(GoalPos) select 1) / GRID_SIZE);
		/*sleep(1);
		hintSilent format["Starting grid: %1", _startLoc];
		sleep(1);
		hintSilent format["Ending grid: %1", _goalLoc];
		sleep(1);*/
		
		GVAR(ChartedPath) = [];
		private _str = "lazyThetaStar" callExtension format["calc:%1:%2", _startLoc, _goalLoc];
		GVAR(ChartedPath) = parseSimpleArray (_str);
		hintSilent format["%1", _str];

		if(GVAR(MarkersCount) > 0) then {
			for "_c" from 0 to GVAR(MarkersCount) - 1 do {
				private _deletedMarkerName = "LTSMarker" + (str (_c));
				deleteMarkerLocal _deletedMarkerName;
			};
		};

		GVAR(MarkersCount) = 0;
		for "_a" from 0 to (count GVAR(ChartedPath)) - 1 do {
			private _markerName = "LTSMarker" + (str (_a));
			GVAR(MarkersCount) = GVAR(MarkersCount) + 1;
			private _localMarker = createMarkerLocal[_markerName, (GVAR(ChartedPath) select _a)];
			_localMarker setMarkerShapeLocal "ICON";
			_localMarker setMarkerTypeLocal "loc_move";
			_localMarker setMarkerColorLocal "ColorGreen";
			_localMarker setMarkerTextLocal (str (_a));
		};
		
	} else {
		hintSilent "Start and/or goal not chosen, exiting";
	};
} else {
	hintSilent "Height limit not set, exiting";
};
