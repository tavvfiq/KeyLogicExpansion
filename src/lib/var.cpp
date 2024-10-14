#include "var.h"

using namespace RE;
using namespace REL;

namespace Var
{
    RE::PlayerCharacter *player = nullptr;
    PlayerControls *pCtrl = nullptr;
    ControlMap *ctrlMap = nullptr;
    UserEvents *userEvent = nullptr;

    void init()
    {
        player = RE::PlayerCharacter::GetSingleton();
        pCtrl = PlayerControls::GetSingleton();
        ctrlMap = ControlMap::GetSingleton();
        userEvent = UserEvents::GetSingleton();
    }
}