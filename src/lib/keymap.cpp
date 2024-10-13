#include "keymap.h"

namespace KeyUtils
{
    void GetVanillaKeyMap(uint32_t *keyArr, RE::BSFixedString keyInput)
    {
        keyArr[INPUT_DEVICE::KeyBoard] = Var::im->GetMappedKey(keyInput, RE::INPUT_DEVICE::kKeyboard);
        keyArr[INPUT_DEVICE::Gamepad] = GamepadButtonOffset + Var::im->GetMappedKey(keyInput, RE::INPUT_DEVICE::kGamepad);
        keyArr[INPUT_DEVICE::Mouse] = MouseButtonOffset + Var::im->GetMappedKey(keyInput, RE::INPUT_DEVICE::kMouse);
    }
}