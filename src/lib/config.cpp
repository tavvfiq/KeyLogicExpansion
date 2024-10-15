#include "config.h"

namespace Config
{
    // Feature
    bool enableGlobalInput;
    bool enableStances;

    // Menu
    bool disableOriTweenMenu;
    bool enableAltTweenMenu;
    Var::AltKeyMap AltTweenMenu;

    bool disableOriQuickInventory;
    bool enableAltQuickInventory;
    Var::AltKeyMap AltQuickInventory;

    bool disableOriQuickMagic;
    bool enableAltQuickMagic;
    Var::AltKeyMap AltQuickMagic;

    bool disableOriQuickStats;
    bool enableAltQuickStats;
    Var::AltKeyMap AltQuickStats;

    bool disableOriQuickMap;
    bool enableAltQuickMap;
    Var::AltKeyMap AltQuickMap;

    // Combat
    bool disableOriAttack;
    bool enableAltAttack;
    Var::AltKeyMap AltAttack;

    bool enableAltPowerAttack;
    bool disableOriPowerAttack;
    Var::AltKeyMap AltPowerAttack;

    bool enableAltBlock;
    bool disableOriBlock;
    Var::AltKeyMap AltBlock;

    // Movement

    // Misc
    bool disableOriTogglePOV;
    bool enableAltTogglePOV;
    Var::AltKeyMap AltTogglePOV;

    bool disableOriAutoMove;
    bool enableAltAutoMove;
    Var::AltKeyMap AltAutoMove;

    bool disableOriToggleRun;
    bool enableAltToggleRun;
    Var::AltKeyMap AltToggleRun;

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
        AltTweenMenu.useShortKey = ini.GetBoolValue(toStr(AltTweenMenu), "UseShortKey", false);
        AltTweenMenu.firstKey = ini.GetLongValue(toStr(AltTweenMenu), "FirstKey", 0);
        AltTweenMenu.secondKey = ini.GetLongValue(toStr(AltTweenMenu), "SecondKey", 0);
        AltTweenMenu.shortKey = ini.GetLongValue(toStr(AltTweenMenu), "ShortKey", 0);
        AltTweenMenu.priority = ini.GetLongValue(toStr(AltTweenMenu), "Priority", 0);

        AltQuickInventory.useShortKey = ini.GetBoolValue(toStr(AltQuickInventory), "UseShortKey", false);
        AltQuickInventory.firstKey = ini.GetLongValue(toStr(AltQuickInventory), "FirstKey", 0);
        AltQuickInventory.secondKey = ini.GetLongValue(toStr(AltQuickInventory), "SecondKey", 0);
        AltQuickInventory.shortKey = ini.GetLongValue(toStr(AltQuickInventory), "ShortKey", 0);
        AltQuickInventory.priority = ini.GetLongValue(toStr(AltQuickInventory), "Priority", 0);

        AltQuickMagic.useShortKey = ini.GetBoolValue(toStr(AltQuickMagic), "UseShortKey", false);
        AltQuickMagic.firstKey = ini.GetLongValue(toStr(AltQuickMagic), "FirstKey", 0);
        AltQuickMagic.secondKey = ini.GetLongValue(toStr(AltQuickMagic), "SecondKey", 0);
        AltQuickMagic.shortKey = ini.GetLongValue(toStr(AltQuickMagic), "ShortKey", 0);
        AltQuickMagic.priority = ini.GetLongValue(toStr(AltQuickMagic), "Priority", 0);

        AltQuickStats.useShortKey = ini.GetBoolValue(toStr(AltQuickStats), "UseShortKey", false);
        AltQuickStats.firstKey = ini.GetLongValue(toStr(AltQuickStats), "FirstKey", 0);
        AltQuickStats.secondKey = ini.GetLongValue(toStr(AltQuickStats), "SecondKey", 0);
        AltQuickStats.useShortKey = ini.GetLongValue(toStr(AltQuickStats), "ShortKey", 0);
        AltQuickStats.priority = ini.GetLongValue(toStr(AltQuickStats), "Priority", 0);

        AltQuickMap.useShortKey = ini.GetBoolValue(toStr(AltQuickMap), "UseShortKey", false);
        AltQuickMap.firstKey = ini.GetLongValue(toStr(AltQuickMap), "FirstKey", 0);
        AltQuickMap.secondKey = ini.GetLongValue(toStr(AltQuickMap), "SecondKey", 0);
        AltQuickMap.shortKey = ini.GetLongValue(toStr(AltQuickMap), "ShortKey", 0);
        AltQuickMap.priority = ini.GetLongValue(toStr(AltQuickMap), "Priority", 0);

        AltAttack.useShortKey = ini.GetBoolValue(toStr(AltAttack), "UseShortKey", false);
        AltAttack.firstKey = ini.GetLongValue(toStr(AltAttack), "FirstKey", 0);
        AltAttack.secondKey = ini.GetLongValue(toStr(AltAttack), "SecondKey", 0);
        AltAttack.shortKey = ini.GetLongValue(toStr(AltAttack), "ShortKey", 0);
        AltAttack.priority = ini.GetLongValue(toStr(AltAttack), "Priority", 0);

