#pragma once
#include "PCH.h"
#include "config.h"
#include "hook.h"

using namespace RE;
using namespace REL;

namespace KeyDecoder
{
    typedef struct
    {
        uint32_t code;
        float value;
    } RawInput;

    enum ActionType
    {
        MenuOpen = 0,
        AttackBlock,
        AutoMove,
        Misc,

        End = 65536
    };

    void RawQueuePusher(RawInput);
    void buildKeySearchList();
    void actionDistributor();
}