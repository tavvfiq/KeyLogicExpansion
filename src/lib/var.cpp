#include "var.h"

using namespace RE;
using namespace REL;

namespace Var
{
    RE::PlayerCharacter *p = nullptr;
    PlayerControls *pc = nullptr;
    ControlMap *im = nullptr;
    UserEvents *inputString = nullptr;

    void init()
    {
        p = RE::PlayerCharacter::GetSingleton();
        pc = PlayerControls::GetSingleton();
        im = ControlMap::GetSingleton();
        inputString = UserEvents::GetSingleton();
    }
}