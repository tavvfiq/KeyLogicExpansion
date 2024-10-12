#include "H/hook.h"

using namespace RE;
using namespace REL;

namespace AltTweenMenu
{
    bool HookMenuOpenHandler::CanProcess(InputEvent *a_event)
    {
        FnCanProcess fn = fnCP.at(*(uintptr_t *)this);
        if (a_event->eventType == INPUT_EVENT_TYPE::kButton)
            if (a_event->AsButtonEvent()->idCode == 34)
                return true;
        return (this->*fn)(a_event);
    }
    inline std::unordered_map<uintptr_t, HookMenuOpenHandler::FnCanProcess> HookMenuOpenHandler::fnCP;
    bool HookMenuOpenHandler::ProcessButton(ButtonEvent *a_event)
    {
        FnProcessButton fn = fnPB.at(*(uintptr_t *)this);
        if (a_event->idCode == 15)
            return true;
        else if (a_event->idCode == 34)
        {
            a_event->userEvent = BSFixedString("Tween Menu");
            return (this->*fn)(a_event);
        }
        else
            return (this->*fn)(a_event);
    }
    inline std::unordered_map<uintptr_t, HookMenuOpenHandler::FnProcessButton> HookMenuOpenHandler::fnPB;
    void HookMenuOpenHandler::Hook()
    {
        REL::Relocation<uintptr_t> vtable{VTABLE_MenuOpenHandler[0]};
        fnCP.insert(std::pair<uintptr_t, FnCanProcess>(
            vtable.address(), stl::unrestricted_cast<FnCanProcess>(vtable.write_vfunc(1, &HookMenuOpenHandler::CanProcess))));
        fnPB.insert(std::pair<uintptr_t, FnProcessButton>(
            vtable.address(), stl::unrestricted_cast<FnProcessButton>(vtable.write_vfunc(5, &HookMenuOpenHandler::ProcessButton))));
    }
}
