#include "config.h"

namespace Config
{
    CSimpleIniA ini;
    const static std::string ini_path = "Data/SKSE/Plugins/KeyLogicExpansion.ini";
    const static std::string custom_dir = "Data/SKSE/Plugins/KeyLogicExpansion/";

    uint32_t pressInterval = 20;
    uint32_t clickTime = 20;
    uint32_t longPressTime = 100;

    // Features
    bool enableStances;
    bool enableCustomInput;
    bool enableSheatheAttack;
    bool enableReverseHorseAttack;

    // Vanilla Input
    uint32_t normalAttack;
    uint32_t powerAttack;
    uint32_t block;
    uint32_t altTweenMenu;
    uint32_t altTogglePOV;

    // Expand Input
    uint32_t warAsh;
    uint32_t warAshModifier;

    uint32_t altZoomIn;
    uint32_t altZoomOut;
    uint32_t zoomModifier;

    // Modifier Input
    std::unordered_map<uint32_t, uint32_t> needModifier;

    // Custom Input
    std::vector<CustomInput> custom;

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

    void setVar()
    {
        // Features
        enableStances = ini.GetBoolValue("Features", NameToStr(enableStances), true);
        enableCustomInput = ini.GetBoolValue("Features", NameToStr(enableCustomInput), true);
        enableSheatheAttack = ini.GetBoolValue("Features", NameToStr(enableSheatheAttack), false);
        enableReverseHorseAttack = ini.GetBoolValue("Features", NameToStr(enableReverseHorseAttack), true);

        // Vanilla Input
        normalAttack = ini.GetLongValue("Vanilla", NameToStr(normalAttack), KeyUtils::Mouse::MouseButtonLeft);
        powerAttack = ini.GetLongValue("Vanilla", NameToStr(powerAttack), KeyUtils::Mouse::MouseButtonRight);
        block = ini.GetLongValue("Vanilla", NameToStr(block), KeyUtils::KeyBoard::Tab);

        altTweenMenu = ini.GetLongValue("Vanilla", NameToStr(altTweenMenu), 0);
        altTogglePOV = ini.GetLongValue("Vanilla", NameToStr(altTogglePOV), 0);

        // Expand Input
        warAsh = ini.GetLongValue("Expand", NameToStr(warAsh), 0);
        warAshModifier = ini.GetLongValue("Expand", NameToStr(warAshModifier), 0);

        altZoomIn = ini.GetLongValue("Expand", NameToStr(altZoomIn), 0);
        altZoomOut = ini.GetLongValue("Expand", NameToStr(altZoomOut), 0);
        zoomModifier = ini.GetLongValue("Expand", NameToStr(zoomModifier), 0);

        // Modifier Input
        std::list<CSimpleIniA::Entry> list;
        ini.GetAllKeys("Modifier", std::ref(list));
        for (auto item : list)
        {
            auto trigger = KeyUtils::GetVanillaKeyMap(item.pItem);
            auto modifier = ini.GetLongValue("Modifier", item.pItem, 0);
            if (item.pItem == VarUtils::userEvent->tweenMenu && altTweenMenu)
                trigger = altTweenMenu;
            else if (item.pItem == VarUtils::userEvent->togglePOV && altTogglePOV)
                trigger = altTogglePOV;
            logger::trace("{} {} {}", item.pItem, trigger, modifier);
            if (trigger)
                needModifier.insert(std::make_pair(trigger, modifier));
        }
        if (warAshModifier)
            needModifier.insert(std::make_pair(warAsh, warAshModifier));
        if (zoomModifier)
        {
            if (altZoomIn)
                needModifier.insert(std::make_pair(altZoomIn, zoomModifier));
            else
                needModifier.insert(std::make_pair(KeyUtils::Mouse::MouseWheelUp, zoomModifier));
            if (altZoomOut)
                needModifier.insert(std::make_pair(altZoomOut, zoomModifier));
            else
                needModifier.insert(std::make_pair(KeyUtils::Mouse::MouseWheelDown, zoomModifier));
        }

        // Custom Input

        if (s)
            delete s;
        s = nullptr;
    }

