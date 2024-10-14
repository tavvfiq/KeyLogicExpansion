#pragma once
#include "config.h"

namespace Config
{
    // Feature
    bool enableGlobalInput;
    bool enableStances;

    // Menu
    bool disableOriTweenMenu;
    bool enableAltTweenMenu;

    bool disableOriQuickInventory;
    bool enableAltQuickInventory;

    bool disableOriQuickMagic;
    bool enableAltQuickMagic;

    bool disableOriQuickStats;
    bool enableAltQuickStats;

    bool disableOriQuickMap;
    bool enableAltQuickMap;

    // Combat
    bool disableOriAttack;
    bool enableAltAttack;

    bool enableAltPowerAttack;
    bool disableOriPowerAttack;

    bool enableAltBlock;
    bool disableOriBlock;

    // Movement

    // Misc
    bool disableOriTogglePOV;
    bool enableAltTogglePOV;
    
    bool disableOriAutoMove;
    bool enableAltAutoMove;

    bool disableOriToggleRun;
    bool enableAltToggleRun;


    static std::string* s = nullptr;
    char* str(std::string ss) {
        if (s) {
            delete(s);
            s = new std::string(ss);
        } else
            s = new std::string(ss);
        s->at(0) = std::toupper(s->at(0));
        return s->data();
    }
    bool loadInI()
    {
        CSimpleIniA ini;
        ini.SetUnicode();
        if (!std::filesystem::exists(ini_path))
        {
            logger::error("ini file not exist, will generate default.");
            return createDefaultInI();
        }
        SI_Error rc = ini.LoadFile(ini_path.c_str());
        if (rc < 0)
        {
            logger::error("Cannot load  = ini.");
            return false;
        }

        #ifndef toStr
        #define toStr(n) str(std::string(#n))
        #endif;

        // Feature
        enableGlobalInput = ini.SetBoolValue("Feature", toStr(enableGlobalInput), true);
        enableStances = ini.SetBoolValue("Feature", toStr(enableStances), true);

        // Menu
        disableOriTweenMenu = ini.SetBoolValue("Menu", toStr(disableOriTweenMenu), true);
        enableAltTweenMenu = ini.SetBoolValue("Menu", toStr(enableAltTweenMenu), false);

        disableOriQuickInventory = ini.SetBoolValue("Menu", toStr(disableOriQuickInventory), false);
        enableAltQuickInventory = ini.SetBoolValue("Menu", toStr(enableAltQuickInventory), false);

        disableOriQuickMagic = ini.SetBoolValue("Menu", toStr(disableOriQuickMagic), false);
        enableAltQuickMagic = ini.SetBoolValue("Menu", toStr(enableAltQuickMagic), false);

        disableOriQuickStats = ini.SetBoolValue("Menu", toStr(disableOriQuickStats), false);
        enableAltQuickStats = ini.SetBoolValue("Menu", toStr(enableAltQuickStats), false);

        disableOriQuickMap = ini.SetBoolValue("Menu", toStr(disableOriQuickMap), false);
        enableAltQuickMap = ini.SetBoolValue("Menu", toStr(enableAltQuickMap), false);

        // Combat
        disableOriAttack = ini.SetBoolValue("Combat", toStr(disableOriAttack), false);
        enableAltAttack = ini.SetBoolValue("Combat", toStr(enableAltAttack), false);

        disableOriPowerAttack = ini.SetBoolValue("Combat", toStr(disableOriPowerAttack), true);
        enableAltPowerAttack = ini.SetBoolValue("Combat", toStr(enableAltPowerAttack), true);

        disableOriBlock = ini.SetBoolValue("Combat", toStr(disableOriBlock), false);
        enableAltBlock = ini.SetBoolValue("Combat", toStr(enableAltBlock), false);


        // Movement

        // Misc
        disableOriTogglePOV = ini.SetBoolValue("Misc", toStr(disableOriTogglePOV), false);
        enableAltTogglePOV = ini.SetBoolValue("Misc", toStr(enableAltTogglePOV), false);

        disableOriAutoMove = ini.SetBoolValue("Misc", toStr(disableOriAutoMove), false);
        enableAltAutoMove = ini.SetBoolValue("Misc", toStr(enableAltAutoMove), false);

        disableOriToggleRun = ini.SetBoolValue("Misc", toStr(disableOriToggleRun), false);
        enableAltToggleRun = ini.SetBoolValue("Misc", toStr(enableAltToggleRun), false);

        #ifdef toStr
        #undef toStr
        #endif;

        logger::trace("ini file load success.");
        return true;
    }
    bool createDefaultInI()
    {
        CSimpleIniA ini;
        ini.SetUnicode();

        #ifndef toStr
        #define toStr(n) str(std::string(#n))
        #endif;

        // Feature
        ini.SetBoolValue("Feature", toStr(enableGlobalInput), true, ";Enable KLE Process ButtonEvent which has AlternativeKey.\n;If you switch off this AlternativeKey won't work.");
        ini.SetBoolValue("Feature", toStr(enableStances), true, ";Enable Stances Supported by KLE.\n;Contain 4 types of stance: High, Mid, Low, and Sheathe.");

        // Menu
        ini.SetBoolValue("Menu", toStr(disableOriTweenMenu), true);
        ini.SetBoolValue("Menu", toStr(enableAltTweenMenu), false);

        ini.SetBoolValue("Menu", toStr(disableOriQuickInventory), false);
        ini.SetBoolValue("Menu", toStr(enableAltQuickInventory), false);

        ini.SetBoolValue("Menu", toStr(disableOriQuickMagic), false);
        ini.SetBoolValue("Menu", toStr(enableAltQuickMagic), false);

        ini.SetBoolValue("Menu", toStr(disableOriQuickStats), false);
        ini.SetBoolValue("Menu", toStr(enableAltQuickStats), false);

        ini.SetBoolValue("Menu", toStr(disableOriQuickMap), false);
        ini.SetBoolValue("Menu", toStr(enableAltQuickMap), false);

        // Combat
        ini.SetBoolValue("Combat", toStr(disableOriAttack), false);
        ini.SetBoolValue("Combat", toStr(enableAltAttack), false);

        ini.SetBoolValue("Combat", toStr(disableOriPowerAttack), true);
        ini.SetBoolValue("Combat", toStr(enableAltPowerAttack), true);

        ini.SetBoolValue("Combat", toStr(disableOriBlock), false);
        ini.SetBoolValue("Combat", toStr(enableAltBlock), false);


        // Movement

        // Misc
        ini.SetBoolValue("Misc", toStr(disableOriTogglePOV), false);
        ini.SetBoolValue("Misc", toStr(enableAltTogglePOV), false);

        ini.SetBoolValue("Misc", toStr(disableOriAutoMove), false);
        ini.SetBoolValue("Misc", toStr(enableAltAutoMove), false);

        ini.SetBoolValue("Misc", toStr(disableOriToggleRun), false);
        ini.SetBoolValue("Misc", toStr(enableAltToggleRun), false);


        #ifdef toStr
        #undef toStr
        #endif;
        if (!s)
            delete s;

        SI_Error rc = ini.SaveFile(ini_path.c_str());
        if (rc < 0)
        {
            logger::error("Create ini failed.");
            return false;
        }

        logger::trace("Create ini success.");
        return true;
    }
}