#pragma once
#include "SimpleIni.h"
#include "keyCodeMap.h"

using namespace RE;
using namespace REL;
using namespace SKSE::log;

namespace Config
{
    typedef struct
    {
        bool useShortKey;
        uint32_t firstKey;
        uint32_t secondKey;
        uint32_t shortKey;
        uint32_t priority;
    } AltKeyMap;

    const std::string ini_path = "Data/SKSE/Plugins/KeyLogicExpansion/settings.ini";

    // Feature
    extern bool enableGlobalInput;
    extern bool enableStances;

    extern uint32_t pressInterval;
    extern uint32_t clickTime;
    extern uint32_t longPressTime;

    // Menu
    extern bool disableOriTweenMenu;
    extern bool enableAltTweenMenu;
    extern AltKeyMap AltTweenMenu;

    extern bool disableOriQuickInventory;
    extern bool enableAltQuickInventory;
    extern AltKeyMap AltQuickInventory;

    extern bool disableOriQuickMagic;
    extern bool enableAltQuickMagic;
    extern AltKeyMap AltQuickMagic;

    extern bool disableOriQuickStats;
    extern bool enableAltQuickStats;
    extern AltKeyMap AltQuickStats;

    extern bool disableOriQuickMap;
    extern bool enableAltQuickMap;
    extern AltKeyMap AltQuickMap;

    // Combat
    extern bool disableOriAttack;
    extern bool enableAltAttack;
    extern AltKeyMap AltAttack;

    extern bool enableAltPowerAttack;
    extern bool disableOriPowerAttack;
    extern AltKeyMap AltPowerAttack;

    extern bool enableAltBlock;
    extern bool disableOriBlock;
    extern AltKeyMap AltBlock;

    // Movement

    // Misc
    extern bool disableOriTogglePOV;
    extern bool enableAltTogglePOV;
    extern AltKeyMap AltTogglePOV;

    extern bool disableOriAutoMove;
    extern bool enableAltAutoMove;
    extern AltKeyMap AltAutoMove;

    extern bool disableOriToggleRun;
    extern bool enableAltToggleRun;
    extern AltKeyMap AltToggleRun;

    bool loadInI();
    bool createDefaultInI();
};