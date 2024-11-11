#include "config.h"

namespace Config
{
CSimpleIniA ini;
const static std::string ini_path = "Data/SKSE/Plugins/KeyLogicExpansion.ini";

// Features
bool &enableCustomInput = std::ref(Custom::enableCustomInput);
bool &enableStances = std::ref(Stances::enableStances);
bool enableHoldSprint;
bool enableHoldSneak;
uint32_t enableSheatheAttack;
bool enableReverseHorseAttack;

// Stances
uint32_t &StancesModifier = std::ref(Stances::StancesModfier);
uint32_t &ChangeToLow = std::ref(Stances::ChangeToLow);
uint32_t &ChangeToMid = std::ref(Stances::ChangeToMid);
uint32_t &ChangeToHigh = std::ref(Stances::ChangeToHigh);

// Vanilla Input
uint32_t normalAttack;
uint32_t powerAttack;
uint32_t block;
uint32_t MagicModifier;
uint32_t BFCO_SpecialAttackModifier;
uint32_t BFCO_ComboAttack;

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
    enableCustomInput = ini.GetBoolValue("Features", NameToStr(enableCustomInput), true);
    enableStances = ini.GetBoolValue("Features", NameToStr(enableStances), false);
    enableHoldSprint = ini.GetBoolValue("Features", NameToStr(enableHoldSprint), true);
    enableHoldSneak = ini.GetBoolValue("Features", NameToStr(enableHoldSneak), false);
    enableReverseHorseAttack = ini.GetBoolValue("Features", NameToStr(enableReverseHorseAttack), false);
    enableSheatheAttack = ini.GetLongValue("Features", NameToStr(enableSheatheAttack), 0);

    // Stances
    if (enableStances)
    {
        Stances::StancesList.push_back(
            FormUtils::GetForm((ini.GetValue("Stances", "Low", "Stances - Dynamic Weapon Movesets SE.esp|0x4251A"))));
        Stances::StancesList.push_back(
            FormUtils::GetForm((ini.GetValue("Stances", "Mid", "Stances - Dynamic Weapon Movesets SE.esp|0x42519"))));
        Stances::StancesList.push_back(
            FormUtils::GetForm((ini.GetValue("Stances", "High", "Stances - Dynamic Weapon Movesets SE.esp|0x42518"))));
        StancesModifier = ini.GetLongValue("Stances", "StancesModifier", KeyUtils::KeyBoard::LeftShift);
        ChangeToLow = ini.GetLongValue("Stances", "ChangeToLow", KeyUtils::Mouse::MouseWheelDown);
        ChangeToMid = ini.GetLongValue("Stances", "ChangeToMid", KeyUtils::Mouse::MouseButtonMiddle);
        ChangeToHigh = ini.GetLongValue("Stances", "ChangeToHigh", KeyUtils::Mouse::MouseWheelUp);
    }

    // Vanilla Input
    normalAttack = ini.GetLongValue("Vanilla", NameToStr(normalAttack), KeyUtils::Mouse::MouseButtonLeft);
    powerAttack = ini.GetLongValue("Vanilla", NameToStr(powerAttack), KeyUtils::Mouse::MouseButtonRight);
    block = ini.GetLongValue("Vanilla", NameToStr(block), KeyUtils::KeyBoard::Tab);
    MagicModifier = ini.GetLongValue("Vanilla", NameToStr(MagicModifier), KeyUtils::KeyBoard::E);
    BFCO_SpecialAttackModifier =
        ini.GetLongValue("Vanilla", NameToStr(BFCO_SpecialAttackModifier), KeyUtils::KeyBoard::LeftShift);
    BFCO_ComboAttack = ini.GetLongValue("Vanilla", NameToStr(BFCO_ComboAttack), KeyUtils::KeyBoard::E);

    altTweenMenu = ini.GetLongValue("Vanilla", NameToStr(altTweenMenu), KeyUtils::KeyBoard::G);
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
    if (altZoomIn)
        needModifier.insert(std::make_pair(altZoomIn, zoomModifier));
    else
        needModifier.insert(std::make_pair(KeyUtils::Mouse::MouseWheelUp, zoomModifier));
    if (altZoomOut)
        needModifier.insert(std::make_pair(altZoomOut, zoomModifier));
    else
        needModifier.insert(std::make_pair(KeyUtils::Mouse::MouseWheelDown, zoomModifier));

    // Custom Input
    if (enableCustomInput)
        Custom::LoadCustom();

    if (s)
        delete s;
    s = nullptr;
}

