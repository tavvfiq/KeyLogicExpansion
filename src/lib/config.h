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
    extern Var::AltKeyMap AltTweenMenu;

    extern bool disableOriQuickInventory;
    extern bool enableAltQuickInventory;
    extern Var::AltKeyMap AltQuickInventory;

    extern bool disableOriQuickMagic;
    extern bool enableAltQuickMagic;
    extern Var::AltKeyMap AltQuickMagic;

    extern bool disableOriQuickStats;
    extern bool enableAltQuickStats;
    extern Var::AltKeyMap AltQuickStats;

    extern bool disableOriQuickMap;
    extern bool enableAltQuickMap;
    extern Var::AltKeyMap AltQuickMap;

    // Combat
    extern bool disableOriAttack;
    extern bool enableAltAttack;
    extern Var::AltKeyMap AltAttack;

    extern bool enableAltPowerAttack;
    extern bool disableOriPowerAttack;
    extern Var::AltKeyMap AltPowerAttack;

    extern bool enableAltBlock;
    extern bool disableOriBlock;
    extern Var::AltKeyMap AltBlock;

    // Movement

    // Misc
    extern bool disableOriTogglePOV;
    extern bool enableAltTogglePOV;
    extern Var::AltKeyMap AltTogglePOV;

    extern bool disableOriAutoMove;
    extern bool enableAltAutoMove;
    extern Var::AltKeyMap AltAutoMove;

    extern bool disableOriToggleRun;
    extern bool enableAltToggleRun;
    extern Var::AltKeyMap AltToggleRun;

    bool loadInI();
    bool createDefaultInI();
};