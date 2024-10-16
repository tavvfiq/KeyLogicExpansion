#pragma once

using namespace RE;
using namespace REL;

namespace Var
{
    extern RE::PlayerCharacter *player;
    extern PlayerControls *pCtrl;
    extern ControlMap *ctrlMap;
    extern UserEvents *userEvent;

    void init();
}