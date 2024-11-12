#include "gui.h"

namespace GUI
{
std::deque<uint32_t> guiInputQueue;
uint8_t guiNums;
typedef struct
{
    uint32_t code;
    const char *name;
} NameMap;
static std::vector<NameMap> KeyNameMap;

static bool showGui = false;
bool showSettings;
static bool showCustom;

static uint64_t tmpMouse;

void buildKeyNameMap()
{
    KeyNameMap = {
        {0, "Disable"},
        {KeyUtils::KeyBoard::ESC, "ESC"},
        {KeyUtils::KeyBoard::Enter, "Enter"},
        {KeyUtils::KeyBoard::Space, "Space"},
        {KeyUtils::KeyBoard::LeftCtrl, "LeftCtrl"},
        {KeyUtils::KeyBoard::RightCtrl, "RightCtrl"},
        {KeyUtils::KeyBoard::LeftShift, "LeftShift"},
        {KeyUtils::KeyBoard::RightShift, "RightShift"},
        {KeyUtils::KeyBoard::LeftAlt, "LeftAlt"},
        {KeyUtils::KeyBoard::RightAlt, "RightAlt"},
        {KeyUtils::KeyBoard::Backspace, "Backspace"},
        {KeyUtils::KeyBoard::Home, "Home"},
        {KeyUtils::KeyBoard::End, "End"},
        {KeyUtils::KeyBoard::Insert, "Insert"},
        {KeyUtils::KeyBoard::PageUp, "PageUp"},
        {KeyUtils::KeyBoard::PageDown, "PageDown"},
        {KeyUtils::KeyBoard::Delete, "Delete"},
        {KeyUtils::KeyBoard::PrtSc, "PrtSc"},
        {KeyUtils::KeyBoard::Pause, "Pause"},
        {KeyUtils::KeyBoard::CapsLock, "CapsLock"},
        {KeyUtils::KeyBoard::NumLock, "NumLock"},
        {KeyUtils::KeyBoard::SrcollLock, "SrcollLock"},
        {KeyUtils::KeyBoard::Tab, "Tab"},
        {KeyUtils::KeyBoard::MAIN1, "1"},
        {KeyUtils::KeyBoard::MAIN2, "2"},
        {KeyUtils::KeyBoard::MAIN3, "3"},
        {KeyUtils::KeyBoard::MAIN4, "4"},
        {KeyUtils::KeyBoard::MAIN5, "5"},
        {KeyUtils::KeyBoard::MAIN6, "6"},
        {KeyUtils::KeyBoard::MAIN7, "7"},
        {KeyUtils::KeyBoard::MAIN8, "8"},
        {KeyUtils::KeyBoard::MAIN9, "9"},
        {KeyUtils::KeyBoard::MAIN0, "0"},
        {KeyUtils::KeyBoard::Minus, "-"},
        {KeyUtils::KeyBoard::Equals, "="},
        {KeyUtils::KeyBoard::Q, "Q"},
        {KeyUtils::KeyBoard::W, "W"},
        {KeyUtils::KeyBoard::E, "E"},
        {KeyUtils::KeyBoard::R, "R"},
        {KeyUtils::KeyBoard::T, "T"},
        {KeyUtils::KeyBoard::Y, "Y"},
        {KeyUtils::KeyBoard::U, "U"},
        {KeyUtils::KeyBoard::I, "I"},
        {KeyUtils::KeyBoard::O, "O"},
        {KeyUtils::KeyBoard::P, "P"},
        {KeyUtils::KeyBoard::A, "A"},
        {KeyUtils::KeyBoard::S, "S"},
        {KeyUtils::KeyBoard::D, "D"},
        {KeyUtils::KeyBoard::F, "F"},
        {KeyUtils::KeyBoard::G, "G"},
        {KeyUtils::KeyBoard::H, "H"},
        {KeyUtils::KeyBoard::J, "K"},
        {KeyUtils::KeyBoard::L, "L"},
        {KeyUtils::KeyBoard::Z, "Z"},
        {KeyUtils::KeyBoard::X, "X"},
        {KeyUtils::KeyBoard::C, "C"},
        {KeyUtils::KeyBoard::V, "V"},
        {KeyUtils::KeyBoard::B, "B"},
        {KeyUtils::KeyBoard::N, "N"},
        {KeyUtils::KeyBoard::M, "M"},
        {KeyUtils::KeyBoard::ArrowUp, "↑"},
        {KeyUtils::KeyBoard::ArrowDown, "↓"},
        {KeyUtils::KeyBoard::ArrowLeft, "←"},
        {KeyUtils::KeyBoard::ArrowRight, "→"},
        {KeyUtils::KeyBoard::LeftBracket, "["},
        {KeyUtils::KeyBoard::RightBracket, "]"},
        {KeyUtils::KeyBoard::Console, "~"},
        {KeyUtils::KeyBoard::Semicolon, ";"},
        {KeyUtils::KeyBoard::Apostrophe, "'"},
        {KeyUtils::KeyBoard::Comma, ","},
        {KeyUtils::KeyBoard::Period, "."},
        {KeyUtils::KeyBoard::Slash, "/"},
        {KeyUtils::KeyBoard::BackSlash, "\\"},
        {KeyUtils::KeyBoard::F1, "F1"},
        {KeyUtils::KeyBoard::F2, "F2"},
        {KeyUtils::KeyBoard::F3, "F3"},
        {KeyUtils::KeyBoard::F4, "F4"},
        {KeyUtils::KeyBoard::F5, "F5"},
        {KeyUtils::KeyBoard::F6, "F6"},
        {KeyUtils::KeyBoard::F7, "F7"},
        {KeyUtils::KeyBoard::F8, "F8"},
        {KeyUtils::KeyBoard::F9, "F9"},
        {KeyUtils::KeyBoard::F10, "F10"},
        {KeyUtils::KeyBoard::F11, "F11"},
        {KeyUtils::KeyBoard::F12, "F12"},
        {KeyUtils::KeyBoard::NUM0, "NUM0"},
        {KeyUtils::KeyBoard::NUM1, "NUM1"},
        {KeyUtils::KeyBoard::NUM2, "NUM2"},
        {KeyUtils::KeyBoard::NUM3, "NUM3"},
        {KeyUtils::KeyBoard::NUM4, "NUM4"},
        {KeyUtils::KeyBoard::NUM5, "NUM5"},
        {KeyUtils::KeyBoard::NUM6, "NUM6"},
        {KeyUtils::KeyBoard::NUM7, "NUM7"},
        {KeyUtils::KeyBoard::NUM8, "NUM8"},
        {KeyUtils::KeyBoard::NUM9, "NUM9"},
        {KeyUtils::KeyBoard::NUM_Minus, "NUM-"},
        {KeyUtils::KeyBoard::NUM_Plus, "NUM+"},
        {KeyUtils::KeyBoard::NUM_star, "NUM*"},
        {KeyUtils::KeyBoard::NUM_Slash, "NUM/"},
        {KeyUtils::KeyBoard::NUM_Dot, "NUM."},
        {KeyUtils::KeyBoard::NUM_Enter, "NUM_Enter"},
        {KeyUtils::Mouse::MouseButtonLeft, "MouseButtonLeft"},
        {KeyUtils::Mouse::MouseButtonRight, "MouseButtonRight"},
        {KeyUtils::Mouse::MouseButtonMiddle, "MouseButtonMiddle"},
        {KeyUtils::Mouse::MouseButton3, "M4"},
        {KeyUtils::Mouse::MouseButton4, "M5"},
        {KeyUtils::Mouse::MouseWheelUp, "MouseWheelUp"},
        {KeyUtils::Mouse::MouseWheelDown, "MouseWheelDown"},
        {1000000, "None"},
    };
}

const char *GetNameByKey(uint32_t code)
{
    auto res = std::find_if(KeyNameMap.begin(), KeyNameMap.end(), [code](NameMap map) { return code == map.code; });
    if (res == KeyNameMap.end())
        return "None";
    else
        return res->name;
}

void SwitchButton(const char *name, bool &value, const char *description = "")
{
    ImGui::Checkbox(name, &value);
    if (ImGui::IsItemHovered())
        ImGui::SetTooltip(description);
}

void SelectButton(const char *name, uint32_t &code, const char *description = "")
{
    if (ImGui::BeginCombo(name, GetNameByKey(code)))
    {
        for (auto item : KeyNameMap)
        {
            bool is_selected = (Config::enableSheatheAttack == item.code);
            if (ImGui::Selectable(item.name, is_selected))
                Config::enableSheatheAttack = item.code;
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    if (ImGui::IsItemHovered())
        ImGui::SetTooltip(description);
}

void KeyBindSettings()
{
    ImGui::Begin("KeyBind Settings", &showSettings, ImGuiWindowFlags_NoCollapse);

    if (ImGui::Button("Save to InI"))
        Config::saveInI();

    SwitchButton("ShowCustomSettings", showCustom);
    if (ImGui::TreeNode("Features"))
    {
        SwitchButton("EnableCustomInput", Custom::enableCustomInput,
                     "Enable custom Input, maybe this is the reason you install this mod.");
        SwitchButton("EnableStances", Stances::enableStances,
                     "Enable Stances Supported by KLE\nContain 3 types "
                     "of stance: High, Mid, Low.");
        SwitchButton("EnableHoldSprint", Config::enableHoldSprint, "Change enable sprint when you hold sprint key");
        SwitchButton("EnableHoldSneak", Config::enableHoldSneak,
                     "Same as EnableHoldSprint, Change enable sneak when you hold sneak key");
        SwitchButton("EnableReverseHorseAttack", Config::enableReverseHorseAttack,
                     "Reverse your HorseAttack diretion, if enable this, left key attack left, right key attack right");
        SelectButton(
            "EnableSheatheAttack", Config::enableSheatheAttack,
            "Make you can attack when you press this key, NOT completed.\n0 means disable, other number means a "
            "keycode\n"
            "when you press the key and Attack or PowerAttack or even Sheathe Key, you will do a SheatheAttack\nNote: "
            "Press with Sheathe Key can do SheatheAttack when you are NOT in Sheathe status.");
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Stances"))
    {
        SelectButton("StancesModifier", Stances::StancesModfier, "Set change stance key, modifier = 0 means disable");
        SelectButton("ChangeToLow", Stances::ChangeToLow);
        SelectButton("ChangeToMid", Stances::ChangeToMid);
        SelectButton("ChangeToHigh", Stances::ChangeToHigh);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Vanilla"))
    {
        SelectButton("NormalAttack", Config::normalAttack, "NormalAttack Key used in KLE system");
        SelectButton("PowerAttack", Config::powerAttack, "PowerAttack Key used in KLE system");
        SelectButton("Block", Config::block, "separete block key from Attack");
        SelectButton("MagicModifier", Config::MagicModifier);
        SelectButton("BFCO SpecialAttackModifier", Config::BFCO_SpecialAttackModifier);
        SelectButton("BFCO ComboAttack", Config::BFCO_ComboAttack);
        SelectButton("AltTweenMenu", Config::altTweenMenu, "instead Vanilla Key");
        SelectButton("AltTogglePOV", Config::altTogglePOV, "instead Vanilla Key");
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Expand"))
    {
        SelectButton("WarAshModifier", Config::warAshModifier);
        SelectButton("WarAsh", Config::warAsh, "EldenRim WarAsh support, press this to use WarAsh.");
        SelectButton("ZoomModifier", Config::zoomModifier,
                     "set it to Non 0 can instead default ZoomIn and ZoomOut \nyou don't konw what it mean? just "
                     "Vanilla MouseWheelUp and MouseWheelDwon");
        SelectButton("AltZoomIn", Config::altZoomIn);
        SelectButton("AltZoomOut", Config::altZoomOut);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Modifier"))
    {
        SelectButton(VarUtils::userEvent->forward.c_str(),
                     Config::needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->forward)]);
        SelectButton(VarUtils::userEvent->strafeLeft.c_str(),
                     Config::needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->strafeLeft)]);
        SelectButton(VarUtils::userEvent->back.c_str(),
                     Config::needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->back)]);
        SelectButton(VarUtils::userEvent->strafeRight.c_str(),
                     Config::needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->strafeRight)]);
        SelectButton(VarUtils::userEvent->rightAttack.c_str(),
                     Config::needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->rightAttack)]);
        SelectButton(VarUtils::userEvent->leftAttack.c_str(),
                     Config::needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->leftAttack)]);
        SelectButton(VarUtils::userEvent->activate.c_str(),
                     Config::needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->activate)]);
        SelectButton(VarUtils::userEvent->readyWeapon.c_str(),
                     Config::needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->readyWeapon)]);
        SelectButton(VarUtils::userEvent->tweenMenu.c_str(),
                     Config::needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->tweenMenu)]);
        SelectButton(VarUtils::userEvent->togglePOV.c_str(),
                     Config::needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->togglePOV)]);
        SelectButton(VarUtils::userEvent->jump.c_str(),
                     Config::needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->jump)]);
        SelectButton(VarUtils::userEvent->sprint.c_str(),
                     Config::needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->sprint)]);
        SelectButton(VarUtils::userEvent->shout.c_str(),
                     Config::needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->shout)]);
        SelectButton(VarUtils::userEvent->sneak.c_str(),
                     Config::needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->sneak)]);
        SelectButton(VarUtils::userEvent->run.c_str(),
                     Config::needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->run)]);
        SelectButton(VarUtils::userEvent->toggleRun.c_str(),
                     Config::needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->toggleRun)]);
        SelectButton(VarUtils::userEvent->autoMove.c_str(),
                     Config::needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->autoMove)]);
        SelectButton(VarUtils::userEvent->favorites.c_str(),
                     Config::needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->favorites)]);
        SelectButton(VarUtils::userEvent->quicksave.c_str(),
                     Config::needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->quicksave)]);
        SelectButton(VarUtils::userEvent->quickload.c_str(),
                     Config::needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->quickload)]);
        SelectButton(VarUtils::userEvent->wait.c_str(),
                     Config::needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->wait)]);
        SelectButton(VarUtils::userEvent->journal.c_str(),
                     Config::needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->journal)]);
        SelectButton(VarUtils::userEvent->pause.c_str(),
                     Config::needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->pause)]);
        SelectButton(VarUtils::userEvent->quickInventory.c_str(),
                     Config::needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->quickInventory)]);
        SelectButton(VarUtils::userEvent->quickMagic.c_str(),
                     Config::needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->quickMagic)]);
        SelectButton(VarUtils::userEvent->quickStats.c_str(),
                     Config::needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->quickStats)]);
        SelectButton(VarUtils::userEvent->quickMap.c_str(),
                     Config::needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->quickMap)]);
        ImGui::TreePop();
    }
    ImGui::End();
}

