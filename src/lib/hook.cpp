#pragma once
#include "hook.h"
#include "module.h"
#include "keyInputQueue.h"

using namespace RE;
using namespace REL;



// HookAutoMoveHandler
std::unordered_map<uintptr_t, HookAutoMoveHandler::FnCanProcess> HookAutoMoveHandler::fnCP;
std::unordered_map<uintptr_t, HookAutoMoveHandler::FnProcessButton> HookAutoMoveHandler::fnPB;
bool HookAutoMoveHandler::CanProcess(InputEvent *a_event)
{
    //FnCanProcess fn = fnCP.at(*(uintptr_t *)this);
    if (a_event->GetEventType() == INPUT_EVENT_TYPE::kButton) {
        if (a_event->AsButtonEvent()->IsDown())
            keyQueue::pushInKey(keyQueue::keyInput{KeyUtils::GetEventKeyMap(a_event->AsButtonEvent()), 0});
        else if (a_event->AsButtonEvent()->IsUp())
            keyQueue::updateKey(keyQueue::keyInput{KeyUtils::GetEventKeyMap(a_event->AsButtonEvent()), a_event->AsButtonEvent()->HeldDuration()});
    }
    return false;
    //return (this->*fn)(a_event);
}
bool HookAutoMoveHandler::ProcessButton(ButtonEvent *a_event, PlayerControlsData *a_data)
{
    FnProcessButton fn = fnPB.at(*(uintptr_t *)this);
    return (this->*fn)(a_event, a_data);
}
void HookAutoMoveHandler::Hook()
{
    REL::Relocation<uintptr_t> vtable{VTABLE_AutoMoveHandler[0]};
    fnCP.insert(std::pair<uintptr_t, FnCanProcess>(
        vtable.address(), stl::unrestricted_cast<FnCanProcess>(vtable.write_vfunc(1, &HookAutoMoveHandler::CanProcess))));
    fnPB.insert(std::pair<uintptr_t, FnProcessButton>(
        vtable.address(), stl::unrestricted_cast<FnProcessButton>(vtable.write_vfunc(5, &HookAutoMoveHandler::ProcessButton))));
}



// HookMenuOpenHandler
bool HookMenuOpenHandler::CanProcess(InputEvent *a_event)
{
    FnCanProcess fn = fnCP.at(*(uintptr_t *)this);
    if (Config::enableAltTweenMenu && a_event->GetEventType() == INPUT_EVENT_TYPE::kButton)
        if (a_event->AsButtonEvent()->idCode == Menu::AltTweenMenu::altTweenMenuKey[(int)a_event->GetDevice()])
            return true;
    return (this->*fn)(a_event);
}
std::unordered_map<uintptr_t, HookMenuOpenHandler::FnCanProcess> HookMenuOpenHandler::fnCP;
bool HookMenuOpenHandler::ProcessButton(ButtonEvent *a_event)
{
    FnProcessButton fn = fnPB.at(*(uintptr_t *)this);

    if (Config::disableOriTweenMenu && a_event->idCode == Menu::AltTweenMenu::oriTweenMenuKey[(int)a_event->GetDevice()])
        return true;
    else if (Config::enableAltTweenMenu && a_event->idCode == Menu::AltTweenMenu::altTweenMenuKey[(int)a_event->GetDevice()])
    {
        a_event->userEvent = Var::userEvent->tweenMenu;
        return (this->*fn)(a_event);
    }
    return (this->*fn)(a_event);
}
std::unordered_map<uintptr_t, HookMenuOpenHandler::FnProcessButton> HookMenuOpenHandler::fnPB;
void HookMenuOpenHandler::Hook()
{
    KeyUtils::GetVanillaKeyMap(Menu::AltTweenMenu::oriTweenMenuKey, Var::userEvent->tweenMenu);
    Menu::AltTweenMenu::altTweenMenuKey[0] = 34;
    REL::Relocation<uintptr_t> vtable{VTABLE_MenuOpenHandler[0]};
    fnCP.insert(std::pair<uintptr_t, FnCanProcess>(
        vtable.address(), stl::unrestricted_cast<FnCanProcess>(vtable.write_vfunc(1, &HookMenuOpenHandler::CanProcess))));
    fnPB.insert(std::pair<uintptr_t, FnProcessButton>(
        vtable.address(), stl::unrestricted_cast<FnProcessButton>(vtable.write_vfunc(5, &HookMenuOpenHandler::ProcessButton))));
}
