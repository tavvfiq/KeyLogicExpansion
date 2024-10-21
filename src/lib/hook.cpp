#include "hook.h"

using namespace RE;
using namespace REL;

namespace Hook
{

    // AutoMoveHandler
    AutoMoveHandler *AutoMoveHandler::that;
    AutoMoveHandler::FnCanProcess AutoMoveHandler::FnCP;
    AutoMoveHandler::FnProcessButton AutoMoveHandler::FnPB;
    bool AutoMoveHandler::CanProcess(InputEvent *a_event)
    {
        return (this->*FnCP)(a_event);
    }
    bool AutoMoveHandler::ProcessButton(ButtonEvent *a_event, PlayerControlsData *a_data)
    {
        return (this->*FnPB)(a_event, a_data);
    }
    void AutoMoveHandler::Hook()
    {
        REL::Relocation<uintptr_t> vtable{VTABLE_AutoMoveHandler[0]};
        that = stl::unrestricted_cast<AutoMoveHandler *>(vtable.address());
        FnCP = stl::unrestricted_cast<FnCanProcess>(vtable.write_vfunc(1, &AutoMoveHandler::CanProcess));
        FnPB = stl::unrestricted_cast<FnProcessButton>(vtable.write_vfunc(4, &AutoMoveHandler::ProcessButton));
    }

    /*
    // MovementHandler
    bool MovementHandler::CanProcess(InputEvent *a_event)
    {
        FnCanProcess fn = fnCP.at(*(uintptr_t *)this);
        return (*fn)(a_event);
    }
    bool MovementHandler::ProcessButton(ButtonEvent *a_event, PlayerControlsData *a_data)
    {
        FnProcessButton_Data fn = fnPB_D.at(*(uintptr_t *)this);
        return (*fn)(a_event, a_data);
    }
    void MovementHandler::Hook()
    {
        REL::Relocation<uintptr_t> vtable{VTABLE_MovementHandler[0]};
        fnCP.insert(std::pair<uintptr_t, FnCanProcess>(
            vtable.address(), stl::unrestricted_cast<FnCanProcess>(vtable.write_vfunc(1, &MovementHandler::CanProcess))));
        fnPB_D.insert(std::pair<uintptr_t, FnProcessButton_Data>(
            vtable.address(), stl::unrestricted_cast<FnProcessButton_Data>(vtable.write_vfunc(4, &MovementHandler::ProcessButton))));
    }*/


    // AttackBlockHandler
    AttackBlockHandler* AttackBlockHandler::that;
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
        FnPB = stl::unrestricted_cast<FnProcessButton>(vtable.write_vfunc(5, &AttackBlockHandler::ProcessButton));
    }

    // MenuOpenHandler
    MenuOpenHandler *MenuOpenHandler::that;
    MenuOpenHandler::FnCanProcess MenuOpenHandler::FnCP;
    MenuOpenHandler::FnProcessButton MenuOpenHandler::FnPB;
    bool MenuOpenHandler::CanProcess(InputEvent *a_event)
    {
        if (a_event->GetEventType() == INPUT_EVENT_TYPE::kButton)
        {
            std::thread(KeyQueue::RawQueuePusher, KeyQueue::RawInput{KeyUtils::GetEventKeyMap(a_event->AsButtonEvent()), a_event->AsButtonEvent()->heldDownSecs, a_event->AsButtonEvent()->value}).detach();
        }
        return (this->*FnCP)(a_event);
    }
    bool MenuOpenHandler::CP(InputEvent *a_event) { return (that->*FnCP)(a_event); }
    bool MenuOpenHandler::ProcessButton(ButtonEvent *a_event)
    {
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

    void Hook()
    {
        AutoMoveHandler::Hook();
        // MovementHandler::Hook();
        AttackBlockHandler::Hook();
        MenuOpenHandler::Hook();
    }
}
