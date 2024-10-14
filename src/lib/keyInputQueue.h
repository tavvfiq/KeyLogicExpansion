#pragma once
#include "PCH.h"

using namespace RE;
using namespace REL;

namespace keyQueue {

    enum ActionType {
        Menu = 0,
        Combat,
        Misc
    };

    typedef struct {
        uint32_t code;
        float held;
    }keyInput;

    typedef struct {
        RE::BSFixedString userEvent;
        ActionType type;
    }Action;

    extern std::deque<keyInput> keyQueue;
    static std::deque<Action> actQueue;
    static std::mutex mtx;

    void pushInKey(keyInput key);
    void updateKey(keyInput key);
    void dumpKey(std::unique_lock<std::mutex>&& lock);
    void doAction();
}