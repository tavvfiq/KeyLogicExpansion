#pragma once
#include "PCH.h"
#include "actionDecoder.h"
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

    void RawQueuePusher(RawInput);
    void buildKeySearchList();
    void actionDistributor();
}