#include "utils.h"

namespace KeyUtils
{
    void GetVanillaKeyMap(uint32_t *keyArr, RE::BSFixedString keyInput)
    {
        keyArr[INPUT_DEVICE::KeyBoard] = Var::ctrlMap->GetMappedKey(keyInput, RE::INPUT_DEVICE::kKeyboard);
        keyArr[INPUT_DEVICE::Mouse] = MouseButtonOffset + Var::ctrlMap->GetMappedKey(keyInput, RE::INPUT_DEVICE::kMouse);
        // keyArr[INPUT_DEVICE::Gamepad] = GamepadButtonOffset + Var::im->GetMappedKey(keyInput, RE::INPUT_DEVICE::kGamepad);
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
            return NULL;
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