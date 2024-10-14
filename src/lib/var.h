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

    typedef struct {
        RE::BSFixedString userEvent;
        bool useShortKey;
        uint32_t firstKey;
        uint32_t SecondKey;
        uint32_t shortKey;
        uint32_t priority;
    }AltKeyMap;
}