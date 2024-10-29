#include "hook.h"

namespace Hook
{
typedef bool _doAction_t(RE::TESActionData *);
REL::Relocation<_doAction_t> _doAction{RELOCATION_ID(40551, 41557)};
void doAction(BGSAction *action)
{
    SKSE::GetTaskInterface()->AddTask([action]() {
        std::unique_ptr<TESActionData> data(TESActionData::Create());
        data->source = NiPointer<TESObjectREFR>(VarUtils::player);
        data->action = action;
        _doAction(data.get());
    });
}
bool CanDo()
{
    return true;
}
bool IsAttackReady()
{
    bool res;
    VarUtils::player->GetGraphVariableBool("IsAttackReady", std::ref(res));
    return res;
}
bool IsBashing()
{
    bool res;
    VarUtils::player->GetGraphVariableBool("IsBashing", std::ref(res));
    return res;
}
bool IsSprinting()
{
    return VarUtils::player->GetPlayerRuntimeData().playerFlags.isSprinting;
}
void NormalAttack()
{
    BGSAction *RightAttack = (BGSAction *)TESForm::LookupByID(0x13005);
    auto action = RightAttack;
    doAction(action);
}
void PowerAttack()
{
    BGSAction *RightPowerAttack = (BGSAction *)TESForm::LookupByID(0x13383);
    auto action = RightPowerAttack;
    doAction(action);
}
void SheatheAttack()
{
    NormalAttack();
}
//
// MenuOpenHandler
//
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
bool MenuOpenHandler::CP(InputEvent *a_event)
{
    return (that->*FnCP)(a_event);
}
bool MenuOpenHandler::ProcessButton(ButtonEvent *a_event)
{
    auto code = KeyUtils::GetEventKeyMap(a_event);
    // altTweenMenu
    if (code == Config::altTweenMenu)
        a_event->userEvent = VarUtils::userEvent->tweenMenu;

    if (Config::needModifier[code])
        if (!KeyUtils::GetKeyState(Config::needModifier[code]))
            return false;
    return (this->*FnPB)(a_event);
}
bool MenuOpenHandler::PB(ButtonEvent *a_event)
{
    return (that->*FnPB)(a_event);
}
void MenuOpenHandler::Hook()
{
    REL::Relocation<uintptr_t> vtable{VTABLE_MenuOpenHandler[0]};
    that = stl::unrestricted_cast<MenuOpenHandler *>(vtable.address());
    FnCP = stl::unrestricted_cast<FnCanProcess>(vtable.write_vfunc(1, &MenuOpenHandler::CanProcess));
    FnPB = stl::unrestricted_cast<FnProcessButton>(vtable.write_vfunc(5, &MenuOpenHandler::ProcessButton));
}

//
// AutoMoveHandler
//
AutoMoveHandler *AutoMoveHandler::that;
AutoMoveHandler::FnProcessButton AutoMoveHandler::FnPB;
bool AutoMoveHandler::ProcessButton(ButtonEvent *a_event, PlayerControlsData *a_data)
{
    auto code = KeyUtils::GetEventKeyMap(a_event);

    if (Config::needModifier[code])
        if (!KeyUtils::GetKeyState(Config::needModifier[code]))
            return false;
    return (this->*FnPB)(a_event, a_data);
}
void AutoMoveHandler::Hook()
{
    REL::Relocation<uintptr_t> vtable{VTABLE_AutoMoveHandler[0]};
    that = stl::unrestricted_cast<AutoMoveHandler *>(vtable.address());
    FnPB = stl::unrestricted_cast<FnProcessButton>(vtable.write_vfunc(4, &AutoMoveHandler::ProcessButton));
}

//
// FirstPersonState
//
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
bool FirstPersonState::CP(InputEvent *a_event)
{
    return (that->*FnCP)(a_event);
}
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
        if (!KeyUtils::GetKeyState(Config::needModifier[code]))
            return false;
    return (this->*FnPB)(a_event, a_data);
}
bool FirstPersonState::PB(ButtonEvent *a_event, PlayerControlsData *a_data)
{
    return (that->*FnPB)(a_event, a_data);
}
void FirstPersonState::Hook()
{
    REL::Relocation<uintptr_t> vtable{VTABLE_FirstPersonState[1]};
    that = stl::unrestricted_cast<FirstPersonState *>(vtable.address());
    FnCP = stl::unrestricted_cast<FnCanProcess>(vtable.write_vfunc(1, &FirstPersonState::CanProcess));
    FnPB = stl::unrestricted_cast<FnProcessButton>(vtable.write_vfunc(4, &FirstPersonState::ProcessButton));
}

