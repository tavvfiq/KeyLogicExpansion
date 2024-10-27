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
    typedef struct
    {
        uint64_t clock;
        bool status;
    } Clock;

    std::unordered_map<uint32_t, Clock> keyStatus;
    uint32_t deleteTime = 20;

    uint32_t GetWinKeyMap(uint32_t code)
    {
        switch (code)
        {
        case KeyBoard::ESC:
            return SKSE::WinAPI::VK_ESCAPE;
        case KeyBoard::MAIN1:
            return SKSE::WinAPI::VK_1;
        case KeyBoard::MAIN2:
            return SKSE::WinAPI::VK_2;
        case KeyBoard::MAIN3:
            return SKSE::WinAPI::VK_3;
        case KeyBoard::MAIN4:
            return SKSE::WinAPI::VK_4;
        case KeyBoard::MAIN5:
            return SKSE::WinAPI::VK_5;
        case KeyBoard::MAIN6:
            return SKSE::WinAPI::VK_6;
        case KeyBoard::MAIN7:
            return SKSE::WinAPI::VK_7;
        case KeyBoard::MAIN8:
            return SKSE::WinAPI::VK_8;
        case KeyBoard::MAIN9:
            return SKSE::WinAPI::VK_9;
        case KeyBoard::MAIN0:
            return SKSE::WinAPI::VK_0;
        case KeyBoard::Minus:
            return SKSE::WinAPI::VK_OEM_MINUS;
        case KeyBoard::Equals:
            return SKSE::WinAPI::VK_OEM_PLUS;
        case KeyBoard::Backspace:
            return SKSE::WinAPI::VK_BACK;
        case KeyBoard::Tab:
            return SKSE::WinAPI::VK_TAB;
        case KeyBoard::Q:
            return SKSE::WinAPI::VK_Q;
        case KeyBoard::W:
            return SKSE::WinAPI::VK_W;
        case KeyBoard::E:
            return SKSE::WinAPI::VK_E;
        case KeyBoard::R:
            return SKSE::WinAPI::VK_R;
        case KeyBoard::T:
            return SKSE::WinAPI::VK_T;
        case KeyBoard::Y:
            return SKSE::WinAPI::VK_Y;
        case KeyBoard::U:
            return SKSE::WinAPI::VK_U;
        case KeyBoard::I:
            return SKSE::WinAPI::VK_I;
        case KeyBoard::O:
            return SKSE::WinAPI::VK_O;
        case KeyBoard::P:
            return SKSE::WinAPI::VK_P;
        case KeyBoard::LeftBracket:
            return SKSE::WinAPI::VK_OEM_4;
        case KeyBoard::RightBracket:
            return SKSE::WinAPI::VK_OEM_6;
        case KeyBoard::Enter:
            return SKSE::WinAPI::VK_RETURN;
        case KeyBoard::LeftCtrl:
            return SKSE::WinAPI::VK_LCONTROL;
        case KeyBoard::A:
            return SKSE::WinAPI::VK_A;
        case KeyBoard::S:
            return SKSE::WinAPI::VK_S;
        case KeyBoard::D:
            return SKSE::WinAPI::VK_D;
        case KeyBoard::F:
            return SKSE::WinAPI::VK_F;
        case KeyBoard::G:
            return SKSE::WinAPI::VK_G;
        case KeyBoard::H:
            return SKSE::WinAPI::VK_H;
        case KeyBoard::J:
            return SKSE::WinAPI::VK_J;
        case KeyBoard::K:
            return SKSE::WinAPI::VK_K;
        case KeyBoard::L:
            return SKSE::WinAPI::VK_L;
        case KeyBoard::Semicolon:
            return SKSE::WinAPI::VK_OEM_1;
        case KeyBoard::Apostrophe:
            return SKSE::WinAPI::VK_OEM_7;
        case KeyBoard::Console:
            return SKSE::WinAPI::VK_OEM_3;
        case KeyBoard::LeftShift:
            return SKSE::WinAPI::VK_LSHIFT;
        case KeyBoard::BackSlash:
            return SKSE::WinAPI::VK_OEM_102;
        case KeyBoard::Z:
            return SKSE::WinAPI::VK_Z;
        case KeyBoard::X:
            return SKSE::WinAPI::VK_X;
        case KeyBoard::C:
            return SKSE::WinAPI::VK_C;
        case KeyBoard::V:
            return SKSE::WinAPI::VK_V;
        case KeyBoard::B:
            return SKSE::WinAPI::VK_B;
        case KeyBoard::N:
            return SKSE::WinAPI::VK_N;
        case KeyBoard::M:
            return SKSE::WinAPI::VK_M;
        case KeyBoard::Comma:
            return SKSE::WinAPI::VK_OEM_COMMA;
        case KeyBoard::Period:
            return SKSE::WinAPI::VK_OEM_PERIOD;
        case KeyBoard::Slash:
            return SKSE::WinAPI::VK_OEM_2;
        case KeyBoard::RightShift:
            return SKSE::WinAPI::VK_RSHIFT;
        case KeyBoard::NUM_star:
            return SKSE::WinAPI::VK_MULTIPLY;
        case KeyBoard::LeftAlt:
            return SKSE::WinAPI::VK_LMENU;
        case KeyBoard::Space:
            return SKSE::WinAPI::VK_SPACE;
        case KeyBoard::CapsLock:
            return SKSE::WinAPI::VK_CAPITAL;
        case KeyBoard::F1:
            return SKSE::WinAPI::VK_F1;
        case KeyBoard::F2:
            return SKSE::WinAPI::VK_F2;
        case KeyBoard::F3:
            return SKSE::WinAPI::VK_F3;
        case KeyBoard::F4:
            return SKSE::WinAPI::VK_F4;
        case KeyBoard::F5:
            return SKSE::WinAPI::VK_F5;
        case KeyBoard::F6:
            return SKSE::WinAPI::VK_F6;
        case KeyBoard::F7:
            return SKSE::WinAPI::VK_F7;
        case KeyBoard::F8:
            return SKSE::WinAPI::VK_F8;
        case KeyBoard::F9:
            return SKSE::WinAPI::VK_F9;
        case KeyBoard::F10:
            return SKSE::WinAPI::VK_F10;
        case KeyBoard::NumLock:
            return SKSE::WinAPI::VK_NUMLOCK;
        case KeyBoard::SrcollLock:
            return SKSE::WinAPI::VK_SCROLL;
        case KeyBoard::NUM7:
            return SKSE::WinAPI::VK_NUMPAD7;
        case KeyBoard::NUM8:
            return SKSE::WinAPI::VK_NUMPAD8;
        case KeyBoard::NUM9:
            return SKSE::WinAPI::VK_NUMPAD9;
        case KeyBoard::NUM_Minus:
            return SKSE::WinAPI::VK_SUBTRACT;
        case KeyBoard::NUM4:
            return SKSE::WinAPI::VK_NUMPAD4;
        case KeyBoard::NUM5:
            return SKSE::WinAPI::VK_NUMPAD5;
        case KeyBoard::NUM6:
            return SKSE::WinAPI::VK_NUMPAD6;
        case KeyBoard::NUM_Plus:
            return SKSE::WinAPI::VK_ADD;
        case KeyBoard::NUM1:
            return SKSE::WinAPI::VK_NUMPAD1;
        case KeyBoard::NUM2:
            return SKSE::WinAPI::VK_NUMPAD2;
        case KeyBoard::NUM3:
            return SKSE::WinAPI::VK_NUMPAD3;
        case KeyBoard::NUM0:
            return SKSE::WinAPI::VK_NUMPAD0;
        case KeyBoard::NUM_Dot:
            return SKSE::WinAPI::VK_DECIMAL;
        case KeyBoard::F11:
            return SKSE::WinAPI::VK_F11;
        case KeyBoard::F12:
            return SKSE::WinAPI::VK_F12;
        }
        return 0;
    }

    bool GetKeyState(uint32_t code)
    {
        auto vKey = GetWinKeyMap(code);
        if (vKey)
            return SKSE::WinAPI::GetKeyState(vKey) & 0x8000;
        return false;
    }

    void Tracker(uint32_t code, std::function<void()> func)
    {
        while (GetKeyState(code))
        {
        }
        func();
    }

    void TrackKeyState(uint32_t code, std::function<void()> func)
    {
        std::thread(Tracker, code, func).detach();
    }

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