        AltPowerAttack.useShortKey = ini.GetBoolValue(toStr(AltPowerAttack), "UseShortKey", false);
        AltPowerAttack.firstKey = ini.GetLongValue(toStr(AltPowerAttack), "FirstKey", KeyUtils::MouseButtonRight);
        AltPowerAttack.secondKey = ini.GetLongValue(toStr(AltPowerAttack), "SecondKey", 0);
        AltPowerAttack.shortKey = ini.GetLongValue(toStr(AltPowerAttack), "ShortKey", 0);
        AltPowerAttack.priority = ini.GetLongValue(toStr(AltPowerAttack), "Priority", 0);

        AltBlock.useShortKey = ini.GetBoolValue(toStr(AltBlock), "UseShortKey", false);
        AltBlock.firstKey = ini.GetLongValue(toStr(AltBlock), "FirstKey", KeyUtils::Tab);
        AltBlock.secondKey = ini.GetLongValue(toStr(AltBlock), "SecondKey", 0);
        AltBlock.shortKey = ini.GetLongValue(toStr(AltBlock), "ShortKey", 0);
        AltBlock.priority = ini.GetLongValue(toStr(AltBlock), "Priority", 0);

#undef toStr

        if (!s)
            delete s;

        logger::trace("ini file load success.");
        return true;
    }
    bool createDefaultInI()
    {
        CSimpleIniA ini;
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
        ini.SetBoolValue(toStr(AltTweenMenu), "UseShortKey", false);
        ini.SetLongValue(toStr(AltTweenMenu), "FirstKey", 0);
        ini.SetLongValue(toStr(AltTweenMenu), "SecondKey", 0);
        ini.SetLongValue(toStr(AltTweenMenu), "ShortKey", 0);
        ini.SetLongValue(toStr(AltTweenMenu), "Priority", 0);

        ini.SetBoolValue(toStr(AltQuickInventory), "UseShortKey", false);
        ini.SetLongValue(toStr(AltQuickInventory), "FirstKey", 0);
        ini.SetLongValue(toStr(AltQuickInventory), "SecondKey", 0);
        ini.SetLongValue(toStr(AltQuickInventory), "ShortKey", 0);
        ini.SetLongValue(toStr(AltQuickInventory), "Priority", 0);

        ini.SetBoolValue(toStr(AltQuickMagic), "UseShortKey", false);
        ini.SetLongValue(toStr(AltQuickMagic), "FirstKey", 0);
        ini.SetLongValue(toStr(AltQuickMagic), "SecondKey", 0);
        ini.SetLongValue(toStr(AltQuickMagic), "ShortKey", 0);
        ini.SetLongValue(toStr(AltQuickMagic), "Priority", 0);

        ini.SetBoolValue(toStr(AltQuickStats), "UseShortKey", false);
        ini.SetLongValue(toStr(AltQuickStats), "FirstKey", 0);
        ini.SetLongValue(toStr(AltQuickStats), "SecondKey", 0);
        ini.SetLongValue(toStr(AltQuickStats), "ShortKey", 0);
        ini.SetLongValue(toStr(AltQuickStats), "Priority", 0);

        ini.SetBoolValue(toStr(AltQuickMap), "UseShortKey", false);
        ini.SetLongValue(toStr(AltQuickMap), "FirstKey", 0);
        ini.SetLongValue(toStr(AltQuickMap), "SecondKey", 0);
        ini.SetLongValue(toStr(AltQuickMap), "ShortKey", 0);
        ini.SetLongValue(toStr(AltQuickMap), "Priority", 0);

        ini.SetBoolValue(toStr(AltAttack), "UseShortKey", false);
        ini.SetLongValue(toStr(AltAttack), "FirstKey", 0);
        ini.SetLongValue(toStr(AltAttack), "SecondKey", 0);
        ini.SetLongValue(toStr(AltAttack), "ShortKey", 0);
        ini.SetLongValue(toStr(AltAttack), "Priority", 0);

        ini.SetBoolValue(toStr(AltPowerAttack), "UseShortKey", false);
        ini.SetLongValue(toStr(AltPowerAttack), "FirstKey", KeyUtils::MouseButtonRight);
        ini.SetLongValue(toStr(AltPowerAttack), "SecondKey", 0);
        ini.SetLongValue(toStr(AltPowerAttack), "ShortKey", 0);
        ini.SetLongValue(toStr(AltPowerAttack), "Priority", 0);

        ini.SetBoolValue(toStr(AltBlock), "UseShortKey", false);
        ini.SetLongValue(toStr(AltBlock), "FirstKey", KeyUtils::Tab);
        ini.SetLongValue(toStr(AltBlock), "SecondKey", 0);
        ini.SetLongValue(toStr(AltBlock), "ShortKey", 0);
        ini.SetLongValue(toStr(AltBlock), "Priority", 0);

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