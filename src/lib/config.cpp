#pragma once
#include "config.h"

namespace Config
{
    bool disableOriTweenMenu = false;
    bool disableOriTogglePOV = false;
    bool disableOriAttack = false;
    bool disableOriPowerAttack = false;
    bool disableOriBlock = false;
    bool disableOriAutoMove = false;
    bool disableOriToggleRunWalk = false;
    bool disableOriQuickInventory = false;
    bool disableOriQuickMagic = false;
    bool disableOriQuickStats = false;
    bool disableOriQuickMap = false;

    bool enableAltTweenMenu = false;
    bool enableAltTogglePOV = false;
    bool enableAltAttack = false;
    bool enableAltPowerAttack = false;
    bool enableAltBlock = false;

    bool loadInI()
    {
        CSimpleIniA ini;
        ini.SetUnicode();
        if (!std::filesystem::exists(ini_path))
        {
            logger::error("InI file not exist, will generate default.");
            return createDefaultInI();
        }
        SI_Error rc = ini.LoadFile(ini_path.c_str());
        if (rc < 0)
        {
            logger::error("Cannot load ini.");
            return false;
        }

        // DisableVanilla
        disableOriTweenMenu = ini.GetBoolValue("DisableVanilla", "DisableOriTweenMenu", false);
        disableOriTogglePOV = ini.GetBoolValue("DisableVanilla", "DisableOriTogglePOV", false);
        disableOriAttack = ini.GetBoolValue("DisableVanilla", "DisableOriAttack", false);
        disableOriPowerAttack = ini.GetBoolValue("DisableVanilla", "DisableOriPowerAttack", false);
        disableOriBlock = ini.GetBoolValue("DisableVanilla", "DisableOriBlock", false);
        disableOriToggleRunWalk = ini.GetBoolValue("DisableVanilla", "DisableOriAutoMove", false);
        disableOriQuickInventory = ini.GetBoolValue("DisableVanilla", "DisableOriToggleRunWalk", false);
        disableOriQuickInventory = ini.GetBoolValue("DisableVanilla", "DisableOriQuickInventory", false);
        disableOriQuickMagic = ini.GetBoolValue("DisableVanilla", "DisableOriQuickMagic", false);
        disableOriQuickStats = ini.GetBoolValue("DisableVanilla", "DisableOriQuickStats", false);
        disableOriQuickMap = ini.GetBoolValue("DisableVanilla", "DisableOriQuickMap", false);

        // EnableAlternate
        enableAltTweenMenu = ini.GetBoolValue("EnableAlternate", "EnableAltTweenMenu", false);
        enableAltTogglePOV = ini.GetBoolValue("EnableAlternate", "EnableAltTogglePOV", false);
        enableAltAttack = ini.GetBoolValue("EnableAlternate", "EnableAltAttack", false);
        enableAltPowerAttack = ini.GetBoolValue("EnableAlternate", "EnableAltPowerAttack", false);
        enableAltBlock = ini.GetBoolValue("EnableAlternate", "EnableAltBlock", false);

        logger::trace("InI file load success.");
        return true;
    }
    bool createDefaultInI()
    {
        CSimpleIniA ini;

        ini.SetUnicode();
        // DisableVanilla
        ini.SetBoolValue("DisableVanilla", "DisableOriTweenMenu", false);
        ini.SetBoolValue("DisableVanilla", "DisableOriTogglePOV", false);
        ini.SetBoolValue("DisableVanilla", "DisableOriAttack", false);
        ini.SetBoolValue("DisableVanilla", "DisableOriPowerAttack", false);
        ini.SetBoolValue("DisableVanilla", "DisableOriBlock", false);
        ini.SetBoolValue("DisableVanilla", "DisableOriAutoMove", false);
        ini.SetBoolValue("DisableVanilla", "DisableOriToggleRunWalk", false);
        ini.SetBoolValue("DisableVanilla", "DisableOriQuickInventory", false);
        ini.SetBoolValue("DisableVanilla", "DisableOriQuickMagic", false);
        ini.SetBoolValue("DisableVanilla", "DisableOriQuickStats", false);
        ini.SetBoolValue("DisableVanilla", "DisableOriQuickMap", false);

        // EnableAlternate
        ini.SetBoolValue("EnableAlternate", "EnableAltTweenMenu", false);
        ini.SetBoolValue("EnableAlternate", "EnableAltTogglePOV", false);
        ini.SetBoolValue("EnableAlternate", "EnableAltAttack", false);
        ini.SetBoolValue("EnableAlternate", "EnableAltPowerAttack", false);
        ini.SetBoolValue("EnableAlternate", "EnableAltBlock", false);

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