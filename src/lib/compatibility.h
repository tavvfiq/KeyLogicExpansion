#pragma once
#include "config.h"

namespace Compatibility
{
extern bool BFCO;
extern bool MCO;

extern bool normalAttackWin;
extern bool powerAttackWin;

bool CanNormalAttack();
bool CanPowerAttack();
bool CanRecovery();

void init();
} // namespace Compatibility