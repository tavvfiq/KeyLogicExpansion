#pragma once
#include "utils.h"

namespace Stances
{
extern bool enableStances;
extern uint32_t StancesModfier;
extern uint32_t ChangeToLow;
extern uint32_t ChangeToMid;
extern uint32_t ChangeToHigh;

enum Stances
{
    Null = 0,
    Low,
    Mid,
    High,
    Sheathe
};
extern std::vector<RE::TESForm *> StancesList;
extern Stances currentStance;

void ChangeStanceTo(Stances stance);
void init();
} // namespace Stances