//
// HeldStateHandler - AttackBlockHandler
//
AttackBlockHandler *AttackBlockHandler::that;
AttackBlockHandler::FnCanProcess AttackBlockHandler::FnCP;
AttackBlockHandler::FnProcessButton AttackBlockHandler::FnPB;
bool AttackBlockHandler::CanProcess(InputEvent *a_event)
{
    if (a_event->GetEventType() == INPUT_EVENT_TYPE::kButton)
    {
        auto evn = a_event->AsButtonEvent();
        auto code = KeyUtils::GetEventKeyMap(evn);
        if (code == Config::normalAttack || code == Config::powerAttack || code == Config::block ||
            code == KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->readyWeapon))
            return true;
    }
    return (this->*FnCP)(a_event);
}
bool AttackBlockHandler::CP(InputEvent *a_event)
{
    return (that->*FnCP)(a_event);
}
bool AttackBlockHandler::ProcessButton(ButtonEvent *a_event, PlayerControlsData *a_data)
{
    auto code = KeyUtils::GetEventKeyMap(a_event);
    if (CanDo())
    {
        // Attack
        if (code == Config::normalAttack)
        {
            // return (this->*FnPB)(a_event, a_data);
            if (VarUtils::player->IsBlocking() || IsBashing())
            {
                if (!IsBashing())
                {
                    VarUtils::player->NotifyAnimationGraph("bashStart");
                    VarUtils::player->NotifyAnimationGraph("bashRelease");
                }
            }
            else
                NormalAttack();
            return true;
        }
        if (code == Config::powerAttack)
        {
            // return (this->*FnPB)(a_event, a_data);
            if (VarUtils::player->IsBlocking() || IsBashing())
            {
                if (!IsBashing())
                {
                    VarUtils::player->NotifyAnimationGraph("bashStart");
                    KeyUtils::TrackKeyState(code, []() {
                        if (VarUtils::player->IsBlocking())
                            SKSE::GetTaskInterface()->AddTask(
                                []() { VarUtils::player->NotifyAnimationGraph("bashRelease"); });
                    });
                }
            }
            else
                PowerAttack();
            return true;
        }
        // Block
        if (code == Config::block)
        {
            if (!VarUtils::player->IsBlocking() && !IsBashing())
            {
                VarUtils::player->NotifyAnimationGraph(VarUtils::userEvent->blockStart);
                KeyUtils::TrackKeyState(code, []() {
                    if (VarUtils::player->IsBlocking())
                        SKSE::GetTaskInterface()->AddTask(
                            []() { VarUtils::player->NotifyAnimationGraph(VarUtils::userEvent->blockStop); });
                });
            }
            return true;
        }
        // SheatheAttack
        if (Config::enableSheatheAttack)
            if (KeyUtils::GetKeyState(Config::enableSheatheAttack))
            {
                if (!IsAttackReady())
                {
                    ReadyWeaponHandler::PB(a_event, a_data);
                    doAction((BGSAction *)TESForm::LookupByID(0x18BA8));
                    SKSE::GetTaskInterface()->AddTask(
                        []() { VarUtils::player->NotifyAnimationGraph(VarUtils::userEvent->forceRelease); });
                    if (code == Config::normalAttack)
                        NormalAttack();
                    else if (code == Config::powerAttack)
                        PowerAttack();
                    else if (code == KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->readyWeapon))
                        SheatheAttack();
                }
                else
                {
                    if (code == KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->readyWeapon))
                    {
                        ReadyWeaponHandler::PB(a_event, a_data);
                        SheatheAttack();
                    }
                }
                return true;
            }
    }
    return (this->*FnPB)(a_event, a_data);
}
bool AttackBlockHandler::PB(ButtonEvent *a_event, PlayerControlsData *a_data)
{
    return (that->*FnPB)(a_event, a_data);
}
void AttackBlockHandler::Hook()
{
    REL::Relocation<uintptr_t> vtable{VTABLE_AttackBlockHandler[0]};
    that = stl::unrestricted_cast<AttackBlockHandler *>(vtable.address());
    FnCP = stl::unrestricted_cast<FnCanProcess>(vtable.write_vfunc(1, &AttackBlockHandler::CanProcess));
    FnPB = stl::unrestricted_cast<FnProcessButton>(vtable.write_vfunc(4, &AttackBlockHandler::ProcessButton));
}

