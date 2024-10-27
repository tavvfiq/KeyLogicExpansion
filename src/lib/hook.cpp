#include "hook.h"

namespace Hook
{
    // MenuOpenHandler
    MenuOpenHandler *MenuOpenHandler::that;
    MenuOpenHandler::FnCanProcess MenuOpenHandler::FnCP;
    MenuOpenHandler::FnProcessButton MenuOpenHandler::FnPB;
    bool MenuOpenHandler::CanProcess(InputEvent *a_event)
    {
        if (a_event->GetEventType() == INPUT_EVENT_TYPE::kButton)
        {
            auto evn = a_event->AsButtonEvent();
            auto code = KeyUtils::GetEventKeyMap(evn);
            if (Config::altTweenMenu && evn->userEvent == VarUtils::userEvent->tweenMenu)
                return false;
            if (code == Config::altTweenMenu)
                return true;
        }
        return (this->*FnCP)(a_event);
    }
    bool MenuOpenHandler::CP(InputEvent *a_event) { return (that->*FnCP)(a_event); }
    bool MenuOpenHandler::ProcessButton(ButtonEvent *a_event)
    {
        auto code = KeyUtils::GetEventKeyMap(a_event);
        if (code == Config::altTweenMenu)
            a_event->userEvent = VarUtils::userEvent->tweenMenu;
        if (Config::needModifier[code]) {
            if (KeyUtils::GetKeyState(Config::needModifier[code]))
                return (this->*FnPB)(a_event);
            else 
                return false;
        }
        return (this->*FnPB)(a_event);
    }
    bool MenuOpenHandler::PB(ButtonEvent *a_event) { return (that->*FnPB)(a_event); }
    void MenuOpenHandler::Hook()
    {
        REL::Relocation<uintptr_t> vtable{VTABLE_MenuOpenHandler[0]};
        that = stl::unrestricted_cast<MenuOpenHandler *>(vtable.address());
        FnCP = stl::unrestricted_cast<FnCanProcess>(vtable.write_vfunc(1, &MenuOpenHandler::CanProcess));
        FnPB = stl::unrestricted_cast<FnProcessButton>(vtable.write_vfunc(5, &MenuOpenHandler::ProcessButton));
    }

    // AutoMoveHandler
    AutoMoveHandler *AutoMoveHandler::that;
    AutoMoveHandler::FnProcessButton AutoMoveHandler::FnPB;
    bool AutoMoveHandler::ProcessButton(ButtonEvent *a_event, PlayerControlsData *a_data)
    {
        auto code = KeyUtils::GetEventKeyMap(a_event);
        if (Config::needModifier[code]) {
            if (KeyUtils::GetKeyState(Config::needModifier[code]))
                return (this->*FnPB)(a_event, a_data);
            else 
                return false;
        }
        return (this->*FnPB)(a_event, a_data);
    }
    void AutoMoveHandler::Hook()
    {
        REL::Relocation<uintptr_t> vtable{VTABLE_AutoMoveHandler[0]};
        that = stl::unrestricted_cast<AutoMoveHandler *>(vtable.address());
        FnPB = stl::unrestricted_cast<FnProcessButton>(vtable.write_vfunc(4, &AutoMoveHandler::ProcessButton));
    }



    void Hook()
    {
        MenuOpenHandler::Hook();
        AutoMoveHandler::Hook();
    }
}
