#include "stances.h"

namespace Stances
{
bool enableStances;
uint32_t ChangeToLow;
uint32_t ChangeToMid;
uint32_t ChangeToHigh;

std::vector<RE::TESForm *> StancesList;

void ChangeStanceTo(Stances stance)
{
    for (auto item : StancesList)
    {
        if (!item)
            continue;
        if (VarUtils::player->HasPerk((RE::BGSPerk *)item))
            VarUtils::player->RemovePerk((RE::BGSPerk *)item);
    }
    auto target = (RE::BGSPerk *)StancesList[stance];
    if (target)
        VarUtils::player->AddPerk(target);
}

void init()
{
    bool inited = false;
    for (auto item : StancesList)
    {
        if (!item)
            continue;
        if (VarUtils::player->HasPerk((RE::BGSPerk *)item))
            inited = true;
    }
    if (!inited)
    {
        RE::BGSPerk *target;
        target = (RE::BGSPerk *)StancesList[Stances::Mid];
        if (target)
            VarUtils::player->AddPerk(target);
    }
}
} // namespace Stances