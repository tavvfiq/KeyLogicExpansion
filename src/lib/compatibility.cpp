#include "compatibility.h"

namespace Compatibility
{
bool BFCO;
bool MCO;

bool normalAttackWin;
bool powerAttackWin;
bool recoverWin;

bool CanNormalAttack()
{
    return normalAttackWin;
}
bool CanPowerAttack()
{
    return powerAttackWin;
}
bool CanRecovery()
{
    bool BFCO_Recovery;
    bool MCO_Recovery;

    return true;
}

bool ModExist(std::string modName)
{
    TESDataHandler *dataHandler = TESDataHandler::GetSingleton();
    TESFile *modFile = nullptr;
    for (auto it = dataHandler->files.begin(); it != dataHandler->files.end(); it++)
    {
        TESFile *f = *it;
        if (strcmp(f->fileName, modName.c_str()) == 0)
        {
            modFile = f;
            break;
        }
    }
    if (!modFile)
        return false;
    return true;
}

void init()
{
    // BFCO: SCSI-ACTbfco-Main.esp
    BFCO = ModExist(std::string("SCSI-ACTbfco-Main.esp"));
    // MCO: Attack_DXP.esp
    MCO = ModExist(std::string("Attack_DXP.esp"));
    if (BFCO && MCO)
        logger::trace("Why you install BFCO and MCO at the same time?");
}
} // namespace Compatibility
