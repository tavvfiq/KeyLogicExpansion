#include "compatibility.h"

namespace Compatibility
{
bool BFCO = false;
ActionList::Animation BFCO_PowerAttackJump1H;
ActionList::Animation BFCO_PowerAttackJump2H;
ActionList::Animation BFCO_NormalAttackSpecial;
ActionList::Animation BFCO_PowerAttackSpecial;
ActionList::Animation BFCO_NormalAttackSwim;
ActionList::Animation BFCO_PowerAttackSwim;
ActionList::Animation BFCO_ComboAttack;

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
    {
        logger::trace("Detecting BFCO installed, compatibility with BFCO.");

        ActionList::PowerAttackRight.type = ActionList::ActionType::Idle;
        ActionList::PowerAttackRight.idle = (RE::TESIdleForm *)FormUtils::GetForm("SCSI-ACTbfco-Main.esp|0x008C5");

        // BFCO_PowerAttackSprint = (ActionList::Action)FormUtils::GetForm("SCSI-ACTbfco-Main.esp|0x008BE");
        // BFCO_PowerAttackJump1H = (ActionList::Action)FormUtils::GetForm("SCSI-ACTbfco-Main.esp|0x008DE");
        // BFCO_PowerAttackJump2H = (ActionList::Action)FormUtils::GetForm("SCSI-ACTbfco-Main.esp|0x008DF");

        BFCO_NormalAttackSpecial.type = ActionList::ActionType::Idle;
        BFCO_NormalAttackSpecial.idle = (RE::TESIdleForm *)FormUtils::GetForm("SCSI-ACTbfco-Main.esp|0x008A7");
        BFCO_NormalAttackSpecial.action = nullptr;
        BFCO_NormalAttackSpecial.event = "attackStartDualWield";

        BFCO_PowerAttackSpecial.type = ActionList::ActionType::Idle;
        BFCO_PowerAttackSpecial.idle = (RE::TESIdleForm *)FormUtils::GetForm("SCSI-ACTbfco-Main.esp|0x008AF");
        BFCO_PowerAttackSpecial.action = nullptr;
        BFCO_PowerAttackSpecial.event = "attackPowerStartDualWield";

        BFCO_NormalAttackSwim.type = ActionList::ActionType::Idle;
        BFCO_NormalAttackSwim.idle = (RE::TESIdleForm *)FormUtils::GetForm("SCSI-ACTbfco-Main.esp|0x00916");
        BFCO_NormalAttackSwim.action = nullptr;
        BFCO_NormalAttackSwim.event = "";

        BFCO_PowerAttackSwim.type = ActionList::ActionType::Idle;
        BFCO_PowerAttackSwim.idle = (RE::TESIdleForm *)FormUtils::GetForm("SCSI-ACTbfco-Main.esp|0x00917");
        BFCO_PowerAttackSwim.action = nullptr;
        BFCO_PowerAttackSwim.event = "";

        BFCO_ComboAttack.type = ActionList::ActionType::Idle;
        BFCO_ComboAttack.idle = (RE::TESIdleForm *)FormUtils::GetForm("SCSI-ACTbfco-Main.esp|0x008BF");
        BFCO_ComboAttack.action = nullptr;
        BFCO_ComboAttack.event = "attackPowerStartForward";
    }
    // MCO: Attack_DXP.esp
    MCO = ModSupport::ModExist(std::string("Attack_DXP.esp"));
    if (MCO)
    {
        logger::trace("Detecting MCO installed, compatibility with MCO.");
        // MCO_PowerAttackSprint = (ActionList::Action)TESForm::LookupByID(0xEC3CC);
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
