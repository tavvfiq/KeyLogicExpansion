#pragma once
#include "SimpleIni.h"
#include "utils.h"

using namespace RE;
using namespace REL;
using namespace SKSE::log;

namespace Config
{
    typedef struct
    {
        uint32_t firstKey;
        uint32_t secondKey;
        uint32_t shortKey;
        uint32_t priority;
    } AltKeyMap;

    const std::string ini_path = "Data/SKSE/Plugins/KeyLogicExpansion.ini";

    // Feature
    extern bool enableStances;
    extern bool disableVanillaMenuKey;
    extern bool disableVanillaCombatKey;

    extern uint32_t pressInterval;
    extern uint32_t clickTime;
    extern uint32_t longPressTime;

    // Menu
    extern AltKeyMap AltTweenMenu;
    extern AltKeyMap AltQuickInventory;
    extern AltKeyMap AltQuickMagic;
    extern AltKeyMap AltQuickStats;
    extern AltKeyMap AltQuickMap;

    // Combat
    extern AltKeyMap AltAttack;
    extern AltKeyMap AltPowerAttack;
    extern AltKeyMap AltBlock;

    // Movement

    // Misc
    extern AltKeyMap AltTogglePOV;
    extern AltKeyMap AltAutoMove;
    extern AltKeyMap AltToggleRun;

    bool loadInI();
    bool createDefaultInI();
};