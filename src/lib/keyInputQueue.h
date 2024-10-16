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

    extern std::deque<RawInput> rawQueue;

    void buildKeySearchList();

    void inputDecoder();
    void actionDecoder();
}