//
// HeldStateHandler - SprintHandler
//
SprintHandler *SprintHandler::that;
SprintHandler::FnCanProcess SprintHandler::FnCP;
SprintHandler::FnProcessButton SprintHandler::FnPB;
bool SprintHandler::CanProcess(InputEvent *a_event)
{
    if (a_event->GetEventType() == INPUT_EVENT_TYPE::kButton)
    {
        auto evn = a_event->AsButtonEvent();
        auto code = KeyUtils::GetEventKeyMap(evn);
    }
    return (this->*FnCP)(a_event);
}
bool SprintHandler::CP(InputEvent *a_event)
{
    return (that->*FnCP)(a_event);
}
bool SprintHandler::ProcessButton(ButtonEvent *a_event, PlayerControlsData *a_data)
{
    auto code = KeyUtils::GetEventKeyMap(a_event);
    if (Config::needModifier[code])
        if (!KeyUtils::GetKeyState(Config::needModifier[code]))
            return false;
    if (Config::enableHoldSprint && IsSprinting())
        KeyUtils::TrackKeyState(code, []() {
            SKSE::GetTaskInterface()->AddTask([]() {
                if (IsSprinting())
                    VarUtils::player->GetPlayerRuntimeData().playerFlags.isSprinting = false;
            });
        });
    return (this->*FnPB)(a_event, a_data);
}
bool SprintHandler::PB(ButtonEvent *a_event, PlayerControlsData *a_data)
{
    return (that->*FnPB)(a_event, a_data);
}
void SprintHandler::Hook()
{
    REL::Relocation<uintptr_t> vtable{VTABLE_SprintHandler[0]};
    that = stl::unrestricted_cast<SprintHandler *>(vtable.address());
    FnCP = stl::unrestricted_cast<FnCanProcess>(vtable.write_vfunc(1, &SprintHandler::CanProcess));
    FnPB = stl::unrestricted_cast<FnProcessButton>(vtable.write_vfunc(4, &SprintHandler::ProcessButton));
}

//
// HeldStateHandler - TogglePOVHandler
//
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
bool TogglePOVHandler::CP(InputEvent *a_event)
{
    return (that->*FnCP)(a_event);
}
bool TogglePOVHandler::ProcessButton(ButtonEvent *a_event, PlayerControlsData *a_data)
{
    auto code = KeyUtils::GetEventKeyMap(a_event);
    // altTogglePOV
    if (code == Config::altTogglePOV)
        a_event->userEvent = VarUtils::userEvent->togglePOV;

    if (Config::needModifier[code])
        if (!KeyUtils::GetKeyState(Config::needModifier[code]))
            return false;
    return (this->*FnPB)(a_event, a_data);
}
bool TogglePOVHandler::PB(ButtonEvent *a_event, PlayerControlsData *a_data)
{
    return (that->*FnPB)(a_event, a_data);
}
void TogglePOVHandler::Hook()
{
    REL::Relocation<uintptr_t> vtable{VTABLE_TogglePOVHandler[0]};
    that = stl::unrestricted_cast<TogglePOVHandler *>(vtable.address());
    FnCP = stl::unrestricted_cast<FnCanProcess>(vtable.write_vfunc(1, &TogglePOVHandler::CanProcess));
    FnPB = stl::unrestricted_cast<FnProcessButton>(vtable.write_vfunc(4, &TogglePOVHandler::ProcessButton));
}