class Win32Hook
{
  public:
    static ATOM RegisterClassA(WNDCLASSA *a_wndClass)
    {
        FnWP = reinterpret_cast<uintptr_t>(a_wndClass->lpfnWndProc);
        a_wndClass->lpfnWndProc = &WND_PROC;
        return FnRC(a_wndClass);
    }
    static LRESULT WND_PROC(const HWND h_wnd, const UINT u_msg, const WPARAM w_param, const LPARAM l_param)
    {
        switch (u_msg)
        {
        case WM_KILLFOCUS:
            showGui = false;
            break;
        case WM_SETFOCUS:
            showGui = true;
            break;
        }
        return FnWP(h_wnd, u_msg, w_param, l_param);
    }
    static void Hook()
    {
        auto &trampoline = SKSE::GetTrampoline();
        SKSE::AllocTrampoline(8);
        const REL::Relocation<uintptr_t> registerWindowHook{REL::VariantID(75591, 77226, 0xDC4B90)};
        FnRC = *(uintptr_t *)trampoline.write_call<6>(
            registerWindowHook.address() + REL::VariantOffset(0x8E, 0x15C, 0x99).offset(), RegisterClassA);
    }

  private:
    static inline REL::Relocation<decltype(RegisterClassA)> FnRC;
    static inline REL::Relocation<decltype(WND_PROC)> FnWP;
};

