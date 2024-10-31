#include "utils.h"

namespace VarUtils
{
const SKSE::SerializationInterface *cosave;
RE::PlayerCharacter *player = nullptr;
PlayerControls *pCtrl = nullptr;
ControlMap *ctrlMap = nullptr;
UserEvents *userEvent = nullptr;
UI *ui = nullptr;

void init()
{
    logger::trace("Initializing cosave serialization...");
    cosave = SKSE::GetSerializationInterface();
    cosave->SetUniqueID(_byteswap_ulong('AKLE'));
    logger::trace("Cosave serialization initialized.");

    player = RE::PlayerCharacter::GetSingleton();
    pCtrl = PlayerControls::GetSingleton();
    ctrlMap = ControlMap::GetSingleton();
    userEvent = UserEvents::GetSingleton();
    ui = UI::GetSingleton();
}
} // namespace VarUtils

namespace KeyUtils
{
uint32_t GetWinKeyMap(uint32_t code)
{
    switch (code)
    {
    // KeyBoard
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
    case KeyBoard::NUM_Enter:
        return SKSE::WinAPI::VK_RETURN;
    case KeyBoard::RightCtrl:
        return SKSE::WinAPI::VK_RCONTROL;
    case KeyBoard::NUM_Slash:
        return SKSE::WinAPI::VK_DIVIDE;
    case KeyBoard::PrtSc:
        return SKSE::WinAPI::VK_SNAPSHOT;
    case KeyBoard::RightAlt:
        return SKSE::WinAPI::VK_RMENU;
    case KeyBoard::Pause:
        return SKSE::WinAPI::VK_PAUSE;
    case KeyBoard::Home:
        return SKSE::WinAPI::VK_HOME;
    case KeyBoard::ArrowUp:
        return SKSE::WinAPI::VK_UP;
    case KeyBoard::PageUp:
        return SKSE::WinAPI::VK_PRIOR;
    case KeyBoard::ArrowLeft:
        return SKSE::WinAPI::VK_LEFT;
    case KeyBoard::ArrowRight:
        return SKSE::WinAPI::VK_RIGHT;
    case KeyBoard::End:
        return SKSE::WinAPI::VK_END;
    case KeyBoard::ArrowDown:
        return SKSE::WinAPI::VK_DOWN;
    case KeyBoard::PageDown:
        return SKSE::WinAPI::VK_NEXT;
    case KeyBoard::Insert:
        return SKSE::WinAPI::VK_INSERT;
    case KeyBoard::Delete:
        return SKSE::WinAPI::VK_DELETE;

    // Mouse
    case Mouse::MouseButtonLeft:
        return SKSE::WinAPI::VK_LBUTTON;
    case Mouse::MouseButtonRight:
        return SKSE::WinAPI::VK_RBUTTON;
    case Mouse::MouseButtonMiddle:
        return SKSE::WinAPI::VK_MBUTTON;
    case Mouse::MouseButton3:
        return SKSE::WinAPI::VK_XBUTTON1;
    case Mouse::MouseButton4:
        return SKSE::WinAPI::VK_XBUTTON2;
    case Mouse::MouseButton5:
        return 0;
    case Mouse::MouseButton6:
        return 0;
    case Mouse::MouseButton7:
        return 0;
    case Mouse::MouseWheelUp:
        return 0;
    case Mouse::MouseWheelDown:
        return 0;

        // GamePad
        /*
        case GamePad::GamepadButtonDpadUp:
            return SKSE::WinAPI::VK_DELETE;
        case GamePad::GamepadButtonDpadDown:
            return SKSE::WinAPI::VK_DELETE;
        case GamePad::GamepadButtonDpadLfet:
            return SKSE::WinAPI::VK_DELETE;
        case GamePad::GamepadButtonDpadRight:
            return SKSE::WinAPI::VK_DELETE;
        */
    }
    return 0;
}

bool GetKeyState(uint32_t code)
{
    if (!code)
        return false;
    auto vKey = GetWinKeyMap(code);
    if (vKey)
        return SKSE::WinAPI::GetKeyState(vKey) & 0x8000;
    return false;
}

void Tracker(uint32_t code, std::function<void()> func)
{
    while (GetKeyState(code))
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
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
} // namespace KeyUtils

namespace TimeUtils
{
uint64_t GetTime()
{
    auto now = std::chrono::steady_clock::now();
    uint64_t time = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count();
    return time;
}
} // namespace TimeUtils

namespace FormUtils
{
TESForm *GetForm(FormID FormId)
{
    return TESForm::LookupByID(FormId);
}
TESForm *GetForm(std::string modForm)
{
    std::string modName;
    FormID formId;

    auto delimiterPos = modForm.find('|');
    if (delimiterPos == std::string::npos)
    {
        logger::error("Error: Unable to resolve FormID: {}\t\tReason: Can't find sysbol '|' ", modForm.c_str());
        return nullptr;
    }
    modName = modForm.substr(0, delimiterPos);
    std::string formid_str = modForm.substr(delimiterPos + 1);
    try
    {
        std::stringstream hexStream;
        hexStream << std::hex << formid_str;
        hexStream >> formId;
    }
    catch (std::exception e)
    {
        logger::error("Error:{}\t\tUnable to resolve FormID: {}\t\tReason: Can't transform {} to hex\t\t", e.what(),
                      modForm.c_str(), formid_str.c_str());
        return nullptr;
    }
    TESDataHandler *dataHandler = TESDataHandler::GetSingleton();
    TESFile *modFile = nullptr;
    for (auto it = dataHandler->files.begin(); it != dataHandler->files.end(); it++)
    {
        TESFile *f = *it;
        if (strcmp(f->fileName, modName.c_str()) == 0)
        {
            modFile = f;
            break;
        }
    }
    if (!modFile)
    {
        logger::error("Error: mod {} not exist", modName.c_str());
        return nullptr;
    }
    uint8_t modIndex = modFile->compileIndex;
    if (modIndex < 0xFE)
    {
        formId |= ((uint32_t)modIndex) << 24;
    }
    else
    {
        uint16_t lightModIndex = modFile->smallFileCompileIndex;
        if (lightModIndex != 0xFFFF)
        {
            formId |= 0xFE000000 | (uint32_t(lightModIndex) << 12);
        }
    }
    logger::trace("resolve formID:{} success, result:{}", modForm.c_str(), formId);
    return TESForm::LookupByID(formId);
}
} // namespace FormUtils