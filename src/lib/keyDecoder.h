#pragma once
#include "PCH.h"
#include "actionDecoder.h"
#include "config.h"
#include "hook.h"

using namespace RE;
using namespace REL;

namespace KeyDecoder
{
    using RawInput = Var::RawInput;
    using PressType = Var::PressType;
    using ActionType = Var::ActionType;
    
    void RawQueuePusher(RawInput);
    void buildKeySearchList();
    void actionDistributor();
}