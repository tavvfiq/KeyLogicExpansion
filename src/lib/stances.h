#pragma once
#include "config.h"

namespace Stances
{
extern bool enableStances;

extern std::vector<RE::FormID> StancesList;

enum Stances
{
    Sheathe = 0,
    Low,
    Mid,
    High
};

void ChangeStanceTo(Stances stance);

void init();

} // namespace Stances