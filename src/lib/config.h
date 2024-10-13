#pragma once
#include "SimpleIni.h"
#include "keyCodeMap.h"

using namespace RE;
using namespace REL;
using namespace SKSE::log;

namespace Config
{   
    const std::string ini_path = "Data/MCM/Config/KeyLogicExpansion/settings.ini";

    extern bool disableOriTweenMenu;
    extern bool disableOriTogglePOV;
    extern bool disableOriAttack;
    extern bool disableOriPowerAttack;
    extern bool disableOriBlock;
    extern bool disableOriAutoMove;
    extern bool disableOriToggleRunWalk;
    extern bool disableOriQuickInventory;
    extern bool disableOriQuickMagic;
    extern bool disableOriQuickStats;
    extern bool disableOriQuickMap;

    extern bool enableAltTweenMenu;
    extern bool enableAltTogglePOV;
    extern bool enableAltAttack;
    extern bool enableAltPowerAttack;
    extern bool enableAltBlock;

    bool loadInI();
    bool createDefaultInI();
};