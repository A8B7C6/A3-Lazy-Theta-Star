class RscFrame;
class RscText;
class RscButtonMenuOK;
class RscButtonMenuCancel;
class RscEdit;

class RscSetHeight {
	name = "RscSetHeight";
	idd = 30241;
	onLoad="uiNamespace setVariable ['RscSetHeight', _this select 0]";
	onUnload="uiNamespace setVariable ['RscSetHeight', nil]";
	movingEnable=1;
	objects[] = {};
	class ControlsBackground {
		class SetHeightGUI: RscFrame {
			idc = 1800;
			colorBackground[] = {0,0,0,1};
			x = 0.300781 * safezoneW + safezoneX;
			y = 0.245 * safezoneH + safezoneY;
			w = 0.398437 * safezoneW;
			h = 0.51 * safezoneH;
		};
		class DescText: RscText {
			idc = 1001;
			text = "Enter ASL height limit in meters (0 <= limit <= 9999):";
			x = 0.348594 * safezoneW + safezoneX;
			y = 0.398 * safezoneH + safezoneY;
			w = 0.31875 * safezoneW;
			h = 0.085 * safezoneH;
			colorText[] = {-1,20,-1,1};
		};
	};
	class Controls {
		class OKButton: RscButtonMenuOK {
			idc = 2001;
			onButtonClick = QUOTE(0 call FUNC(setHeightDialog));
			x = 0.619531 * safezoneW + safezoneX;
			y = 0.636 * safezoneH + safezoneY;
			w = 0.06375 * safezoneW;
			h = 0.085 * safezoneH;
		};
		class CancelButton: RscButtonMenuCancel {
			idc = 2002;
			onButtonClick = QUOTE(1 call FUNC(setHeightDialog));
			x = 0.316719 * safezoneW + safezoneX;
			y = 0.636 * safezoneH + safezoneY;
			w = 0.06375 * safezoneW;
			h = 0.085 * safezoneH;
		};
		class HeightLimitSetter: RscEdit {
			idc = 1400;
			text = "";
			autocomplete = "scripting";
			x = 0.452187 * safezoneW + safezoneX;
			y = 0.5 * safezoneH + safezoneY;
			w = 0.095625 * safezoneW;
			h = 0.085 * safezoneH;
			colorBackground[] = {0,0,0,0.5};
			colorText[] = {-1,20,-1,1};
			sizeEx = 2 * GUI_GRID_H;
			maxChars = 4;
		};
	};
};

