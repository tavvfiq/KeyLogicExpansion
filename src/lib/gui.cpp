#include "gui.h"

namespace GUI
{
std::deque<uint32_t> guiInputQueue;
uint8_t guiNums;

static ID3D11Device *device = nullptr;
static ID3D11DeviceContext *context = nullptr;
static IDXGISwapChain *swapChain = nullptr;
static ID3D11RenderTargetView *targetView = NULL;

static std::unordered_map<int32_t, std::string> KeyNameMap;

static bool show = false;
static bool showKeyBind;
static bool showStances;

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
    };
}

void KeyBindSettings()
{
    ImGui::SetNextWindowPos(ImVec2(200, 200));
    ImGui::SetNextWindowSize(ImVec2(400, 800));
    ImGui::Begin("KeyBind Settings", &showKeyBind, ImGuiWindowFlags_NoCollapse);

    if (ImGui::Button("Save to InI"))
    {
        Config::saveInI();
    }

    ImGui::Checkbox("EnableCustomInput", &Custom::enableCustomInput);
    ImGui::Checkbox("EnableStances", &Stances::enableStances);
    ImGui::Checkbox("EnableHoldSprint", &Config::enableHoldSprint);
    ImGui::Checkbox("EnableHoldSneak", &Config::enableHoldSneak);
    ImGui::Checkbox("EnableReverseHorseAttack", &Config::enableReverseHorseAttack);

    if (ImGui::BeginCombo("EnableSheatheAttack", KeyNameMap[Config::enableSheatheAttack].data()))
    {
        for (auto item : KeyNameMap)
        {
            bool is_selected = (Config::enableSheatheAttack == item.first);
            if (ImGui::Selectable(item.second.data(), is_selected))
                Config::enableSheatheAttack = item.first;

            // 设置高亮
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    ImGui::End();
}

void ShowStances()
{
    //  window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
    //  ImGui::SetNextWindowClass(&window_class);
    //   ImGui::Begin("Keyboard State");
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
        if (u_msg == WM_KILLFOCUS)
            show = false;

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

        device = reinterpret_cast<ID3D11Device *>(renderManager->data.forwarder);
        context = reinterpret_cast<ID3D11DeviceContext *>(renderManager->data.context);
        swapChain = reinterpret_cast<IDXGISwapChain *>(renderManager->data.renderWindows[0].swapChain);

        DXGI_SWAP_CHAIN_DESC sd{};
        swapChain->GetDesc(&sd);
        ImGui::CreateContext();
        auto &io = ImGui::GetIO();

        io.DisplaySize = {static_cast<float>(sd.BufferDesc.Width), static_cast<float>(sd.BufferDesc.Height)};
        io.ConfigWindowsMoveFromTitleBarOnly = true;
        io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

        ImGui_ImplWin32_Init(sd.OutputWindow);
        ImGui_ImplDX11_Init(device, context);
        ImGui::StyleColorsDark();
    }
    static void Present(uint32_t a1)
    {
        FnPr(a1);

        if (show)
        {
            ImGui_ImplWin32_NewFrame();
            ImGui_ImplDX11_NewFrame();
            ImGui::NewFrame();
            if (showKeyBind)
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
                KeyBindSettings();
                ImGui::ShowDemoWindow();
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

void init()
{
    Win32Hook::Hook();
    DX11Hook::Hook();
}
void showGUI()
{
    show = true;
    showKeyBind = true;
    showStances = Stances::enableStances;
}
} // namespace GUI