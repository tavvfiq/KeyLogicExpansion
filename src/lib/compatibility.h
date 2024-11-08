#pragma once
#include "config.h"

namespace Compatibility
{
extern bool BFCO;
extern RE::TESIdleForm *BFCO_PowerAttackSprint;
extern RE::TESIdleForm *BFCO_PowerAttackJump1H;
extern RE::TESIdleForm *BFCO_PowerAttackJump2H;
extern RE::TESIdleForm *BFCO_NormalAttackSpecial;
extern RE::TESIdleForm *BFCO_PowerAttackSpecial;
extern RE::TESIdleForm *BFCO_NormalAttackSwim;
extern RE::TESIdleForm *BFCO_PowerAttackSwim;
extern RE::TESIdleForm *BFCO_ComboAttack;

extern bool MCO;

extern bool ELDEN;
extern bool CanUseWarAsh;

extern bool normalAttackWin;
extern bool powerAttackWin;

bool CanNormalAttack();
bool CanPowerAttack();
bool CanRecovery();

bool IsWarAsh(TESForm *form);

void init();
} // namespace Compatibility