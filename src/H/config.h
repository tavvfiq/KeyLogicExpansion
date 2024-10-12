#include "PCH.h"
#include "SimpleIni.h"

using namespace RE;
using namespace REL;
using namespace SKSE::log;

namespace Config {

    static std::string path = "Data/MCM/Config/KeyLogicExpansion/settings.ini";

    bool useAltTweenMenu = false;
    bool useAltTogglePOV = false;
    bool useAltAttack = false;
    bool useAltPowerAttack = false;
    bool useAltBlock = false;

    bool disableOriAutoMove = false;
    bool disableOriToggleRunWalk = false;
    bool disableOriQuickInventory = false;
    bool disableOriQuickMagic = false;
    bool disableOriQuickStats = false;
    bool disableOriQuickMap = false;

    bool loadIni();
    bool createDefaultIni();
}