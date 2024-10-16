#include "config.h"

namespace Config
{

    CSimpleIniA ini;

    // Feature
    bool enableGlobalInput;
    bool enableStances;

    uint32_t clickTime = 50;
    uint32_t longPressTime = 200;

    // Menu
    bool disableOriTweenMenu;
    bool enableAltTweenMenu;
    AltKeyMap AltTweenMenu;

    bool disableOriQuickInventory;
    bool enableAltQuickInventory;
    AltKeyMap AltQuickInventory;

    bool disableOriQuickMagic;
    bool enableAltQuickMagic;
    AltKeyMap AltQuickMagic;

    bool disableOriQuickStats;
    bool enableAltQuickStats;
    AltKeyMap AltQuickStats;

    bool disableOriQuickMap;
    bool enableAltQuickMap;
    AltKeyMap AltQuickMap;

    // Combat
    bool disableOriAttack;
    bool enableAltAttack;
    AltKeyMap AltAttack;

    bool enableAltPowerAttack;
    bool disableOriPowerAttack;
    AltKeyMap AltPowerAttack;

    bool enableAltBlock;
    bool disableOriBlock;
    AltKeyMap AltBlock;

    // Movement

    // Misc
    bool disableOriTogglePOV;
    bool enableAltTogglePOV;
    AltKeyMap AltTogglePOV;

    bool disableOriAutoMove;
    bool enableAltAutoMove;
    AltKeyMap AltAutoMove;

    bool disableOriToggleRun;
    bool enableAltToggleRun;
    AltKeyMap AltToggleRun;

    static std::string *s = nullptr;
    char *str(std::string ss)
    {
        if (s)
        {
            delete (s);
            s = new std::string(ss);
        }
        else
            s = new std::string(ss);
        if (s->at(0) >= 'a')
            s->at(0) = std::toupper(s->at(0));
        return s->data();
    }

    void getKey(AltKeyMap& keyMap,char* name , uint32_t firstKey=0, uint32_t secondKey=0, bool useShortKey=false, uint32_t shortKey=0, uint32_t priority=0) {
        keyMap.useShortKey = ini.GetBoolValue(name, "UseShortKey", useShortKey);
        keyMap.firstKey = ini.GetLongValue(name, "FirstKey", firstKey);
        keyMap.secondKey = ini.GetLongValue(name, "SecondKey", secondKey);
        keyMap.shortKey = ini.GetLongValue(name, "ShortKey", shortKey);
        keyMap.priority = ini.GetLongValue(name, "Priority", priority);
    }

    void setKey(AltKeyMap keyMap,char* name , uint32_t firstKey=0, uint32_t secondKey=0, bool useShortKey=false, uint32_t shortKey=0, uint32_t priority=0) {
        ini.SetBoolValue(name, "UseShortKey", useShortKey);
        ini.SetLongValue(name, "FirstKey", firstKey);
        ini.SetLongValue(name, "SecondKey", secondKey);
        ini.SetLongValue(name, "ShortKey", shortKey);
        ini.SetLongValue(name, "Priority", priority);
    }

