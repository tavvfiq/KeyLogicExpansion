#pragma once
#include "config.h"

namespace Stances
{
extern bool enableStances;
extern uint32_t ChangeToLow;
extern uint32_t ChangeToMid;
extern uint32_t ChangeToHigh;

extern std::vector<RE::TESForm *> StancesList;

enum Stances
{
    Low = 0,
    Mid,
    High
};

void ChangeStanceTo(Stances stance);

void init();

} // namespace Stances