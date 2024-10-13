#pragma once
#include "PCH.h"

using namespace RE;
using namespace REL;

namespace keyQueue {

    typedef struct {
        uint32_t idCode;
        RE::BSFixedString userEvent;
    }keyInput;

    extern std::deque<keyInput> keyQueue;
}