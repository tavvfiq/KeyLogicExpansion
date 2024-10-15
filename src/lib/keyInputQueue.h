#pragma once
#include "PCH.h"
#include "config.h"

using namespace RE;
using namespace REL;

namespace KeyQueue
{

    enum ActionType
    {
        MenuOpen = 0,
        AttackBlock,
        AutoMove,
        Misc,

        End = 65536
    };

    typedef struct
    {
        uint32_t code;
        float held;
    } keyInput;

    extern std::deque<keyInput> keyQueue;
    static std::mutex mtx;

    void buildKeySearchList();

    void pushInKey(keyInput key);
    void updateKey(keyInput key);
    void dumpKey(std::unique_lock<std::mutex> &&lock);
    void decodeAction();


}