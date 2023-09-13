#include "script_component.hpp"

//Switch handles the buttons of the SetHeight dialog
switch (_this) do {
	case 0: { //OK Button clicked
		GVAR(HeightLimitString) = ctrlText ((uiNamespace getVariable "RscSetHeight") displayCtrl 1400);

		private _isValid = (count(GVAR(HeightLimitString) splitString "0123456789") == 0);
		if(_isValid) then {
			GVAR(HeightLimit) = parseNumber(GVAR(HeightLimitString));
			GVAR(SetHeightUI) = false;
			closeDialog 1;
		} else {
			hintSilent "Input must be an integer from 0-9999";
		};
	};
	case 1: { //Cancel Button clicked
		GVAR(SetHeightCancelled) = true;
		GVAR(SetHeightUI) = false;
		closeDialog 2;
	};
};