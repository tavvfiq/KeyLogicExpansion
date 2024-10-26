#include "utils.h"

namespace KeyUtils
{
    uint32_t GetVanillaKeyMap(RE::BSFixedString userEvent) {
        auto keyBoard =  Var::ctrlMap->GetMappedKey(userEvent, RE::INPUT_DEVICE::kKeyboard);
        auto mouse = Var::ctrlMap->GetMappedKey(userEvent, RE::INPUT_DEVICE::kMouse);
        if (keyBoard != 255)
            return keyBoard;
        if (mouse != 255)
            return mouse + MouseButtonOffset;
        return 0;
    }

    uint32_t GetEventKeyMap(ButtonEvent *a_event)
    {
        switch (a_event->GetDevice())
        {
        case RE::INPUT_DEVICE::kKeyboard:
            return a_event->GetIDCode();
        case RE::INPUT_DEVICE::kMouse:
            return MouseButtonOffset + a_event->GetIDCode();
        default:
            return 0;
        }
    }
}

namespace TimeUtils
{
    uint64_t GetTime()
    {
        auto now = std::chrono::steady_clock::now();
        uint64_t time = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count();
        return time;
    }
}