#include "hook.h"

namespace Hook
{
enum class AttackType : std::uint8_t
{
    Null = 0,
    Right = 1,
    Left = 2,
    Dual = 3
};
typedef struct
{
    BGSAction *wantAction;
    uint64_t time;
    AttackType type;
} ActionQueue;
static ActionQueue queue;
uint32_t isInQueue = false;

AttackType currentType;
bool leftMagic;
bool rightMagic;

bool IsAttacking()
{
    bool res;
    VarUtils::player->GetGraphVariableBool("IsAttacking", std::ref(res));
    return res;
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
bool IsJumping()
{
    bool res;
    VarUtils::player->GetGraphVariableBool("bInJumpState", std::ref(res));
    return res;
}
bool IsRiding()
{
    return (VarUtils::player->AsActorState()->actorState1.sitSleepState == SIT_SLEEP_STATE::kRidingMount);
}
bool IsInKillmove()
{
    return VarUtils::player->GetActorRuntimeData().boolFlags.all(Actor::BOOL_FLAGS::kIsInKillMove);
}
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
    if (VarUtils::ui->numPausesGame > 0 || IsInKillmove() || IsRiding() || VarUtils::ui->IsMenuOpen("Dialogue Menu") ||
        VarUtils::player->AsActorState()->actorState1.sitSleepState != SIT_SLEEP_STATE::kNormal ||
        (VarUtils::ctrlMap->enabledControls.underlying() &
         ((uint32_t)UserEvents::USER_EVENT_FLAG::kMovement & (uint32_t)UserEvents::USER_EVENT_FLAG::kLooking)) !=
            ((uint32_t)UserEvents::USER_EVENT_FLAG::kMovement & (uint32_t)UserEvents::USER_EVENT_FLAG::kLooking))
        return false;
    logger::trace("{} or {}", IsSprinting(), IsJumping());
    AttackType type = AttackType::Null;
    bool _leftMagic = false;
    bool _rightMagic = false;
    auto shield = VarUtils::player->GetWornArmor(RE::BGSBipedObjectForm::BipedObjectSlot::kShield);
    RE::TESForm *lHand = nullptr;
    RE::TESForm *rHand = nullptr;
    RE::MagicItem *lMagic = nullptr;
    if (!shield)
        lMagic = VarUtils::player->GetActorRuntimeData().selectedSpells[RE::Actor::SlotTypes::kLeftHand];
    if (lMagic && lMagic->GetSpellType() != RE::MagicSystem::SpellType::kPoison &&
        lMagic->GetSpellType() != RE::MagicSystem::SpellType::kEnchantment)
        _leftMagic = true;
    if (!shield && !_leftMagic)
    {
        lHand = VarUtils::player->GetActorRuntimeData().currentProcess->GetEquippedLeftHand();
        if (lHand)
        {
            if (lHand->As<RE::TESObjectWEAP>()->GetWeaponType() == RE::WEAPON_TYPE::kBow ||
                lHand->As<RE::TESObjectWEAP>()->GetWeaponType() == RE::WEAPON_TYPE::kCrossbow)
                return false;
            else if (lHand->As<RE::TESObjectWEAP>()->GetWeaponType() == RE::WEAPON_TYPE::kTwoHandSword ||
                     lHand->As<RE::TESObjectWEAP>()->GetWeaponType() == RE::WEAPON_TYPE::kTwoHandAxe)
            {
                currentType = AttackType::Right;
                leftMagic = false;
                rightMagic = false;
                return true;
            }
            else if (lHand->As<RE::TESObjectWEAP>()->GetWeaponType() == RE::WEAPON_TYPE::kStaff)
                _leftMagic = true;
            else
                type = AttackType::Left;
        }
    }
    auto rMagic = VarUtils::player->GetActorRuntimeData().selectedSpells[RE::Actor::SlotTypes::kRightHand];
    if (rMagic && rMagic->GetSpellType() != RE::MagicSystem::SpellType::kPoison &&
        rMagic->GetSpellType() != RE::MagicSystem::SpellType::kEnchantment)
        _rightMagic = true;
    if (!_rightMagic)
    {
        rHand = VarUtils::player->GetActorRuntimeData().currentProcess->GetEquippedRightHand();
        if (rHand)
        {
            if (rHand->As<RE::TESObjectWEAP>()->GetWeaponType() == RE::WEAPON_TYPE::kStaff)
                _rightMagic = true;
            else if (rHand->As<RE::TESObjectWEAP>()->GetWeaponType() == RE::WEAPON_TYPE::kOneHandSword ||
                     rHand->As<RE::TESObjectWEAP>()->GetWeaponType() == RE::WEAPON_TYPE::kOneHandDagger ||
                     rHand->As<RE::TESObjectWEAP>()->GetWeaponType() == RE::WEAPON_TYPE::kOneHandAxe ||
                     rHand->As<RE::TESObjectWEAP>()->GetWeaponType() == RE::WEAPON_TYPE::kOneHandMace)
            {
                if (type == AttackType::Left)
                    type = AttackType::Dual;
                else
                    type = AttackType::Right;
            }
        }
    }
    leftMagic = _leftMagic;
    rightMagic = _rightMagic;
    if (leftMagic || rightMagic)
        return true;
    if (!lHand && !rHand)
    {
        if (shield)
            type = AttackType::Right;
        else
            type = AttackType::Dual;
    }
    if (type == AttackType::Null)
        return false;
    currentType = type;
    return true;
}
bool CanBash()
{
    auto shield = VarUtils::player->GetWornArmor(RE::BGSBipedObjectForm::BipedObjectSlot::kShield);
    if (shield)
        return true;
    auto lHand = VarUtils::player->GetActorRuntimeData().currentProcess->GetEquippedLeftHand();
    if (lHand)
    {
        if (lHand->As<RE::TESObjectWEAP>()->GetWeaponType() == RE::WEAPON_TYPE::kTwoHandSword ||
            lHand->As<RE::TESObjectWEAP>()->GetWeaponType() == RE::WEAPON_TYPE::kTwoHandAxe)
            return true;
        else if (lHand->As<RE::TESObjectWEAP>()->GetWeaponType() == RE::WEAPON_TYPE::kStaff)
            return false;
    }
    else
    {
        auto rHand = VarUtils::player->GetActorRuntimeData().currentProcess->GetEquippedRightHand();
        if (rHand)
        {
            if (rHand->As<RE::TESObjectWEAP>()->GetWeaponType() == RE::WEAPON_TYPE::kStaff)
                return false;
            else if (rHand->As<RE::TESObjectWEAP>()->GetWeaponType() == RE::WEAPON_TYPE::kOneHandSword ||
                     rHand->As<RE::TESObjectWEAP>()->GetWeaponType() == RE::WEAPON_TYPE::kOneHandDagger ||
                     rHand->As<RE::TESObjectWEAP>()->GetWeaponType() == RE::WEAPON_TYPE::kOneHandAxe ||
                     rHand->As<RE::TESObjectWEAP>()->GetWeaponType() == RE::WEAPON_TYPE::kOneHandMace)
                return true;
        }
    }
    return false;
}
void ActionPreInput(std::function<void()> func)
{
    while (true)
    {
        if ((TimeUtils::GetTime() - queue.time) / 1000.0 > 100)
        {
            isInQueue = 0;
            break;
        }
        if (isInQueue == 1 && Compatibility::CanNormalAttack() && queue.type == currentType)
        {
            func();
            isInQueue = 0;
            break;
        }
        else if (isInQueue == 2 && Compatibility::CanPowerAttack() && queue.type == currentType)
        {
            func();
            isInQueue = 0;
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}
void NormalAttack()
{
    BGSAction *RightAttack = (BGSAction *)TESForm::LookupByID(0x13005);
    BGSAction *LeftAttack = (BGSAction *)TESForm::LookupByID(0x13004);
    BGSAction *DualAttack = (BGSAction *)TESForm::LookupByID(0x50C96);
    BGSAction *action;
    switch (currentType)
    {
    case AttackType::Right:
        action = RightAttack;
        break;
    case AttackType::Left:
        action = LeftAttack;
        break;
    case AttackType::Dual:
        action = DualAttack;
        break;
    }
    if ((Compatibility::MCO || Compatibility::BFCO) && IsAttacking())
    {
        action = RightAttack;
        if (!Compatibility::CanNormalAttack())
        {
            queue.wantAction = action;
            queue.time = TimeUtils::GetTime();
            queue.type = currentType;
            if (!isInQueue)
            {
                isInQueue = 1;
                if (Compatibility::MCO)
                    std::thread(ActionPreInput, []() { doAction(queue.wantAction); }).detach();
                else if (Compatibility::BFCO)
                    std::thread(ActionPreInput, []() {
                        VarUtils::player->AsActorValueOwner()->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kDamage,
                                                                                 RE::ActorValue::kStamina, 0);
                        SKSE::GetTaskInterface()->AddTask(
                            []() { VarUtils::player->NotifyAnimationGraph("attackStart"); });
                    }).detach();
            }
            else
                isInQueue = 1;
            return;
        }
    }
    doAction(action);
}
void PowerAttack()
{
    BGSAction *RightPowerAttack = (BGSAction *)TESForm::LookupByID(0x13383);
    BGSAction *LeftPowerAttack = (BGSAction *)TESForm::LookupByID(0x2E2F6);
    BGSAction *DualPowerAttack = (BGSAction *)TESForm::LookupByID(0x2E2F7);
    BGSAction *action;
    switch (currentType)
    {
    case AttackType::Right:
        action = RightPowerAttack;
        break;
    case AttackType::Left:
        action = LeftPowerAttack;
        break;
    case AttackType::Dual:
        action = DualPowerAttack;
        break;
    }
    if ((Compatibility::MCO || Compatibility::BFCO) && IsAttacking())
    {
        action = RightPowerAttack;
        if (!Compatibility::CanPowerAttack())
        {
            queue.wantAction = action;
            queue.time = TimeUtils::GetTime();
            queue.type = currentType;
            if (!isInQueue)
            {
                isInQueue = 2;
                if (Compatibility::MCO)
                    std::thread(ActionPreInput, []() { doAction(queue.wantAction); }).detach();
                else if (Compatibility::BFCO)
                    std::thread(ActionPreInput, []() {
                        SKSE::GetTaskInterface()->AddTask([]() {
                            VarUtils::player->AsActorValueOwner()->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kDamage,
                                                                                     RE::ActorValue::kStamina, 0);
                            VarUtils::player->NotifyAnimationGraph("attackPowerStartInPlace");
                        });
                    }).detach();
            }
            else
                isInQueue = 2;
            return;
        }
    }
    doAction(action);
}
void SheatheAttack()
{
    // NormalAttack();
}

//
// AnimationGraphEventSink
//
AnimationGraphEventSink *AnimationGraphEventSink::GetSingleton()
{
    static AnimationGraphEventSink singleton;
    return &singleton;
}
void AnimationGraphEventSink::Install()
{
    VarUtils::player->AddAnimationGraphEventSink(AnimationGraphEventSink::GetSingleton());
}
RE::BSEventNotifyControl AnimationGraphEventSink::ProcessEvent(
    const RE::BSAnimationGraphEvent *a_event, RE::BSTEventSource<RE::BSAnimationGraphEvent> *a_eventSource)
{
    if (!a_event || !a_eventSource)
        return RE::BSEventNotifyControl::kContinue;

    if (a_event->tag == "MCO_WinOpen" || a_event->tag == "BFCO_NextWinStart")
        Compatibility::normalAttackWin = true;
    else if (a_event->tag == "MCO_PowerWinOpen" || a_event->tag == "BFCO_NextPowerWinStart")
        Compatibility::powerAttackWin = true;
    else if (a_event->tag == "MCO_WinClose")
        Compatibility::normalAttackWin = false;
    else if (a_event->tag == "MCO_PowerWinClose")
        Compatibility::powerAttackWin = false;
    else if (a_event->tag == "BFCO_DIY_EndLoop")
    {
        Compatibility::normalAttackWin = false;
        Compatibility::powerAttackWin = false;
    }

    return RE::BSEventNotifyControl::kContinue;
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
        while (Stances::enableStances)
        {
            if (Stances::StancesModfier)
                if (!KeyUtils::GetKeyState(Stances::StancesModfier))
                    break;
            if (code == Stances::ChangeToLow)
                Stances::ChangeStanceTo(Stances::Stances::Low);
            else if (code == Stances::ChangeToMid)
                Stances::ChangeStanceTo(Stances::Stances::Mid);
            else if (code == Stances::ChangeToHigh)
                Stances::ChangeStanceTo(Stances::Stances::High);
            break;
        }
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
        if (Compatibility::BFCO || code == Config::BFCO_ComboAttack)
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
            if (Compatibility::BFCO && (IsSprinting() || IsJumping()))
            {
                VarUtils::player->AsActorValueOwner()->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kDamage,
                                                                         RE::ActorValue::kStamina, 0);
                VarUtils::player->NotifyAnimationGraph("attackStartSprint");
                return true;
            }
            if (KeyUtils::GetKeyState(Config::BFCO_SpecialAttackModifier))
            {
                if (VarUtils::player->AsActorValueOwner()->GetActorValue(RE::ActorValue::kStamina) > 10)
                    if (VarUtils::player->NotifyAnimationGraph("attackStartDualWield"))
                        VarUtils::player->AsActorValueOwner()->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kDamage,
                                                                                 RE::ActorValue::kStamina, -60);
                return true;
            }
            if (leftMagic || rightMagic)
            {
                a_event->userEvent = VarUtils::userEvent->rightAttack;
                return (this->*FnPB)(a_event, a_data);
            }
            a_event->userEvent = "";
            if (VarUtils::player->IsBlocking() || IsBashing())
            {
                if (CanBash())
                {
                    a_event->userEvent = VarUtils::userEvent->rightAttack;
                    (this->*FnPB)(a_event, a_data);
                    VarUtils::player->NotifyAnimationGraph("bashRelease");
                    return true;
                }
                else
                    return false;
            }
            NormalAttack();
            return (this->*FnPB)(a_event, a_data);
        }
        // Power Attack
        if (code == Config::powerAttack)
        {
            if (Compatibility::BFCO && (IsSprinting() || IsJumping()))
            {
                VarUtils::player->AsActorValueOwner()->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kDamage,
                                                                         RE::ActorValue::kStamina, 0);
                VarUtils::player->NotifyAnimationGraph("attackPowerStart_Sprint");
                return true;
            }
            if (KeyUtils::GetKeyState(Config::BFCO_SpecialAttackModifier))
            {
                if (VarUtils::player->AsActorValueOwner()->GetActorValue(RE::ActorValue::kStamina) > 10)
                    if (VarUtils::player->NotifyAnimationGraph("attackPowerStartDualWield"))
                        VarUtils::player->AsActorValueOwner()->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kDamage,
                                                                                 RE::ActorValue::kStamina, -80);
                return true;
            }
            if (leftMagic || rightMagic)
            {
                a_event->userEvent = VarUtils::userEvent->leftAttack;
                return (this->*FnPB)(a_event, a_data);
            }
            a_event->userEvent = "";
            if (VarUtils::player->IsBlocking() || IsBashing())
            {
                if (CanBash())
                {
                    a_event->userEvent = VarUtils::userEvent->rightAttack;
                    return (this->*FnPB)(a_event, a_data);
                }
                else
                    return false;
            }
            PowerAttack();
            return (this->*FnPB)(a_event, a_data);
        }
        // Block
        if (code == Config::block)
        {
            a_event->userEvent = "";
            if (!VarUtils::player->IsBlocking() && !IsBashing())
            {
                VarUtils::player->AsActorState()->actorState2.wantBlocking = true;
                VarUtils::player->NotifyAnimationGraph(VarUtils::userEvent->blockStart);
                KeyUtils::TrackKeyState(code, []() {
                    while (VarUtils::player->IsBlocking() || IsBashing())
                    {
                        std::this_thread::sleep_for(std::chrono::milliseconds(20));
                        SKSE::GetTaskInterface()->AddTask([]() {
                            VarUtils::player->AsActorState()->actorState2.wantBlocking = false;
                            VarUtils::player->NotifyAnimationGraph(VarUtils::userEvent->blockStop);
                        });
                    }
                });
            }
            if (VarUtils::player->IsBlocking())
                KeyUtils::TrackKeyState(code, []() {
                    while (VarUtils::player->IsBlocking() || IsBashing())
                    {
                        std::this_thread::sleep_for(std::chrono::milliseconds(20));
                        SKSE::GetTaskInterface()->AddTask([]() {
                            VarUtils::player->AsActorState()->actorState2.wantBlocking = false;
                            VarUtils::player->NotifyAnimationGraph(VarUtils::userEvent->blockStop);
                        });
                    }
                });
            (this->*FnPB)(a_event, a_data);
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
        // BFCO ComboAttack
        if (code == Config::BFCO_ComboAttack)
        {
            if (!IsAttacking())
                return false;
            VarUtils::player->NotifyAnimationGraph("BFCOAttackStart_Comb");
            if (VarUtils::player->AsActorValueOwner()->GetActorValue(RE::ActorValue::kStamina) > 10)
                if (VarUtils::player->NotifyAnimationGraph("attackPowerStartForward"))
                    VarUtils::player->AsActorValueOwner()->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kDamage,
                                                                             RE::ActorValue::kStamina, -80);
        }
    }
    if (IsRiding() && Config::enableReverseHorseAttack)
    {
        if (code == Config::normalAttack || code == KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->rightAttack))
            a_event->userEvent = VarUtils::userEvent->leftAttack;
        else if (code == Config::powerAttack || code == KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->leftAttack))
            a_event->userEvent = VarUtils::userEvent->rightAttack;
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
// MovementHandler
//
MovementHandler *MovementHandler::that;
MovementHandler::FnCanProcess MovementHandler::FnCP;
MovementHandler::FnProcessButton MovementHandler::FnPB;
bool MovementHandler::CanProcess(InputEvent *a_event)
{
    return (this->*FnCP)(a_event);
}
bool MovementHandler::CP(InputEvent *a_event)
{
    return (that->*FnCP)(a_event);
}
bool MovementHandler::ProcessButton(ButtonEvent *a_event, PlayerControlsData *a_data)
{
    auto code = KeyUtils::GetEventKeyMap(a_event);
    if (Config::needModifier[code])
        if (!KeyUtils::GetKeyState(Config::needModifier[code]))
            return false;
    if (!IsSprinting())
    {
        auto sprint = KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->sprint);
        if (!KeyUtils::GetKeyState(sprint))
            return (this->*FnPB)(a_event, a_data);
        if (Config::needModifier[sprint])
            if (!KeyUtils::GetKeyState(Config::needModifier[sprint]))
                return (this->*FnPB)(a_event, a_data);
        VarUtils::player->GetPlayerRuntimeData().playerFlags.isSprinting = true;
        KeyUtils::TrackKeyState(sprint, []() {
            SKSE::GetTaskInterface()->AddTask([]() {
                if (IsSprinting())
                    VarUtils::player->GetPlayerRuntimeData().playerFlags.isSprinting = false;
            });
        });
    }
    return (this->*FnPB)(a_event, a_data);
}
bool MovementHandler::PB(ButtonEvent *a_event, PlayerControlsData *a_data)
{
    return (that->*FnPB)(a_event, a_data);
}
void MovementHandler::Hook()
{
    REL::Relocation<uintptr_t> vtable{VTABLE_MovementHandler[0]};
    that = stl::unrestricted_cast<MovementHandler *>(vtable.address());
    FnCP = stl::unrestricted_cast<FnCanProcess>(vtable.write_vfunc(1, &MovementHandler::CanProcess));
    FnPB = stl::unrestricted_cast<FnProcessButton>(vtable.write_vfunc(4, &MovementHandler::ProcessButton));
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
    if (IsAttacking())
    {
        if (Compatibility::BFCO || Compatibility::MCO)
        {
            if (Compatibility::CanRecovery())
                return (this->*FnCP)(a_event);
            else
                return false;
        }
    }
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
    MovementHandler::Hook();
    ReadyWeaponHandler::Hook();
    SneakHandler::Hook();
    ThirdPersonState::Hook();
}
} // namespace Hook
