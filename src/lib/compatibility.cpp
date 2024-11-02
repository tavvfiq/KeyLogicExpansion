#include "compatibility.h"

namespace Compatibility
{
bool BFCO = false;
bool MCO = false;

bool ELDEN = false;
bool CanUseWarAsh = false;
std::vector<TESForm *> warAshList;

bool normalAttackWin = false;
bool powerAttackWin = false;
bool recoverWin = false;

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
bool IsWarAsh(TESForm *form)
{
    auto res = std::find_if(warAshList.begin(), warAshList.end(), [form](TESForm *i) { return form == i; });
    if (res == warAshList.end())
        return false;
    return true;
}

void init()
{
    // BFCO: SCSI-ACTbfco-Main.esp
    BFCO = ModSupport::ModExist(std::string("SCSI-ACTbfco-Main.esp"));
    if (BFCO)
        logger::trace("Detecting BFCO installed, compatibility with BFCO.");
    // MCO: Attack_DXP.esp
    MCO = ModSupport::ModExist(std::string("Attack_DXP.esp"));
    if (MCO)
        logger::trace("Detecting MCO installed, compatibility with MCO.");

    if (BFCO && MCO)
        logger::trace("Why you install BFCO and MCO at the same time?");

    // EldenRim: EldenSkyrim_RimSkills.esp
    ELDEN = ModSupport::ModExist("EldenSkyrim_RimSkills.esp");
    if (ELDEN)
    {
        logger::trace("Detecting EldenSkyrim installed, can enable WarAsh Now.");
        CanUseWarAsh = true;
        // Ash of War - Strength
        warAshList.push_back(FormUtils::GetForm("EldenSkyrim_RimSkills.esp|0x01023"));
        // Ash of War - Dexterity
        warAshList.push_back(FormUtils::GetForm("EldenSkyrim_RimSkills.esp|0x01025"));
        // Ash of War - Arcane
        warAshList.push_back(FormUtils::GetForm("EldenSkyrim_RimSkills.esp|0x01026"));
        // Ash of War - Mystery
        warAshList.push_back(FormUtils::GetForm("EldenSkyrim_RimSkills.esp|0x01027"));
        // Ash of War - Fortune
        warAshList.push_back(FormUtils::GetForm("EldenSkyrim_RimSkills.esp|0x01028"));
    }
}
} // namespace Compatibility
