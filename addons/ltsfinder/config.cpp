#include "script_component.hpp"

class CfgPatches {
    class ADDON {
        name = COMPONENT_NAME;
        author = "A8B7C6";
        units[] = {};
        weapons[] = {};
        requiredVersion = 2.11;
        requiredAddons[] = {"cba_main"};
    };
};

#include "CfgEventHandlers.hpp"
#include "UI/RscSetHeight.hpp"
