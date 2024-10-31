#pragma once
#include "config.h"

namespace Compatibility
{
extern bool BFCO;
extern bool MCO;

extern bool ELDEN;

extern bool normalAttackWin;
extern bool powerAttackWin;

bool CanNormalAttack();
bool CanPowerAttack();
bool CanRecovery();

bool IsWarAsh(RE::FormID formId);

void init();
} // namespace Compatibility