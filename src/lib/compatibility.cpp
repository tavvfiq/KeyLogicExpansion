#include "compatibility.h"

namespace Compatibility
{
bool BFCO = false;
RE::TESIdleForm *BFCO_PowerAttackSprint;
RE::TESIdleForm *BFCO_PowerAttackJump1H;
RE::TESIdleForm *BFCO_PowerAttackJump2H;
RE::TESIdleForm *BFCO_NormalAttackSpecial;
RE::TESIdleForm *BFCO_PowerAttackSpecial;
RE::TESIdleForm *BFCO_NormalAttackSwim;
RE::TESIdleForm *BFCO_PowerAttackSwim;
RE::TESIdleForm *BFCO_ComboAttack;

bool MCO = false;
RE::TESIdleForm *MCO_PowerAttackSprint;

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
    {
        logger::trace("Detecting BFCO installed, compatibility with BFCO.");

        // ActionList::PowerAttackRight = (RE::TESIdleForm *)FormUtils::GetForm("SCSI-ACTbfco-Main.esp|0x008C5");

        BFCO_PowerAttackSprint = (RE::TESIdleForm *)FormUtils::GetForm("SCSI-ACTbfco-Main.esp|0x008BE");
        BFCO_PowerAttackJump1H = (RE::TESIdleForm *)FormUtils::GetForm("SCSI-ACTbfco-Main.esp|0x008DE");
        BFCO_PowerAttackJump2H = (RE::TESIdleForm *)FormUtils::GetForm("SCSI-ACTbfco-Main.esp|0x008DF");

        BFCO_NormalAttackSpecial = (RE::TESIdleForm *)FormUtils::GetForm("SCSI-ACTbfco-Main.esp|0x008A7");
        BFCO_PowerAttackSpecial = (RE::TESIdleForm *)FormUtils::GetForm("SCSI-ACTbfco-Main.esp|0x008AF");

        BFCO_NormalAttackSwim = (RE::TESIdleForm *)FormUtils::GetForm("SCSI-ACTbfco-Main.esp|0x00916");
        BFCO_PowerAttackSwim = (RE::TESIdleForm *)FormUtils::GetForm("SCSI-ACTbfco-Main.esp|0x00917");

        BFCO_ComboAttack = (RE::TESIdleForm *)FormUtils::GetForm("SCSI-ACTbfco-Main.esp|0x008BF");
    }
    // MCO: Attack_DXP.esp
    MCO = ModSupport::ModExist(std::string("Attack_DXP.esp"));
    if (MCO)
    {
        logger::trace("Detecting MCO installed, compatibility with MCO.");
        MCO_PowerAttackSprint = (RE::TESIdleForm *)TESForm::LookupByID(0xEC3CC);
    }

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