void createInI()
{
    // Features
    ini.SetBoolValue("Features", NameToStr(enableCustomInput), true,
                     ";Enable custom Input, maybe this is the reason you install this mod.");
    ini.SetBoolValue("Features", NameToStr(enableStances), false,
                     ";Enable Stances Supported by KLE\n;Contain 3 types "
                     "of stance: High, Mid, Low.");
    ini.SetBoolValue("Features", NameToStr(enableHoldSprint), true, ";Change enable sprint when you hold sprint key");
    ini.SetBoolValue("Features", NameToStr(enableHoldSneak), false,
                     ";Same as EnableHoldSprint, Change enable sneak when you hold sneak key");
    ini.SetBoolValue(
        "Features", NameToStr(enableReverseHorseAttack), false,
        ";Reverse your HorseAttack diretion, if enable this, left key attack left, right key attack right");
    ini.SetLongValue(
        "Features", NameToStr(enableSheatheAttack), 0,
        ";Make you can attack when you press this key, NOT completed.\n;0 means disable, other number means a "
        "keycode\n;"
        "when you press the key and Attack or PowerAttack or even Sheathe Key, you will do a SheatheAttack\n;Note: "
        "Press with Sheathe Key can do SheatheAttack when you are NOT in Sheathe status.");

    // Stances
    ini.SetValue("Stances", "Low", "Stances - Dynamic Weapon Movesets SE.esp|0x4251A",
                 ";Set this according to your stances mod");
    ini.SetValue("Stances", "Mid", "Stances - Dynamic Weapon Movesets SE.esp|0x42519");
    ini.SetValue("Stances", "High", "Stances - Dynamic Weapon Movesets SE.esp|0x42518");
    ini.SetLongValue("Stances", "StancesModifier", KeyUtils::KeyBoard::LeftShift,
                     ";Set change stance key, modifier = 0 means disable\n");
    ini.SetLongValue("Stances", "ChangeToLow", KeyUtils::Mouse::MouseWheelDown);
    ini.SetLongValue("Stances", "ChangeToMid", KeyUtils::Mouse::MouseButtonMiddle);
    ini.SetLongValue("Stances", "ChangeToHigh", KeyUtils::Mouse::MouseWheelUp);

    // Vanilla Input
    ini.SetLongValue(
        "Vanilla", NameToStr(normalAttack), KeyUtils::Mouse::MouseButtonLeft,
        ";separete block key from Attack, still have some problems, don't use blockbash, it is overpowered");
    ini.SetLongValue("Vanilla", NameToStr(powerAttack), KeyUtils::Mouse::MouseButtonRight);
    ini.SetLongValue("Vanilla", NameToStr(block), KeyUtils::KeyBoard::Tab);
    ini.SetLongValue("Vanilla", NameToStr(MagicModifier), KeyUtils::KeyBoard::E);
    ini.SetLongValue("Vanilla", NameToStr(BFCO_SpecialAttackModifier), KeyUtils::KeyBoard::LeftShift);
    ini.SetLongValue("Vanilla", NameToStr(BFCO_ComboAttack), KeyUtils::KeyBoard::E);

    ini.SetLongValue("Vanilla", NameToStr(altTweenMenu), KeyUtils::KeyBoard::G,
                     ";instead Vanilla Key, just All Alt* option do the same thing.");
    ini.SetLongValue("Vanilla", NameToStr(altTogglePOV), 0);

    // Expand Input
    ini.SetLongValue("Expand", NameToStr(warAsh), 0, ";EldenRim WarAsh support, press this to use WarAsh.");
    ini.SetLongValue("Expand", NameToStr(warAshModifier), 0);

    ini.SetLongValue("Expand", NameToStr(altZoomIn), 0,
                     ";set it to Non 0 can instead default ZoomIn and ZoomOut \n;you don't konw what it mean? just "
                     "Vanilla MouseWheelUp and MouseWheelDwon");
    ini.SetLongValue("Expand", NameToStr(altZoomOut), 0);
    ini.SetLongValue("Expand", NameToStr(zoomModifier), 0);

    // Modifier Input
    ini.SetLongValue("Modifier", VarUtils::userEvent->forward.c_str(), 0,
                     ";Add a Modifier to Vanilla Key, if you want a key to work, you need press modifier key "
                     "first.\n;set it 0 to disable, other means a keycode");
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

void saveInI()
{
    ini.SetUnicode();
    SI_Error rc = ini.LoadFile(ini_path.c_str());
    if (rc < 0)
        return SKSE::stl::report_and_fail("Cannot save ini.");

    // Features
    ini.SetBoolValue("Features", NameToStr(enableCustomInput), enableCustomInput);
    ini.SetBoolValue("Features", NameToStr(enableStances), enableStances);
    ini.SetBoolValue("Features", NameToStr(enableHoldSprint), enableHoldSprint);
    ini.SetBoolValue("Features", NameToStr(enableHoldSneak), enableHoldSneak);
    ini.SetBoolValue("Features", NameToStr(enableReverseHorseAttack), enableReverseHorseAttack);
    ini.SetLongValue("Features", NameToStr(enableSheatheAttack), enableSheatheAttack);

    // Stances
    ini.SetLongValue("Stances", "StancesModifier", StancesModifier);
    ini.SetLongValue("Stances", "ChangeToLow", ChangeToLow);
    ini.SetLongValue("Stances", "ChangeToMid", ChangeToMid);
    ini.SetLongValue("Stances", "ChangeToHigh", ChangeToHigh);

    // Vanilla Input
    ini.SetLongValue("Vanilla", NameToStr(normalAttack), normalAttack);
    ini.SetLongValue("Vanilla", NameToStr(powerAttack), powerAttack);
    ini.SetLongValue("Vanilla", NameToStr(block), block);
    ini.SetLongValue("Vanilla", NameToStr(MagicModifier), MagicModifier);
    ini.SetLongValue("Vanilla", NameToStr(BFCO_SpecialAttackModifier), BFCO_SpecialAttackModifier);
    ini.SetLongValue("Vanilla", NameToStr(BFCO_ComboAttack), BFCO_ComboAttack);

    ini.SetLongValue("Vanilla", NameToStr(altTweenMenu), altTweenMenu);
    ini.SetLongValue("Vanilla", NameToStr(altTogglePOV), altTogglePOV);

    // Expand Input
    ini.SetLongValue("Expand", NameToStr(warAsh), warAsh);
    ini.SetLongValue("Expand", NameToStr(warAshModifier), warAshModifier);

    ini.SetLongValue("Expand", NameToStr(altZoomIn), altZoomIn);
    ini.SetLongValue("Expand", NameToStr(altZoomOut), altZoomOut);
    ini.SetLongValue("Expand", NameToStr(zoomModifier), zoomModifier);

    // Modifier Input
    ini.SetLongValue("Modifier", VarUtils::userEvent->forward.c_str(),
                     needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->forward)]);
    ini.SetLongValue("Modifier", VarUtils::userEvent->strafeLeft.c_str(),
                     needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->strafeLeft)]);
    ini.SetLongValue("Modifier", VarUtils::userEvent->back.c_str(),
                     needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->back)]);
    ini.SetLongValue("Modifier", VarUtils::userEvent->strafeRight.c_str(),
                     needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->strafeRight)]);
    ini.SetLongValue("Modifier", VarUtils::userEvent->rightAttack.c_str(),
                     needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->rightAttack)]);
    ini.SetLongValue("Modifier", VarUtils::userEvent->leftAttack.c_str(),
                     needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->leftAttack)]);
    ini.SetLongValue("Modifier", VarUtils::userEvent->activate.c_str(),
                     needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->activate)]);
    ini.SetLongValue("Modifier", VarUtils::userEvent->readyWeapon.c_str(),
                     needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->readyWeapon)]);
    ini.SetLongValue("Modifier", VarUtils::userEvent->tweenMenu.c_str(),
                     needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->tweenMenu)]);
    ini.SetLongValue("Modifier", VarUtils::userEvent->togglePOV.c_str(),
                     needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->togglePOV)]);
    ini.SetLongValue("Modifier", VarUtils::userEvent->jump.c_str(),
                     needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->jump)]);
    ini.SetLongValue("Modifier", VarUtils::userEvent->sprint.c_str(),
                     needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->sprint)]);
    ini.SetLongValue("Modifier", VarUtils::userEvent->shout.c_str(),
                     needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->shout)]);
    ini.SetLongValue("Modifier", VarUtils::userEvent->sneak.c_str(),
                     needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->sneak)]);
    ini.SetLongValue("Modifier", VarUtils::userEvent->run.c_str(),
                     needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->run)]);
    ini.SetLongValue("Modifier", VarUtils::userEvent->toggleRun.c_str(),
                     needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->toggleRun)]);
    ini.SetLongValue("Modifier", VarUtils::userEvent->autoMove.c_str(),
                     needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->autoMove)]);
    ini.SetLongValue("Modifier", VarUtils::userEvent->favorites.c_str(),
                     needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->favorites)]);
    ini.SetLongValue("Modifier", VarUtils::userEvent->quicksave.c_str(),
                     needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->quicksave)]);
    ini.SetLongValue("Modifier", VarUtils::userEvent->quickload.c_str(),
                     needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->quickload)]);
    ini.SetLongValue("Modifier", VarUtils::userEvent->wait.c_str(),
                     needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->wait)]);
    ini.SetLongValue("Modifier", VarUtils::userEvent->journal.c_str(),
                     needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->journal)]);
    ini.SetLongValue("Modifier", VarUtils::userEvent->pause.c_str(),
                     needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->pause)]);
    ini.SetLongValue("Modifier", VarUtils::userEvent->quickInventory.c_str(),
                     needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->quickInventory)]);
    ini.SetLongValue("Modifier", VarUtils::userEvent->quickMagic.c_str(),
                     needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->quickMagic)]);
    ini.SetLongValue("Modifier", VarUtils::userEvent->quickStats.c_str(),
                     needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->quickStats)]);
    ini.SetLongValue("Modifier", VarUtils::userEvent->quickMap.c_str(),
                     needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->quickMap)]);
}
} // namespace Config