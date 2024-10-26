#include "utils.h"

namespace VarUtils
{
    RE::PlayerCharacter *player = nullptr;
    PlayerControls *pCtrl = nullptr;
    ControlMap *ctrlMap = nullptr;
    UserEvents *userEvent = nullptr;
    UI *ui = nullptr;

    void init()
    {
        player = RE::PlayerCharacter::GetSingleton();
        pCtrl = PlayerControls::GetSingleton();
        ctrlMap = ControlMap::GetSingleton();
        userEvent = UserEvents::GetSingleton();
        ui = UI::GetSingleton();
    }
}

namespace KeyUtils
{
    uint32_t GetVanillaKeyMap(RE::BSFixedString userEvent)
    {
        auto keyBoard = VarUtils::ctrlMap->GetMappedKey(userEvent, RE::INPUT_DEVICE::kKeyboard);
        auto mouse = VarUtils::ctrlMap->GetMappedKey(userEvent, RE::INPUT_DEVICE::kMouse);
        if (keyBoard != 0xFF)
            return keyBoard;
        if (mouse != 0xFF)
            return mouse + Mouse::MouseButtonOffset;
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