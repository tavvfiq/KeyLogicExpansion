#pragma once
#include "PCH.h"
#include "hook.h"

using namespace RE;
using namespace REL;

namespace ActionDecoder
{
    using PressType = Var::PressType;
    using ActionType = Var::ActionType;

    void ClickAction(BSFixedString userEvent, ActionType type);
    void HoldAction(BSFixedString userEvent, ActionType type);
    void ChargeAction(BSFixedString userEvent, ActionType type, PressType& press, uint32_t& chargeable);
}