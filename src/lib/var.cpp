#include "var.h"

using namespace RE;
using namespace REL;

namespace Var
{
    RE::PlayerCharacter *player = nullptr;
    PlayerControls *pCtrl = nullptr;
    ControlMap *ctrlMap = nullptr;
    UserEvents *userEvent = nullptr;
    UI *ui = nullptr;

    BGSAction* RightAttack = nullptr;
	BGSAction* RightPowerAttack = nullptr;
	BGSAction* DualPowerAttack = nullptr;

    void init()
    {
        player = RE::PlayerCharacter::GetSingleton();
        pCtrl = PlayerControls::GetSingleton();
        ctrlMap = ControlMap::GetSingleton();
        userEvent = UserEvents::GetSingleton();
        ui = UI::GetSingleton();

        BGSAction* RightAttack = (BGSAction*)TESForm::LookupByID(0x13005);
        BGSAction* RightPowerAttack = (BGSAction*)TESForm::LookupByID(0x13383);
        BGSAction* DualPowerAttack = (BGSAction*)TESForm::LookupByID(0x2E2F7);
    }
}