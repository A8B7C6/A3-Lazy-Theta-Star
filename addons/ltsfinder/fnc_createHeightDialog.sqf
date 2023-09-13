createDialog "RscSetHeight";
waitUntil {!isNull (findDisplay 46) && {dialog}};
disableSerialization;

private _ctrlEdit = (uiNamespace getVariable "RscSetHeight") displayCtrl 1400;

_ctrlEdit ctrlAddEventHandler ["KeyUp", {
	params ["_ctrl", "_key"];
	
	private _text = (ctrlText _ctrl);	
	_ctrlEdit ctrlSetText _text;
}];

ctrlSetFocus _ctrlEdit;
