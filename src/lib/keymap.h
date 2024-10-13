#pragma once
#include "PCH.h"
#include "var.h"

using namespace RE;
using namespace REL;
using namespace SKSE::log;

namespace KeyUtils
{
    enum INPUT_DEVICE {
        KeyBoard = 0,
        Gamepad,
        Mouse,
        Total
    };

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
        GamepadButtonDpadUp = 266,
        GamepadButtonDpadDown,
        GamepadButtonDpadLfet,
        GamepadButtonDpadRight,
        GamepadButtonStart,
        GamepadButtonBack,
        GamepadButtonLeftThumb,
        GamepadButtonRightThumb,
        GamepadButtonLeftShoulder,
        GamepadButtonRightShoulder,
        GamepadButtonA,
        GamepadButtonB,
        GamepadButtonX,
        GamepadButtonY,
        GamepadButtonLT,
        GamepadButtonRT // 281
    };

    void GetVanillaKeyMap(uint32_t *keyArr, RE::BSFixedString keyInput);
}