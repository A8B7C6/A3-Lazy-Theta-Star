#include "script_component.hpp"

if (!hasInterface) exitWith {true};

GVAR(StartPos) = [];
GVAR(GoalPos) = [];
GVAR(SetStartAndGoalCancelled) = false;

openMap true;
waitUntil{visibleMap};
hintSilent "Choose the starting location";
while{alive player && visibleMap} do {
	onMapSingleClick {
		GVAR(StartPos) = _pos;
		onMapSingleClick {}; 
		openMap false;
		//true
	};
};

if(count(GVAR(StartPos)) == 0) then {
	GVAR(SetStartAndGoalCancelled) = true;
};

if(!GVAR(SetStartAndGoalCancelled)) then {
	openMap true;
	hintSilent "Choose the destination";
	waitUntil{visibleMap};
	while{alive player && visibleMap} do {
		onMapSingleClick {
			GVAR(GoalPos) = _pos;
			onMapSingleClick {}; 
			openMap false;
			//true
		};
	};

	if(count(GVAR(GoalPos)) == 0) then {
		GVAR(SetStartAndGoalCancelled) = true;
	};
};

GVAR(SetPositionsUI) = false;
