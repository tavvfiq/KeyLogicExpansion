#include "compatibility.h"

namespace Compatibility
{
bool BFCO;
bool MCO;

bool ELDEN;
std::vector<TESForm *> warAshList;

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
bool IsWarAsh(RE::FormID formId)
{
    auto res = std::find_if(warAshList.begin(), warAshList.end(), [formId](TESForm *i) { return formId == i->formID; });
    if (res == warAshList.end())
        return false;
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
    if (BFCO)
        logger::trace("Detecting BFCO installed, compatibility with BFCO.");
    // MCO: Attack_DXP.esp
    MCO = ModExist(std::string("Attack_DXP.esp"));
    if (MCO)
        logger::trace("Detecting MCO installed, compatibility with MCO.");

    if (BFCO && MCO)
        logger::trace("Why you install BFCO and MCO at the same time?");

    // EldenRim: EldenSkyrim_RimSkills.esp
    ELDEN = ModExist("EldenSkyrim_RimSkills.esp");
    if (ELDEN)
    {
        logger::trace("Detecting EldenSkyrim installed, can enable WarAsh Now.");
        warAshList.push_back(FormUtils::GetForm("EldenSkyrim_RimSkills.esp|0x0087E"));
        warAshList.push_back(FormUtils::GetForm("EldenSkyrim_RimSkills.esp|0x0087F"));
        warAshList.push_back(FormUtils::GetForm("EldenSkyrim_RimSkills.esp|0x00880"));
        warAshList.push_back(FormUtils::GetForm("EldenSkyrim_RimSkills.esp|0x00881"));
        warAshList.push_back(FormUtils::GetForm("EldenSkyrim_RimSkills.esp|0x00FC8"));
    }
}
} // namespace Compatibility
