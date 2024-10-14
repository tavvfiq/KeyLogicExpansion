#pragma once
#include "SimpleIni.h"
#include "keyCodeMap.h"

using namespace RE;
using namespace REL;
using namespace SKSE::log;

namespace Config
{   
    const std::string ini_path = "Data/SKSE/Plugins/KeyLogicExpansion/settings.ini";

    // Feature
    extern bool enableGlobalInput;
    extern bool enableStances;

    // Menu
    extern bool disableOriTweenMenu;
    extern bool enableAltTweenMenu;

    extern bool disableOriQuickInventory;
    extern bool enableAltQuickInventory;

    extern bool disableOriQuickMagic;
    extern bool enableAltQuickMagic;

    extern bool disableOriQuickStats;
    extern bool enableAltQuickStats;

    extern bool disableOriQuickMap;
    extern bool enableAltQuickMap;

    // Combat
    extern bool disableOriAttack;
    extern bool enableAltAttack;

    extern bool disableOriPowerAttack;
    extern bool enableAltPowerAttack;

    extern bool disableOriBlock;
    extern bool enableAltBlock;

    // Movement

    // Misc
    extern bool disableOriTogglePOV;
    extern bool enableAltTogglePOV;
    
    extern bool disableOriAutoMove;
    extern bool enableAltAutoMove;

    extern bool disableOriToggleRun;
    extern bool enableAltToggleRun;


    

    bool loadInI();
    bool createDefaultInI();
};