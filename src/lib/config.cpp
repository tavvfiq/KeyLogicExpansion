#include "config.h"

namespace Config
{

    CSimpleIniA ini;

    // Feature
    bool enableStances;

    uint32_t pressInterval = 20;
    uint32_t clickTime = 20;
    uint32_t longPressTime = 200;

    // Menu
    AltKeyMap AltTweenMenu;
    AltKeyMap AltQuickInventory;
    AltKeyMap AltQuickMagic;
    AltKeyMap AltQuickStats;
    AltKeyMap AltQuickMap;

    // Combat
    AltKeyMap AltAttack;
    AltKeyMap AltPowerAttack;
    AltKeyMap AltBlock;

    // Movement

    // Misc
    AltKeyMap AltTogglePOV;
    AltKeyMap AltAutoMove;
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

    void getKey(AltKeyMap &keyMap, char *name, uint32_t firstKey = 0, uint32_t secondKey = 0, uint32_t shortKey = 0, uint32_t priority = 0)
    {
        keyMap.firstKey = ini.GetLongValue(name, "FirstKey", firstKey);
        keyMap.secondKey = ini.GetLongValue(name, "SecondKey", secondKey);
        keyMap.shortKey = ini.GetLongValue(name, "ShortKey", shortKey);
        keyMap.priority = ini.GetLongValue(name, "Priority", priority);
    }

    void setKey(AltKeyMap &keyMap, char *name, uint32_t firstKey = 0, uint32_t secondKey = 0, uint32_t shortKey = 0, uint32_t priority = 0)
    {
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
            logger::error("Cannot load ini.");
            return false;
        }

#ifdef toStr
#undef toStr
#endif;
#define toStr(n) str(std::string(#n))

        // Feature
        enableStances = ini.GetBoolValue("Feature", toStr(enableStances), true);

        // AlternativeKey
        getKey(AltTweenMenu, toStr(AltTweenMenu), KeyUtils::G);
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
        ini.SetBoolValue("Feature", toStr(enableStances), true, ";Enable Stances Supported by KLE.\n;Contain 4 types of stance: High, Mid, Low, and Sheathe.");

        // AlternativeKey
        setKey(AltTweenMenu, toStr(AltTweenMenu), KeyUtils::G);
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