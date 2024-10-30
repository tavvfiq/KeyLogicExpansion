#include "stances.h"

namespace Stances
{
bool enableStances;
uint32_t StancesModfier;
uint32_t ChangeToLow;
uint32_t ChangeToMid;
uint32_t ChangeToHigh;

static RE::BGSPerk *currentStance;
std::vector<RE::TESForm *> StancesList;

void ChangeStanceTo(Stances stance)
{
    auto target = (RE::BGSPerk *)StancesList[stance];
    if (currentStance)
        VarUtils::player->RemoveTemporaryPerk(currentStance);
    if (target)
        VarUtils::player->ApplyTemporaryPerk(target);
    currentStance = target;
}

void init()
{
    bool inited = false;
    if (!inited)
    {
        RE::BGSPerk *target = (RE::BGSPerk *)StancesList[Stances::Mid];
        currentStance = target;
        if (target)
            VarUtils::player->ApplyTemporaryPerk(target);
    }
}
} // namespace Stances