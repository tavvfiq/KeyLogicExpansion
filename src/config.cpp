#include "H/config.h"

namespace Config {
    bool loadIni() {
        CSimpleIniA ini;
        if (std::filesystem::exists(path)) {
            logger::warn("ini file not exist, will generate default.");
            return createDefaultIni();
        }
        SI_Error rc = ini.LoadFile(path.c_str());
    }
    bool createDefaultIni() {
        return true;
    }
}