    void createInI()
    {
        // Features
        ini.SetBoolValue("Features", NameToStr(enableStances), true);
        ini.SetBoolValue("Features", NameToStr(enableCustomInput), true);
        ini.SetBoolValue("Features", NameToStr(enableSheatheAttack), false);
        ini.SetBoolValue("Features", NameToStr(enableReverseHorseAttack), true);

        // Vanilla Input
        ini.SetLongValue("Vanilla", NameToStr(normalAttack), KeyUtils::Mouse::MouseButtonLeft);
        ini.SetLongValue("Vanilla", NameToStr(powerAttack), KeyUtils::Mouse::MouseButtonRight);
        ini.SetLongValue("Vanilla", NameToStr(block), KeyUtils::KeyBoard::Tab);

        ini.SetLongValue("Vanilla", NameToStr(altTweenMenu), 0);
        ini.SetLongValue("Vanilla", NameToStr(altTogglePOV), 0);

        // Expand Input
        ini.GetLongValue("Expand", NameToStr(warAsh), 0);
        ini.GetLongValue("Expand", NameToStr(warAshModifier), 0);

        ini.GetLongValue("Expand", NameToStr(altZoomIn), 0);
        ini.GetLongValue("Expand", NameToStr(altZoomOut), 0);
        ini.GetLongValue("Expand", NameToStr(zoomModifier), 0);

        // Modifier Input
        ini.SetLongValue("Modifier", VarUtils::userEvent->forward.c_str(), 0);
        ini.SetLongValue("Modifier", VarUtils::userEvent->strafeLeft.c_str(), 0);
        ini.SetLongValue("Modifier", VarUtils::userEvent->back.c_str(), 0);
        ini.SetLongValue("Modifier", VarUtils::userEvent->strafeRight.c_str(), 0);
        ini.SetLongValue("Modifier", VarUtils::userEvent->rightAttack.c_str(), 0);
        ini.SetLongValue("Modifier", VarUtils::userEvent->leftAttack.c_str(), 0);
        ini.SetLongValue("Modifier", VarUtils::userEvent->activate.c_str(), 0);
        ini.SetLongValue("Modifier", VarUtils::userEvent->readyWeapon.c_str(), 0);
        ini.SetLongValue("Modifier", VarUtils::userEvent->tweenMenu.c_str(), 0);
        ini.SetLongValue("Modifier", VarUtils::userEvent->togglePOV.c_str(), 0);
        ini.SetLongValue("Modifier", VarUtils::userEvent->jump.c_str(), 0);
        ini.SetLongValue("Modifier", VarUtils::userEvent->sprint.c_str(), 0);
        ini.SetLongValue("Modifier", VarUtils::userEvent->shout.c_str(), 0);
        ini.SetLongValue("Modifier", VarUtils::userEvent->sneak.c_str(), 0);
        ini.SetLongValue("Modifier", VarUtils::userEvent->run.c_str(), 0);
        ini.SetLongValue("Modifier", VarUtils::userEvent->toggleRun.c_str(), 0);
        ini.SetLongValue("Modifier", VarUtils::userEvent->autoMove.c_str(), 0);
        ini.SetLongValue("Modifier", VarUtils::userEvent->favorites.c_str(), 0);
        ini.SetLongValue("Modifier", VarUtils::userEvent->quicksave.c_str(), 0);
        ini.SetLongValue("Modifier", VarUtils::userEvent->quickload.c_str(), 0);
        ini.SetLongValue("Modifier", VarUtils::userEvent->wait.c_str(), 0);
        ini.SetLongValue("Modifier", VarUtils::userEvent->journal.c_str(), 0);
        ini.SetLongValue("Modifier", VarUtils::userEvent->pause.c_str(), 0);
        ini.SetLongValue("Modifier", VarUtils::userEvent->quickInventory.c_str(), 0);
        ini.SetLongValue("Modifier", VarUtils::userEvent->quickMagic.c_str(), 0);
        ini.SetLongValue("Modifier", VarUtils::userEvent->quickStats.c_str(), 0);
        ini.SetLongValue("Modifier", VarUtils::userEvent->quickMap.c_str(), 0);

        // Custom Input

        SI_Error rc = ini.SaveFile(ini_path.c_str());
        if (rc < 0)
            return SKSE::stl::report_and_fail("Cannot load ini.");

        setVar();

        logger::trace("Create ini success.");
    }

    void loadInI()
    {
        ini.SetUnicode();
        if (!std::filesystem::exists(ini_path))
        {
            logger::error("ini file not exist, will generate default.");
            return createInI();
        }
        SI_Error rc = ini.LoadFile(ini_path.c_str());
        if (rc < 0)
            return SKSE::stl::report_and_fail("Cannot load ini.");

        setVar();
    }
}