    bool loadInI()
    {
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

#ifdef toStr
#undef toStr
#endif;
#define toStr(n) str(std::string(#n))

        // Feature
        enableGlobalInput = ini.GetBoolValue("Feature", toStr(enableGlobalInput), true);
        enableStances = ini.GetBoolValue("Feature", toStr(enableStances), true);

        // Menu
        disableOriTweenMenu = ini.GetBoolValue("Menu", toStr(disableOriTweenMenu), true);
        enableAltTweenMenu = ini.GetBoolValue("Menu", toStr(enableAltTweenMenu), false);

        disableOriQuickInventory = ini.GetBoolValue("Menu", toStr(disableOriQuickInventory), false);
        enableAltQuickInventory = ini.GetBoolValue("Menu", toStr(enableAltQuickInventory), false);

        disableOriQuickMagic = ini.GetBoolValue("Menu", toStr(disableOriQuickMagic), false);
        enableAltQuickMagic = ini.GetBoolValue("Menu", toStr(enableAltQuickMagic), false);

        disableOriQuickStats = ini.GetBoolValue("Menu", toStr(disableOriQuickStats), false);
        enableAltQuickStats = ini.GetBoolValue("Menu", toStr(enableAltQuickStats), false);

        disableOriQuickMap = ini.GetBoolValue("Menu", toStr(disableOriQuickMap), false);
        enableAltQuickMap = ini.GetBoolValue("Menu", toStr(enableAltQuickMap), false);

        // Combat
        disableOriAttack = ini.GetBoolValue("Combat", toStr(disableOriAttack), false);
        enableAltAttack = ini.GetBoolValue("Combat", toStr(enableAltAttack), false);

        disableOriPowerAttack = ini.GetBoolValue("Combat", toStr(disableOriPowerAttack), true);
        enableAltPowerAttack = ini.GetBoolValue("Combat", toStr(enableAltPowerAttack), true);

        disableOriBlock = ini.GetBoolValue("Combat", toStr(disableOriBlock), false);
        enableAltBlock = ini.GetBoolValue("Combat", toStr(enableAltBlock), false);

        // Movement

        // Misc
        disableOriTogglePOV = ini.GetBoolValue("Misc", toStr(disableOriTogglePOV), false);
        enableAltTogglePOV = ini.GetBoolValue("Misc", toStr(enableAltTogglePOV), false);

        disableOriAutoMove = ini.GetBoolValue("Misc", toStr(disableOriAutoMove), false);
        enableAltAutoMove = ini.GetBoolValue("Misc", toStr(enableAltAutoMove), false);

        disableOriToggleRun = ini.GetBoolValue("Misc", toStr(disableOriToggleRun), false);
        enableAltToggleRun = ini.GetBoolValue("Misc", toStr(enableAltToggleRun), false);

        // AlternativeKey
        getKey(AltTweenMenu, toStr(AltTweenMenu));
        getKey(AltQuickInventory, toStr(AltQuickInventory));
        getKey(AltQuickMagic, toStr(AltQuickMagic));
        getKey(AltQuickStats, toStr(AltQuickStats));
        getKey(AltQuickMap, toStr(AltQuickMap));
        getKey(AltAttack, toStr(AltAttack));
        getKey(AltPowerAttack, toStr(AltPowerAttack), KeyUtils::MouseButtonRight);
        getKey(AltBlock, toStr(AltBlock), KeyUtils::Tab);
        getKey(AltTogglePOV, toStr(AltTogglePOV));
        getKey(AltAutoMove, toStr(AltAutoMove));
        getKey(AltToggleRun, toStr(AltToggleRun));

#undef toStr

        if (!s)
            delete s;

        logger::trace("ini file load success.");
        return true;
    }
    bool createDefaultInI()
    {
        ini.SetUnicode();

#ifdef toStr
#undef toStr
#endif;
#define toStr(n) str(std::string(#n))

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

        ini.SetBoolValue("Combat", toStr(disableOriBlock), true);
        ini.SetBoolValue("Combat", toStr(enableAltBlock), true);

        // Movement

        // Misc
        ini.SetBoolValue("Misc", toStr(disableOriTogglePOV), false);
        ini.SetBoolValue("Misc", toStr(enableAltTogglePOV), false);

        ini.SetBoolValue("Misc", toStr(disableOriAutoMove), false);
        ini.SetBoolValue("Misc", toStr(enableAltAutoMove), false);

        ini.SetBoolValue("Misc", toStr(disableOriToggleRun), false);
        ini.SetBoolValue("Misc", toStr(enableAltToggleRun), false);

        // AlternativeKey
        setKey(AltTweenMenu, toStr(AltTweenMenu));
        setKey(AltQuickInventory, toStr(AltQuickInventory));
        setKey(AltQuickMagic, toStr(AltQuickMagic));
        setKey(AltQuickStats, toStr(AltQuickStats));
        setKey(AltQuickMap, toStr(AltQuickMap));
        setKey(AltAttack, toStr(AltAttack));
        setKey(AltPowerAttack, toStr(AltPowerAttack), KeyUtils::MouseButtonRight);
        setKey(AltBlock, toStr(AltBlock), KeyUtils::Tab);
        setKey(AltTogglePOV, toStr(AltTogglePOV));
        setKey(AltAutoMove, toStr(AltAutoMove));
        setKey(AltToggleRun, toStr(AltToggleRun));

#undef toStr

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