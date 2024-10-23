#pragma once
#include "PCH.h"
#include "hook.h"

using namespace RE;
using namespace REL;

namespace ActionDecoder
{
    enum ActionType
    {
        MenuOpen = 0,
        AttackBlock,
        AutoMove,
        Misc,

        End = 65536
    };

    void ClickAction(BSFixedString userEvent, ActionType type);
}