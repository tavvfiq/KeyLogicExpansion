#include "stances.h"

namespace Stances
{
bool enableStances;
uint32_t StancesModfier;
uint32_t ChangeToLow;
uint32_t ChangeToMid;
uint32_t ChangeToHigh;

std::vector<RE::TESForm *> StancesList;
Stances currentStance = Stances::Null;

void ChangeStanceTo(Stances stance)
{
    auto target = (RE::BGSPerk *)StancesList[stance];
    if (StancesList[currentStance])
        VarUtils::player->RemoveTemporaryPerk((RE::BGSPerk *)StancesList[currentStance]);
    if (target)
        VarUtils::player->ApplyTemporaryPerk(target);
    currentStance = stance;
}

void init()
{
    bool inited = false;
    for (int i = 1; i < StancesList.size(); i++)
        if (VarUtils::player->HasPerk((RE::BGSPerk *)i) || StancesList[i] == nullptr)
        {
            inited = true;
            currentStance = (Stances)i;
            break;
        }
    if (!inited)
    {
        RE::BGSPerk *target = (RE::BGSPerk *)StancesList[Stances::Mid];
        currentStance = Stances::Mid;
        if (target)
            VarUtils::player->ApplyTemporaryPerk(target);
    }
}
} // namespace Stances