//
// ReadyWeaponHandler
//
ReadyWeaponHandler *ReadyWeaponHandler::that;
ReadyWeaponHandler::FnCanProcess ReadyWeaponHandler::FnCP;
ReadyWeaponHandler::FnProcessButton ReadyWeaponHandler::FnPB;
bool ReadyWeaponHandler::CanProcess(InputEvent *a_event)
{
    return (this->*FnCP)(a_event);
}
bool ReadyWeaponHandler::CP(InputEvent *a_event)
{
    return (that->*FnCP)(a_event);
}
bool ReadyWeaponHandler::ProcessButton(ButtonEvent *a_event, PlayerControlsData *a_data)
{
    auto code = KeyUtils::GetEventKeyMap(a_event);
    if (Config::needModifier[code])
        if (!KeyUtils::GetKeyState(Config::needModifier[code]))
            return false;
    return (this->*FnPB)(a_event, a_data);
}
bool ReadyWeaponHandler::PB(ButtonEvent *a_event, PlayerControlsData *a_data)
{
    return (that->*FnPB)(a_event, a_data);
}
void ReadyWeaponHandler::Hook()
{
    REL::Relocation<uintptr_t> vtable{VTABLE_ReadyWeaponHandler[0]};
    that = stl::unrestricted_cast<ReadyWeaponHandler *>(vtable.address());
    FnCP = stl::unrestricted_cast<FnCanProcess>(vtable.write_vfunc(1, &ReadyWeaponHandler::CanProcess));
    FnPB = stl::unrestricted_cast<FnProcessButton>(vtable.write_vfunc(4, &ReadyWeaponHandler::ProcessButton));
}

//
// SneakHandler
//
SneakHandler *SneakHandler::that;
SneakHandler::FnCanProcess SneakHandler::FnCP;
SneakHandler::FnProcessButton SneakHandler::FnPB;
bool SneakHandler::CanProcess(InputEvent *a_event)
{
    if (a_event->GetEventType() == INPUT_EVENT_TYPE::kButton)
    {
        auto evn = a_event->AsButtonEvent();
        auto code = KeyUtils::GetEventKeyMap(evn);
    }
    return (this->*FnCP)(a_event);
}
bool SneakHandler::CP(InputEvent *a_event)
{
    return (that->*FnCP)(a_event);
}
bool SneakHandler::ProcessButton(ButtonEvent *a_event, PlayerControlsData *a_data)
{
    auto code = KeyUtils::GetEventKeyMap(a_event);
    if (Config::needModifier[code])
        if (!KeyUtils::GetKeyState(Config::needModifier[code]))
            return false;
    if (Config::enableHoldSneak)
        KeyUtils::TrackKeyState(code, [a_data]() {
            SKSE::GetTaskInterface()->AddTask([a_data]() {
                std::unique_ptr<ButtonEvent> evn(
                    ButtonEvent::Create(INPUT_DEVICE::kKeyboard, VarUtils::userEvent->sneak, 0, 1, 0));
                if (VarUtils::player->IsSneaking())
                    PB(evn.get(), a_data);
            });
        });
    return (this->*FnPB)(a_event, a_data);
}
bool SneakHandler::PB(ButtonEvent *a_event, PlayerControlsData *a_data)
{
    return (that->*FnPB)(a_event, a_data);
}
void SneakHandler::Hook()
{
    REL::Relocation<uintptr_t> vtable{VTABLE_SneakHandler[0]};
    that = stl::unrestricted_cast<SneakHandler *>(vtable.address());
    FnCP = stl::unrestricted_cast<FnCanProcess>(vtable.write_vfunc(1, &SneakHandler::CanProcess));
    FnPB = stl::unrestricted_cast<FnProcessButton>(vtable.write_vfunc(4, &SneakHandler::ProcessButton));
}

//
// ThirdPersonState
//
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
bool ThirdPersonState::CP(InputEvent *a_event)
{
    return (that->*FnCP)(a_event);
}
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
        if (!KeyUtils::GetKeyState(Config::needModifier[code]))
            return false;
    return (this->*FnPB)(a_event, a_data);
}
bool ThirdPersonState::PB(ButtonEvent *a_event, PlayerControlsData *a_data)
{
    return (that->*FnPB)(a_event, a_data);
}
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
    SprintHandler::Hook();
    TogglePOVHandler::Hook();
    ReadyWeaponHandler::Hook();
    SneakHandler::Hook();
    ThirdPersonState::Hook();
}
} // namespace Hook