class DX11Hook
{
  public:
    static void CreateD3D11()
    {
        FnCD11();

        const auto renderManager = RE::BSGraphics::Renderer::GetSingleton();

        auto device = reinterpret_cast<ID3D11Device *>(renderManager->data.forwarder);
        auto context = reinterpret_cast<ID3D11DeviceContext *>(renderManager->data.context);
        auto swapChain = reinterpret_cast<IDXGISwapChain *>(renderManager->data.renderWindows[0].swapChain);

        DXGI_SWAP_CHAIN_DESC sd{};
        swapChain->GetDesc(&sd);
        ImGui::CreateContext();

        ImGuiIO &io = ImGui::GetIO();
        io.DisplaySize = {static_cast<float>(sd.BufferDesc.Width), static_cast<float>(sd.BufferDesc.Height)};
        io.ConfigWindowsMoveFromTitleBarOnly = true;
        io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.BackendFlags = ImGuiBackendFlags_HasMouseCursors | ImGuiBackendFlags_RendererHasVtxOffset;

        ImGui_ImplWin32_Init(sd.OutputWindow);
        ImGui_ImplDX11_Init(device, context);
        ImGui::StyleColorsDark();
    }
    static void Present(uint32_t a1)
    {
        FnPr(a1);

        if (showGui)
        {
            ImGui_ImplWin32_NewFrame();
            ImGui_ImplDX11_NewFrame();
            ImGui::NewFrame();
            ImGuiIO &io = ImGui::GetIO();
            io.MouseDrawCursor = showSettings;
            // if (showStances)
            if (showSettings)
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
                KeyBindSettings();
            }
            ImGui::Render();
            ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        }
    }
    static void Hook()
    {
        auto &trampoline = SKSE::GetTrampoline();
        SKSE::AllocTrampoline(14);
        const REL::Relocation<uintptr_t> created3d11Hook{REL::VariantID(75595, 77226, 0xDC5530)};
        FnCD11 = trampoline.write_call<5>(created3d11Hook.address() + REL::VariantOffset(0x9, 0x275, 0x9).offset(),
                                          CreateD3D11);

        const REL::Relocation<uintptr_t> presentHook{REL::VariantID(75461, 77246, 0xDBBDD0)};
        SKSE::AllocTrampoline(14);
        FnPr = trampoline.write_call<5>(presentHook.address() + REL::VariantOffset(0x9, 0x9, 0x15).offset(), Present);
    }

  private:
    static inline REL::Relocation<decltype(CreateD3D11)> FnCD11;
    static inline REL::Relocation<decltype(Present)> FnPr;
};

