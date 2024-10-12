#pragma once
#include "H/config.h"

namespace Config
{
    bool loadIni()
    {
        CSimpleIniA ini;
        if (!std::filesystem::exists(path))
        {
            logger::error("InI file not exist, will generate default.");
            return createDefaultIni();
        }
        SI_Error rc = ini.LoadFile(path.c_str());
        if (rc < 0)
        {
            logger::error("Cannot load ini.");
            return false;
        }

        // DisableVanilla
        disableOriTweenMenu = ini.GetBoolValue("DisableVanilla", "DisableOriTweenMenu");
        disableOriTogglePOV = ini.GetBoolValue("DisableVanilla", "DisableOriTogglePOV");
        disableOriAttack = ini.GetBoolValue("DisableVanilla", "DisableOriAttack");
        disableOriPowerAttack = ini.GetBoolValue("DisableVanilla", "DisableOriPowerAttack");
        disableOriBlock = ini.GetBoolValue("DisableVanilla", "DisableOriBlock");
        disableOriToggleRunWalk = ini.GetBoolValue("DisableVanilla", "DisableOriAutoMove");
        disableOriQuickInventory = ini.GetBoolValue("DisableVanilla", "DisableOriToggleRunWalk");
        disableOriQuickInventory = ini.GetBoolValue("DisableVanilla", "DisableOriQuickInventory");
        disableOriQuickMagic = ini.GetBoolValue("DisableVanilla", "DisableOriQuickMagic");
        disableOriQuickStats = ini.GetBoolValue("DisableVanilla", "DisableOriQuickStats");
        disableOriQuickMap = ini.GetBoolValue("DisableVanilla", "DisableOriQuickMap");

        // EnableAlternate
        enableAltTweenMenu = ini.GetBoolValue("EnableAlternate", "EnableAltTweenMenu");
        enableAltTweenMenu = ini.GetBoolValue("EnableAlternate", "EnableAltTogglePOV");
        enableAltTweenMenu = ini.GetBoolValue("EnableAlternate", "EnableAltAttack");
        enableAltTweenMenu = ini.GetBoolValue("EnableAlternate", "EnableAltPowerAttack");
        enableAltTweenMenu = ini.GetBoolValue("EnableAlternate", "EnableAltBlock");

        logger::trace("InI file load success.");

        return true;
    }
    bool createDefaultIni()
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

        SI_Error rc = ini.SaveFile(path.c_str());
        if (rc < 0)
        {
            logger::error("Create ini failed.");
            return false;
        }

        logger::trace("Create ini success.");
        return true;
    }
}