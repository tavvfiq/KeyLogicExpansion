#include "config.h"

namespace Config
{
CSimpleIniA ini;
const static std::string ini_path = "Data/SKSE/Plugins/KeyLogicExpansion.ini";

// Features
bool enableCustomInput = std::ref(Custom::enableCustomInput);
bool enableStances = std::ref(Stances::enableStances);
bool enableHoldSprint;
bool enableHoldSneak;
uint32_t enableSheatheAttack;
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
std::unordered_set<uint32_t> listModifier;
bool isModifier(uint32_t code)
{
    return listModifier.find(code) == listModifier.end();
}

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
    enableStances = ini.GetBoolValue("Features", NameToStr(enableStances), true);
    enableHoldSprint = ini.GetBoolValue("Features", NameToStr(enableHoldSprint), false);
    enableHoldSneak = ini.GetBoolValue("Features", NameToStr(enableHoldSneak), false);
    enableSheatheAttack = ini.GetLongValue("Features", NameToStr(enableSheatheAttack), 0);
    enableReverseHorseAttack = ini.GetBoolValue("Features", NameToStr(enableReverseHorseAttack), false);

    // Vanilla Input
    normalAttack = ini.GetLongValue("Vanilla", NameToStr(normalAttack), KeyUtils::Mouse::MouseButtonLeft);
    powerAttack = ini.GetLongValue("Vanilla", NameToStr(powerAttack), KeyUtils::Mouse::MouseButtonRight);
    block = ini.GetLongValue("Vanilla", NameToStr(block), KeyUtils::KeyBoard::Tab);

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
        listModifier.insert(modifier);
        if (item.pItem == VarUtils::userEvent->tweenMenu && altTweenMenu)
            trigger = altTweenMenu;
        else if (item.pItem == VarUtils::userEvent->togglePOV && altTogglePOV)
            trigger = altTogglePOV;
        logger::trace("{} {} {}", item.pItem, trigger, modifier);
        if (trigger)
            needModifier.insert(std::make_pair(trigger, modifier));
    }
    needModifier.insert(std::make_pair(warAsh, warAshModifier));
    listModifier.insert(warAshModifier);
    if (altZoomIn)
        needModifier.insert(std::make_pair(altZoomIn, zoomModifier));
    else
        needModifier.insert(std::make_pair(KeyUtils::Mouse::MouseWheelUp, zoomModifier));
    if (altZoomOut)
        needModifier.insert(std::make_pair(altZoomOut, zoomModifier));
    else
        needModifier.insert(std::make_pair(KeyUtils::Mouse::MouseWheelDown, zoomModifier));
    listModifier.insert(zoomModifier);

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
    ini.SetBoolValue("Features", NameToStr(enableStances), true,
                     ";Enable Stances Supported by KLE, need EnableCustomInput = true or won't work.\n;Contain 4 types "
                     "of stance: High, Mid, Low, and Sheathe.");
    ini.SetBoolValue("Features", NameToStr(enableHoldSprint), false, ";Change enable sprint when you hold sprint key");
    ini.SetBoolValue("Features", NameToStr(enableHoldSneak), false,
                     ";Same as EnableHoldSprint, Change enable sneak when you hold sneak key");
    ini.SetLongValue(
        "Features", NameToStr(enableSheatheAttack), 0,
        ";Make you can attack when you press this key, NOT completed.\n;0 means disable, other number means a "
        "keycode\n;"
        "when you press the key and Attack or PowerAttack or even Sheathe Key, you will do a SheatheAttack\n;Note: "
        "Press with Sheathe Key can do SheatheAttack when you are NOT in Sheathe status.");
    ini.SetBoolValue(
        "Features", NameToStr(enableReverseHorseAttack), false,
        ";Reverse your HorseAttack diretion, if enable this, left key attack left, right key attack right");

    // Vanilla Input
    ini.SetLongValue(
        "Vanilla", NameToStr(normalAttack), KeyUtils::Mouse::MouseButtonLeft,
        ";separete block key from Attack, still have some problems, don't use blockbash, it is overpowered");
    ini.SetLongValue("Vanilla", NameToStr(powerAttack), KeyUtils::Mouse::MouseButtonRight);
    ini.SetLongValue("Vanilla", NameToStr(block), KeyUtils::KeyBoard::Tab);

    ini.SetLongValue("Vanilla", NameToStr(altTweenMenu), 0,
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
} // namespace Config