#pragma once

using namespace RE;
using namespace REL;

namespace Var
{
    typedef struct
    {
        uint32_t chargeable;
        uint32_t firstKey;
        uint32_t secondKey;
        uint32_t shortKey;
        uint32_t priority;
    } AltKeyMap;
    
    typedef struct
    {
        uint32_t code;
        float value;
    } RawInput;

    enum PressType
    {
        release = 0,
        disable,
        click,
        hold,
        processed
    };

    enum ActionType
    {
        MenuOpen = 0,
        Attack,
        Block,
        AutoMove,
        Misc
    };

    extern RE::PlayerCharacter *player;
    extern PlayerControls *pCtrl;
    extern ControlMap *ctrlMap;
    extern UserEvents *userEvent;
    extern UI* ui;

    extern BGSAction* RightAttack;
	extern BGSAction* RightPowerAttack;
	extern BGSAction* DualPowerAttack;

    void init();
}