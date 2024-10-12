#pragma once
#include "PCH.h"
#include "SimpleIni.h"

using namespace RE;
using namespace REL;
using namespace SKSE::log;

namespace Config {

    static std::string path = "Data/MCM/Config/KeyLogicExpansion/settings.ini";

    static bool disableOriTweenMenu;
    static bool disableOriTogglePOV;
    static bool disableOriAttack;
    static bool disableOriPowerAttack;
    static bool disableOriBlock;
    static bool disableOriAutoMove;
    static bool disableOriToggleRunWalk;
    static bool disableOriQuickInventory;
    static bool disableOriQuickMagic;
    static bool disableOriQuickStats;
    static bool disableOriQuickMap;

    static bool enableAltTweenMenu;
    static bool enableAltTogglePOV;
    static bool enableAltAttack;
    static bool enableAltPowerAttack;
    static bool enableAltBlock;

    bool loadIni();
    bool createDefaultIni();
}