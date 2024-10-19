#pragma once
#include "PCH.h"
#include "config.h"
#include "hook.h"

using namespace RE;
using namespace REL;

namespace KeyQueue
{
    typedef struct
    {
        uint32_t code;
        float held;
        float value;
    } RawInput;

    void RawQueuePusher(RawInput);
    void buildKeySearchList();
    void actionDecoder();
}