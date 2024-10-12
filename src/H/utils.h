#pragma once

using namespace RE;
using namespace REL;
using namespace SKSE::log;

namespace Var
{
    static RE::PlayerCharacter *p;
    static PlayerControls *pc;
    static ControlMap *im;
    static UserEvents *inputString;
}

namespace KeyUtils
{
    // Enums from SKSE to get DXScanCodes
    enum
    {
        ESC = 1,
        MAIN1,
        MAIN2,
        MAIN3,
        MAIN4,
        MAIN5,
        MAIN6,
        MAIN7,
        MAIN8,
        MAIN9,
        MAIN0,
        Minus,  // -
        Equals, // =
        Backspace,
        Tab,
        Q,
        W,
        E,
        R,
        T,
        Y,
        U,
        I,
        O,
        P,
        LeftBracket,  // [
        RightBracket, // ]
        Enter,
        LeftCtrl,
        A,
        S,
        D,
        F,
        G,
        H,
        J,
        K,
        L,
        Semicolon,  // ;
        Apostrophe, // '
        Console,    // ~
        LeftShift,
        BackSlash, /* \ */
        Z,
        X,
        C,
        V,
        B,
        N,
        M,
        Comma,  //,
        Period, //.
        Slash,  // /
        RightShift,
        NUM_star, // NUM*
        LeftAlt,
        Space,
        CapsLock,
        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        NumLock,
        SrcollLock,
        NUM7,
        NUM8,
        NUM9,
        NUM_Minus, // NUM-
        NUM4,
        NUM5,
        NUM6,
        NUM_Plus, // NUM+
        NUM1,
        NUM2,
        NUM3,
        NUM0,
        NUM_Dot, // NUM.

        F11 = 87,
        F12
    };

    enum
    {
        // first 256 for keyboard, then 8 mouse buttons, then mouse wheel up, wheel down, then 16 gamepad buttons
        MouseButtonOffset = 256,
        MouseButtonLeft = 256,
        MouseButtonRight,
        MouseButtonMiddle,
        MouseButton3,
        MouseButton4,
        MouseButton5,
        MouseButton6,
        MouseButton7,
        MouseWheelUp,
        MouseWheelDown, // 265
    };

    enum
    {
        GamepadButtonOffset = 266,
        GamepadButtonOffset_DPAD_UP = 266,
        GamepadButtonOffset_DPAD_DOWN,
        GamepadButtonOffset_DPAD_LEFT,
        GamepadButtonOffset_DPAD_RIGHT,
        GamepadButtonOffset_START,
        GamepadButtonOffset_BACK,
        GamepadButtonOffset_LEFT_THUMB,
        GamepadButtonOffset_RIGHT_THUMB,
        GamepadButtonOffset_LEFT_SHOULDER,
        GamepadButtonOffset_RIGHT_SHOULDER,
        GamepadButtonOffset_A,
        GamepadButtonOffset_B,
        GamepadButtonOffset_X,
        GamepadButtonOffset_Y,
        GamepadButtonOffset_LT,
        GamepadButtonOffset_RT // 281
    };
#ifndef __KEYUTILS_FUNCTION__
#define __KEYUTILS_FUNCTION__
    void GetVanillaKeyMap(uint32_t *keyArr, RE::BSFixedString keyInput)
    {
        keyArr[INPUT_DEVICE::kKeyboard] = Var::im->GetMappedKey(keyInput, INPUT_DEVICE::kKeyboard);
        keyArr[INPUT_DEVICE::kMouse] = MouseButtonOffset + Var::im->GetMappedKey(keyInput, INPUT_DEVICE::kMouse);
        keyArr[INPUT_DEVICE::kGamepad] = GamepadButtonOffset + Var::im->GetMappedKey(keyInput, INPUT_DEVICE::kGamepad);
    }
#endif
}