class InputHook
{
  public:
    static void ProcessEvent(RE::BSTEventSource<RE::InputEvent *> *a_dispatcher, RE::InputEvent *const *a_events)
    {
        if (showGui && showSettings)
        {
            ImGuiIO &io = ImGui::GetIO();
            io.MouseDown[0] = GetKeyState(VK_LBUTTON) & 0x8000;
            if (KeyUtils::MouseWheelTime > tmpMouse)
            {
                tmpMouse = KeyUtils::MouseWheelTime;
                io.AddMouseWheelEvent(0, KeyUtils::MouseWheelStatus);
            }
        }
        FnPE(a_dispatcher, a_events);
    }
    static void Hook()
    {
        const REL::Relocation<uintptr_t> inputHook{REL::VariantID(67315, 68617, 0xC519E0)};
        auto &trampoline = SKSE::GetTrampoline();
        SKSE::AllocTrampoline(14);
        FnPE =
            trampoline.write_call<5>(inputHook.address() + REL::VariantOffset(0x7B, 0x7B, 0x81).offset(), ProcessEvent);
    }

  private:
    static inline REL::Relocation<decltype(ProcessEvent)> FnPE;
};

void init()
{
    buildKeyNameMap();
    Win32Hook::Hook();
    DX11Hook::Hook();
    InputHook::Hook();
}
void showGUI()
{
    showGui = true;
    showSettings = false;
    showCustom = false;
}
} // namespace GUI