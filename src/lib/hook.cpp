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
            auto code = KeyUtils::GetEventKeyMap(evn); // altTweenMenu
            if (Config::altTweenMenu && code == KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->tweenMenu))
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
        // altTweenMenu
        if (code == Config::altTweenMenu)
            a_event->userEvent = VarUtils::userEvent->tweenMenu;

        if (Config::needModifier[code])
        {
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

        if (Config::needModifier[code])
        {
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

    // FirstPersonState
    FirstPersonState *FirstPersonState::that;
    FirstPersonState::FnCanProcess FirstPersonState::FnCP;
    FirstPersonState::FnProcessButton FirstPersonState::FnPB;
    bool FirstPersonState::CanProcess(InputEvent *a_event)
    {
        if (a_event->GetEventType() == INPUT_EVENT_TYPE::kButton)
        {
            auto evn = a_event->AsButtonEvent();
            auto code = KeyUtils::GetEventKeyMap(evn);
            // altTogglePOV
            if (Config::altTogglePOV && code == KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->togglePOV))
                return false;
            if (code == Config::altTogglePOV)
                return true;
            // altZoomIn
            if (Config::altZoomIn && code == KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->zoomIn))
                return false;
            if (code == Config::altZoomIn)
                return true;
            // altZoomOut
            if (Config::altZoomOut && code == KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->zoomOut))
                return false;
            if (code == Config::altZoomOut)
                return true;
        }
        return (this->*FnCP)(a_event);
    }
    bool FirstPersonState::CP(InputEvent *a_event) { return (that->*FnCP)(a_event); }
    bool FirstPersonState::ProcessButton(ButtonEvent *a_event, PlayerControlsData *a_data)
    {
        auto code = KeyUtils::GetEventKeyMap(a_event);
        // altTogglePOV
        if (code == Config::altTogglePOV)
            a_event->userEvent = VarUtils::userEvent->togglePOV;
        // altZoomIn
        if (code == Config::altZoomIn)
            a_event->userEvent = VarUtils::userEvent->zoomIn;
        // altZoomOut
        if (code == Config::altZoomOut)
            a_event->userEvent = VarUtils::userEvent->zoomOut;

        if (Config::needModifier[code])
        {
            if (KeyUtils::GetKeyState(Config::needModifier[code]))
                return (this->*FnPB)(a_event, a_data);
            else
                return false;
        }
        return (this->*FnPB)(a_event, a_data);
    }
    bool FirstPersonState::PB(ButtonEvent *a_event, PlayerControlsData *a_data) { return (that->*FnPB)(a_event, a_data); }
    void FirstPersonState::Hook()
    {
        REL::Relocation<uintptr_t> vtable{VTABLE_FirstPersonState[1]};
        that = stl::unrestricted_cast<FirstPersonState *>(vtable.address());
        FnCP = stl::unrestricted_cast<FnCanProcess>(vtable.write_vfunc(1, &FirstPersonState::CanProcess));
        FnPB = stl::unrestricted_cast<FnProcessButton>(vtable.write_vfunc(4, &FirstPersonState::ProcessButton));
    }

    // AttackBlockHandler
    AttackBlockHandler *AttackBlockHandler::that;
    AttackBlockHandler::FnCanProcess AttackBlockHandler::FnCP;
    AttackBlockHandler::FnProcessButton AttackBlockHandler::FnPB;
    bool AttackBlockHandler::CanProcess(InputEvent *a_event)
    {
        return (this->*FnCP)(a_event);
    }
    bool AttackBlockHandler::CP(InputEvent *a_event) { return (that->*FnCP)(a_event); }
    bool AttackBlockHandler::ProcessButton(ButtonEvent *a_event, PlayerControlsData *a_data)
    {
        return (this->*FnPB)(a_event, a_data);
    }
    bool AttackBlockHandler::PB(ButtonEvent *a_event, PlayerControlsData *a_data) { return (that->*FnPB)(a_event, a_data); }
    void AttackBlockHandler::Hook()
    {
        REL::Relocation<uintptr_t> vtable{VTABLE_AttackBlockHandler[0]};
        that = stl::unrestricted_cast<AttackBlockHandler *>(vtable.address());
        FnCP = stl::unrestricted_cast<FnCanProcess>(vtable.write_vfunc(1, &AttackBlockHandler::CanProcess));
        FnPB = stl::unrestricted_cast<FnProcessButton>(vtable.write_vfunc(4, &AttackBlockHandler::ProcessButton));
    }

    // AttackBlockHandler
    TogglePOVHandler *TogglePOVHandler::that;
    TogglePOVHandler::FnCanProcess TogglePOVHandler::FnCP;
    TogglePOVHandler::FnProcessButton TogglePOVHandler::FnPB;
    bool TogglePOVHandler::CanProcess(InputEvent *a_event)
    {
        if (a_event->GetEventType() == INPUT_EVENT_TYPE::kButton)
        {
            auto evn = a_event->AsButtonEvent();
            auto code = KeyUtils::GetEventKeyMap(evn);
            // altTogglePOV
            if (Config::altTogglePOV && code == KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->togglePOV))
                return false;
            if (code == Config::altTogglePOV)
                return true;
        }
        return (this->*FnCP)(a_event);
    }
    bool TogglePOVHandler::CP(InputEvent *a_event) { return (that->*FnCP)(a_event); }
    bool TogglePOVHandler::ProcessButton(ButtonEvent *a_event, PlayerControlsData *a_data)
    {
        auto code = KeyUtils::GetEventKeyMap(a_event);
        // altTogglePOV
        if (code == Config::altTogglePOV)
            a_event->userEvent = VarUtils::userEvent->togglePOV;

        if (Config::needModifier[code])
        {
            if (KeyUtils::GetKeyState(Config::needModifier[code]))
                return (this->*FnPB)(a_event, a_data);
            else
                return false;
        }
        return (this->*FnPB)(a_event, a_data);
    }
    bool TogglePOVHandler::PB(ButtonEvent *a_event, PlayerControlsData *a_data) { return (that->*FnPB)(a_event, a_data); }
    void TogglePOVHandler::Hook()
    {
        REL::Relocation<uintptr_t> vtable{VTABLE_TogglePOVHandler[0]};
        that = stl::unrestricted_cast<TogglePOVHandler *>(vtable.address());
        FnCP = stl::unrestricted_cast<FnCanProcess>(vtable.write_vfunc(1, &TogglePOVHandler::CanProcess));
        FnPB = stl::unrestricted_cast<FnProcessButton>(vtable.write_vfunc(4, &TogglePOVHandler::ProcessButton));
    }

    // ThirdPersonState
    ThirdPersonState *ThirdPersonState::that;
    ThirdPersonState::FnCanProcess ThirdPersonState::FnCP;
    ThirdPersonState::FnProcessButton ThirdPersonState::FnPB;
    bool ThirdPersonState::CanProcess(InputEvent *a_event)
    {
        if (a_event->GetEventType() == INPUT_EVENT_TYPE::kButton)
        {
            auto evn = a_event->AsButtonEvent();
            auto code = KeyUtils::GetEventKeyMap(evn);
            // altTogglePOV
            if (Config::altTogglePOV && code == KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->togglePOV))
                return false;
            if (code == Config::altTogglePOV)
                return true;
            // altZoomIn
            if (Config::altZoomIn && code == KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->zoomIn))
                return false;
            if (code == Config::altZoomIn)
                return true;
            // altZoomOut
            if (Config::altZoomOut && code == KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->zoomOut))
                return false;
            if (code == Config::altZoomOut)
                return true;
        }
        return (this->*FnCP)(a_event);
    }
    bool ThirdPersonState::CP(InputEvent *a_event) { return (that->*FnCP)(a_event); }
    bool ThirdPersonState::ProcessButton(ButtonEvent *a_event, PlayerControlsData *a_data)
    {
        auto code = KeyUtils::GetEventKeyMap(a_event);
        // altTogglePOV
        if (code == Config::altTogglePOV)
            a_event->userEvent = VarUtils::userEvent->togglePOV;
        // altZoomIn
        if (code == Config::altZoomIn)
            a_event->userEvent = VarUtils::userEvent->zoomIn;
        // altZoomOut
        if (code == Config::altZoomOut)
            a_event->userEvent = VarUtils::userEvent->zoomOut;

        if (Config::needModifier[code])
        {
            if (KeyUtils::GetKeyState(Config::needModifier[code]))
                return (this->*FnPB)(a_event, a_data);
            else
                return false;
        }
        return (this->*FnPB)(a_event, a_data);
    }
    bool ThirdPersonState::PB(ButtonEvent *a_event, PlayerControlsData *a_data) { return (that->*FnPB)(a_event, a_data); }
    void ThirdPersonState::Hook()
    {
        REL::Relocation<uintptr_t> vtable{VTABLE_ThirdPersonState[1]};
        that = stl::unrestricted_cast<ThirdPersonState *>(vtable.address());
        FnCP = stl::unrestricted_cast<FnCanProcess>(vtable.write_vfunc(1, &ThirdPersonState::CanProcess));
        FnPB = stl::unrestricted_cast<FnProcessButton>(vtable.write_vfunc(4, &ThirdPersonState::ProcessButton));
    }

    void Hook()
    {
        MenuOpenHandler::Hook();
        AutoMoveHandler::Hook();
        FirstPersonState::Hook();
        AttackBlockHandler::Hook();
        TogglePOVHandler::Hook();
        ThirdPersonState::Hook();
    }
}
