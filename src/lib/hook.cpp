#include "hook.h"

namespace Hook
{
using AttackType = Style::AttackType;
typedef struct
{
    RE::TESIdleForm *target;
    uint64_t time;
} ActionQueue;
static ActionQueue queue;
static uint8_t isInQueue = 0;
static uint64_t startTime = 0;

// typedef bool _doAction_t(RE::TESActionData *);
// REL::Relocation<_doAction_t> _doAction{RELOCATION_ID(40551, 41557)};
void doAction(RE::TESIdleForm *idle)
{
    SKSE::GetTaskInterface()->AddTask([idle]() {
        VarUtils::player->GetActorRuntimeData().currentProcess->PlayIdle(VarUtils::player, idle, nullptr);
    });
}

void Recover()
{
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        if ((PlayerStatus::IsBlocking() || PlayerStatus::IsBashing()) && !KeyUtils::GetKeyState(Config::block))
            SKSE::GetTaskInterface()->AddTask([]() {
                if (VarUtils::player->NotifyAnimationGraph("blockStop"))
                    VarUtils::player->AsActorState()->actorState2.wantBlocking = false;
            });
        if (Config::enableHoldSneak && PlayerStatus::IsSneaking() &&
            !KeyUtils::GetKeyState(KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->sneak)))
            SKSE::GetTaskInterface()->AddTask([]() { VarUtils::player->NotifyAnimationGraph("SneakStop"); });
    }
}

void DetectStyle()
{
    uint8_t leftWeapen = 0;
    uint8_t rightWeapen = 0;
    uint8_t leftMagic = 0;
    uint8_t rightMagic = 0;
    auto shield = VarUtils::player->GetWornArmor(RE::BGSBipedObjectForm::BipedObjectSlot::kShield);
    RE::TESForm *lHand = nullptr;
    RE::TESForm *rHand = nullptr;
    RE::MagicItem *lMagic = nullptr;
    if (!shield)
        lMagic = VarUtils::player->GetActorRuntimeData().selectedSpells[RE::Actor::SlotTypes::kLeftHand];
    if (lMagic && lMagic->GetSpellType() != RE::MagicSystem::SpellType::kPoison &&
        lMagic->GetSpellType() != RE::MagicSystem::SpellType::kEnchantment)
        leftMagic = 1;
    if (!shield && !leftMagic)
    {
        lHand = VarUtils::player->GetActorRuntimeData().currentProcess->GetEquippedLeftHand();
        // Specially Torch
        if (lHand && lHand->GetFormID() == 0x1D4EC)
        {
            lHand = nullptr;
            leftWeapen = 2;
        }
        if (lHand)
        {
            if (lHand->As<RE::TESObjectWEAP>()->GetWeaponType() == RE::WEAPON_TYPE::kBow ||
                lHand->As<RE::TESObjectWEAP>()->GetWeaponType() == RE::WEAPON_TYPE::kCrossbow)
            {
                Style::currentStyle = Style::Styles::Bow;
                return;
            }
            else if (lHand->As<RE::TESObjectWEAP>()->GetWeaponType() == RE::WEAPON_TYPE::kTwoHandSword ||
                     lHand->As<RE::TESObjectWEAP>()->GetWeaponType() == RE::WEAPON_TYPE::kTwoHandAxe)
            {
                Style::currentStyle = Style::Styles::TwoHand;
                return;
            }
            else if (lHand->As<RE::TESObjectWEAP>()->GetWeaponType() == RE::WEAPON_TYPE::kStaff)
                leftMagic = 2;
            else
                leftWeapen = 1;
        }
    }
    auto rMagic = VarUtils::player->GetActorRuntimeData().selectedSpells[RE::Actor::SlotTypes::kRightHand];
    if (rMagic && rMagic->GetSpellType() != RE::MagicSystem::SpellType::kPoison &&
        rMagic->GetSpellType() != RE::MagicSystem::SpellType::kEnchantment)
        rightMagic = 1;
    if (!rightMagic)
    {
        rHand = VarUtils::player->GetActorRuntimeData().currentProcess->GetEquippedRightHand();
        if (rHand)
        {
            if (rHand->As<RE::TESObjectWEAP>()->GetWeaponType() == RE::WEAPON_TYPE::kStaff)
                rightMagic = 2;
            else if (rHand->As<RE::TESObjectWEAP>()->GetWeaponType() == RE::WEAPON_TYPE::kOneHandSword ||
                     rHand->As<RE::TESObjectWEAP>()->GetWeaponType() == RE::WEAPON_TYPE::kOneHandDagger ||
                     rHand->As<RE::TESObjectWEAP>()->GetWeaponType() == RE::WEAPON_TYPE::kOneHandAxe ||
                     rHand->As<RE::TESObjectWEAP>()->GetWeaponType() == RE::WEAPON_TYPE::kOneHandMace)
                rightWeapen = 1;
        }
    }
    if (shield)
    {
        if (rightMagic == 1)
            Style::currentStyle = Style::Styles::ShieldMagic;
        else if (rightMagic == 2)
            Style::currentStyle = Style::Styles::ShieldStaff;
        if (rightWeapen == 1)
            Style::currentStyle = Style::Styles::ShieldSword;
        else if (rightWeapen == 0)
            Style::currentStyle = Style::Styles::ShieldFist;
    }
    else if (leftMagic == 1)
    {
        if (rightMagic == 1)
            Style::currentStyle = Style::Styles::DualMagic;
        else if (rightMagic == 2)
            Style::currentStyle = Style::Styles::MagicStaff;
        if (rightWeapen == 1)
            Style::currentStyle = Style::Styles::MagicSword;
        else if (rightWeapen == 0)
            Style::currentStyle = Style::Styles::MagicFist;
    }
    else if (leftMagic == 2)
    {
        if (rightMagic == 1)
            Style::currentStyle = Style::Styles::StaffMagic;
        else if (rightMagic == 2)
            Style::currentStyle = Style::Styles::DualStaff;
        if (rightWeapen == 1)
            Style::currentStyle = Style::Styles::StaffSword;
        else if (rightWeapen == 0)
            Style::currentStyle = Style::Styles::StaffFist;
    }
    else if (leftWeapen == 1)
    {
        if (rightMagic == 1)
            Style::currentStyle = Style::Styles::SwordMagic;
        else if (rightMagic == 2)
            Style::currentStyle = Style::Styles::SwordStaff;
        if (rightWeapen == 1)
            Style::currentStyle = Style::Styles::DualSword;
        else if (rightWeapen == 0)
            Style::currentStyle = Style::Styles::SwordFist;
    }
    else if (leftWeapen == 0)
    {
        if (rightMagic == 1)
            Style::currentStyle = Style::Styles::OnlyMagic;
        else if (rightMagic == 2)
            Style::currentStyle = Style::Styles::OnlyStaff;
        if (rightWeapen == 1)
            Style::currentStyle = Style::Styles::OnlySword;
        else if (rightWeapen == 0)
            Style::currentStyle = Style::Styles::DualFist;
    }
    else
        Style::currentStyle = Style::Styles::Null;
}
bool CanDo()
{
    if (VarUtils::ui->numPausesGame > 0 || PlayerStatus::IsInKillmove() || PlayerStatus::IsRiding() ||
        VarUtils::ui->IsMenuOpen("Dialogue Menu") ||
        VarUtils::player->AsActorState()->actorState1.sitSleepState != SIT_SLEEP_STATE::kNormal ||
        (VarUtils::ctrlMap->enabledControls.underlying() &
         ((uint32_t)UserEvents::USER_EVENT_FLAG::kMovement & (uint32_t)UserEvents::USER_EVENT_FLAG::kLooking)) !=
            ((uint32_t)UserEvents::USER_EVENT_FLAG::kMovement & (uint32_t)UserEvents::USER_EVENT_FLAG::kLooking))
        return false;
    DetectStyle();
    if (Style::currentStyle == Style::Styles::Bow)
        return false;
    return true;
}
bool CanBash()
{
    if (Style::currentStyle == Style::Styles::TwoHand || Style::currentStyle == Style::Styles::DualSword ||
        Style::currentStyle == Style::Styles::DualFist || Style::currentStyle == Style::Styles::DualMagic ||
        Style::currentStyle == Style::Styles::OnlySword || Style::currentStyle == Style::Styles::OnlyMagic ||
        Style::currentStyle == Style::Styles::OnlyStaff || Style::currentStyle == Style::Styles::ShieldSword ||
        Style::currentStyle == Style::Styles::ShieldFist || Style::currentStyle == Style::Styles::ShieldMagic ||
        Style::currentStyle == Style::Styles::ShieldStaff || Style::currentStyle == Style::Styles::ShieldMagic ||
        Style::currentStyle == Style::Styles::MagicSword || Style::currentStyle == Style::Styles::MagicFist ||
        Style::currentStyle == Style::Styles::MagicStaff || Style::currentStyle == Style::Styles::StaffSword)
        return true;
    return false;
}
void ActionPreInput(std::function<void()> func)
{
    uint32_t preInputTime = 100;
    while (true)
    {
        if ((TimeUtils::GetTime() - queue.time) / 1000.0 > preInputTime)
        {
            isInQueue = 0;
            break;
        }
        // Process NormalAttack
        if (isInQueue == 1 && Compatibility::CanNormalAttack())
        {
            startTime = TimeUtils::GetTime();
            func();
            isInQueue = 0;
            break;
        }
        // Process PowerAttack
        else if (isInQueue == 2 && Compatibility::CanPowerAttack())
        {
            startTime = TimeUtils::GetTime();
            func();
            isInQueue = 0;
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}
void NormalAttack(AttackType type)
{
    RE::TESIdleForm *target;
    switch (type)
    {
    case AttackType::Right:
        target = ActionList::NormalAttackRight;
        break;
    case AttackType::Left:
        target = ActionList::NormalAttackLeft;
        break;
    case AttackType::Dual:
        target = ActionList::NormalAttackDual;
        break;
    }
    if (Compatibility::BFCO)
    {
        if (PlayerStatus::IsSwiming())
            doAction(Compatibility::BFCO_NormalAttackSwim);
        else if ((PlayerStatus::IsSprinting() && PlayerStatus::IsMoving()) || PlayerStatus::IsJumping())
            VarUtils::player->NotifyAnimationGraph("attackStartSprint");
        if (KeyUtils::GetKeyState(Config::BFCO_SpecialAttackModifier))
        {
            if (!PlayerStatus::IsAttacking())
            {
                startTime = TimeUtils::GetTime();
                doAction(Compatibility::BFCO_NormalAttackSpecial);
            }
            else if (PlayerStatus::IsAttacking() && !Compatibility::CanNormalAttack())
            {
                queue.target = Compatibility::BFCO_NormalAttackSpecial;
                queue.time = TimeUtils::GetTime();
                if (!isInQueue)
                {
                    isInQueue = 1;
                    std::thread(ActionPreInput, []() { doAction(queue.target); }).detach();
                }
                else
                    isInQueue = 1;
            }
            return;
        }
    }
    else if (Compatibility::MCO)
    {
        if (PlayerStatus::IsSprinting() && PlayerStatus::IsMoving())
        {
            VarUtils::player->NotifyAnimationGraph("attackStartSprint");
            return;
        }
    }
    if (Compatibility::MCO || Compatibility::BFCO)
    {
        if (PlayerStatus::IsAttacking() && !Compatibility::CanNormalAttack())
        {
            if ((TimeUtils::GetTime() - startTime) / 1000.0 < 50.0)
                return;
            queue.target = target;
            queue.time = TimeUtils::GetTime();
            if (!isInQueue)
            {
                isInQueue = 1;
                std::thread(ActionPreInput, []() { doAction(queue.target); }).detach();
            }
            else
                isInQueue = 1;
            return;
        }
        else
        {
            startTime = TimeUtils::GetTime();
            doAction(target);
        }
    }
    else
        doAction(target);
}
void PowerAttack(AttackType type)
{
    RE::TESIdleForm *target;
    switch (type)
    {
    case AttackType::Right:
        target = ActionList::PowerAttackRight;
        break;
    case AttackType::Left:
        target = ActionList::PowerAttackLeft;
        break;
    case AttackType::Dual:
        target = ActionList::PowerAttackDual;
        break;
    }
    if (Compatibility::BFCO)
    {
        if (PlayerStatus::IsSwiming())
            doAction(Compatibility::BFCO_NormalAttackSwim);
        else if ((PlayerStatus::IsSprinting() && PlayerStatus::IsMoving()) || PlayerStatus::IsJumping())
            VarUtils::player->NotifyAnimationGraph("attackStartSprint");
        if (KeyUtils::GetKeyState(Config::BFCO_SpecialAttackModifier))
        {
            if (!PlayerStatus::IsAttacking())
            {
                startTime = TimeUtils::GetTime();
                doAction(Compatibility::BFCO_NormalAttackSpecial);
            }
            else if (PlayerStatus::IsAttacking() && !Compatibility::CanNormalAttack())
            {
                queue.target = Compatibility::BFCO_NormalAttackSpecial;
                queue.time = TimeUtils::GetTime();
                if (!isInQueue)
                {
                    isInQueue = 1;
                    std::thread(ActionPreInput, []() { doAction(queue.target); }).detach();
                }
                else
                    isInQueue = 1;
            }
            return;
        }
    }
    else if (Compatibility::MCO)
    {
        if (PlayerStatus::IsSprinting() && PlayerStatus::IsMoving())
            VarUtils::player->NotifyAnimationGraph("attackStartSprint");
    }

    if (Compatibility::MCO || Compatibility::BFCO)
    {
        if (PlayerStatus::IsAttacking() && !Compatibility::CanNormalAttack())
        {
            if ((TimeUtils::GetTime() - startTime) / 1000.0 < 50.0)
                return;
            queue.target = target;
            queue.time = TimeUtils::GetTime();
            if (!isInQueue)
            {
                isInQueue = 2;
                std::thread(ActionPreInput, []() { doAction(queue.target); }).detach();
            }
            else
                isInQueue = 2;
            return;
        }
        else
        {
            startTime = TimeUtils::GetTime();
            doAction(target);
        }
    }
    else
        doAction(target);
}
void Block()
{
    if (!PlayerStatus::IsBlocking() && !PlayerStatus::IsBashing() && PlayerStatus::IsAttackReady())
        SKSE::GetTaskInterface()->AddTask([]() {
            if (VarUtils::player->NotifyAnimationGraph("blockStart"))
                VarUtils::player->AsActorState()->actorState2.wantBlocking = true;
        });
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
        if (Custom::enableCustomInput && evn->IsDown())
            Custom::inputQueue.push_back(Custom::NewInput{code, TimeUtils::GetTime()});
        if (code == KeyUtils::KeyBoard::K && KeyUtils::GetKeyState(KeyUtils::KeyBoard::RightShift) && evn->IsDown())
            GUI::showSettings = !GUI::showSettings;
        if ((code == KeyUtils::Mouse::MouseWheelUp || code == KeyUtils::Mouse::MouseWheelDown) && evn->IsDown())
        {
            KeyUtils::MouseWheelTime = TimeUtils::GetTime();
            if (code == KeyUtils::Mouse::MouseWheelUp)
                KeyUtils::MouseWheelStatus = 1;
            else
                KeyUtils::MouseWheelStatus = -1;
            if (!KeyUtils::TracingMouse)
            {
                KeyUtils::TracingMouse = true;
                KeyUtils::TrackKeyState(0, []() {
                    if ((TimeUtils::GetTime() - KeyUtils::MouseWheelTime) / 1000.0 > 10)
                        KeyUtils::MouseWheelStatus = 0;
                    KeyUtils::TracingMouse = false;
                });
            }
        }
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
            else if (code == KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->readyWeapon))
                Stances::ChangeStanceTo(Stances::Stances::Sheathe);
            break;
        }
        while (code == Config::warAsh && Compatibility::ELDEN && Compatibility::CanUseWarAsh)
        {
            if (Config::warAshModifier)
                if (!KeyUtils::GetKeyState(Config::warAshModifier))
                    break;
            auto spells = RE::MagicFavorites::GetSingleton()->spells;
            for (auto spell = spells.begin(); spell != spells.end(); spell++)
                if (Compatibility::IsWarAsh(*spell))
                {
                    VarUtils::player->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant)
                        ->CastSpellImmediate((RE::MagicItem *)(*spell), true, nullptr, 0, false, 0, nullptr);
                    Compatibility::CanUseWarAsh = false;
                    std::thread([]() {
                        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
                        Compatibility::CanUseWarAsh = true;
                    }).detach();
                    break;
                }
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
        if (code == KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->rightAttack) ||
            code == KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->leftAttack))
            return false;
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
    if (CanDo() && Config::normalAttack && Config::powerAttack && Config::block)
    {
        if (VarUtils::player->IsBlocking() || PlayerStatus::IsBashing())
        {
            if (CanBash())
            {
                if (code == Config::normalAttack)
                {
                    a_event->userEvent = VarUtils::userEvent->rightAttack;
                    (this->*FnPB)(a_event, a_data);
                    VarUtils::player->NotifyAnimationGraph("bashRelease");
                    return true;
                }
                else if (code == Config::powerAttack)
                {
                    a_event->userEvent = VarUtils::userEvent->rightAttack;
                    return (this->*FnPB)(a_event, a_data);
                }
            }
            else
                return false;
        }
        if (code == Config::block)
            Block();
        // BFCO ComboAttack
        if (Compatibility::BFCO && code == Config::BFCO_ComboAttack && PlayerStatus::IsAttacking())
        {
            VarUtils::player->NotifyAnimationGraph("BFCOAttackStart_Comb");
            doAction(Compatibility::BFCO_ComboAttack);
            startTime = TimeUtils::GetTime();
        }

        if (code == Config::normalAttack)
        {
            if (Style::styleMap[Style::currentStyle].sheatheNormalAttack && !PlayerStatus::IsAttackReady())
            {
                a_event->userEvent = VarUtils::userEvent->rightAttack;
                return (this->*FnPB)(a_event, a_data);
            }
            if (Style::styleMap[Style::currentStyle].usingVanillaLogic ||
                KeyUtils::GetKeyState(Style::styleMap[Style::currentStyle].vanillaModifier))
            {
                if (Style::styleMap[Style::currentStyle].reverseAttack)
                    a_event->userEvent = VarUtils::userEvent->leftAttack;
                return (this->*FnPB)(a_event, a_data);
            }
            else
            {
                if (a_event->IsDown() && !Style::styleMap[Style::currentStyle].repeatNormalAttack)
                    NormalAttack(Style::styleMap[Style::currentStyle].normalAttackType);
                else if (Style::styleMap[Style::currentStyle].repeatNormalAttack)
                    NormalAttack(Style::styleMap[Style::currentStyle].normalAttackType);
                return true;
            }
        }
        else if (code == Config::powerAttack)
        {
            if (Style::styleMap[Style::currentStyle].sheathePowerAttack && !PlayerStatus::IsAttackReady())
            {
                a_event->userEvent = VarUtils::userEvent->rightAttack;
                return (this->*FnPB)(a_event, a_data);
            }
            if (Style::styleMap[Style::currentStyle].usingVanillaLogic ||
                KeyUtils::GetKeyState(Style::styleMap[Style::currentStyle].vanillaModifier))
            {
                if (Style::styleMap[Style::currentStyle].reverseAttack)
                    a_event->userEvent = VarUtils::userEvent->rightAttack;
                return (this->*FnPB)(a_event, a_data);
            }
            else
            {
                if (a_event->IsDown() && !Style::styleMap[Style::currentStyle].repeatNormalAttack)
                    PowerAttack(Style::styleMap[Style::currentStyle].powerAttackType);
                else if (Style::styleMap[Style::currentStyle].repeatNormalAttack)
                    PowerAttack(Style::styleMap[Style::currentStyle].powerAttackType);
                return true;
            }
        }
        return false;
    }
    if (PlayerStatus::IsRiding() && Config::enableReverseHorseAttack)
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
    if (Config::enableHoldSprint && PlayerStatus::IsSprinting())
        KeyUtils::TrackKeyState(code, []() {
            SKSE::GetTaskInterface()->AddTask([]() {
                if (PlayerStatus::IsSprinting())
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
    if (!PlayerStatus::IsSprinting())
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
                if (PlayerStatus::IsSprinting())
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
    if (Stances::enableStances)
        if (KeyUtils::GetKeyState(Stances::StancesModfier))
            return false;
    return (this->*FnCP)(a_event);
}
bool ReadyWeaponHandler::CP(InputEvent *a_event)
{
    return (that->*FnCP)(a_event);
}
bool ReadyWeaponHandler::ProcessButton(ButtonEvent *a_event, PlayerControlsData *a_data)
{
    if (PlayerStatus::IsAttacking())
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

//
// ToggleRunHandler
//
ToggleRunHandler *ToggleRunHandler::that;
ToggleRunHandler::FnCanProcess ToggleRunHandler::FnCP;
ToggleRunHandler::FnProcessButton ToggleRunHandler::FnPB;
bool ToggleRunHandler::CanProcess(InputEvent *a_event)
{
    return (this->*FnCP)(a_event);
}
bool ToggleRunHandler::CP(InputEvent *a_event)
{
    return (that->*FnCP)(a_event);
}
bool ToggleRunHandler::ProcessButton(ButtonEvent *a_event, PlayerControlsData *a_data)
{
    auto code = KeyUtils::GetEventKeyMap(a_event);
    if (Config::needModifier[code])
        if (!KeyUtils::GetKeyState(Config::needModifier[code]))
            return false;
    return (this->*FnPB)(a_event, a_data);
}
bool ToggleRunHandler::PB(ButtonEvent *a_event, PlayerControlsData *a_data)
{
    return (that->*FnPB)(a_event, a_data);
}
void ToggleRunHandler::Hook()
{
    REL::Relocation<uintptr_t> vtable{VTABLE_ToggleRunHandler[0]};
    that = stl::unrestricted_cast<ToggleRunHandler *>(vtable.address());
    FnCP = stl::unrestricted_cast<FnCanProcess>(vtable.write_vfunc(1, &ToggleRunHandler::CanProcess));
    FnPB = stl::unrestricted_cast<FnProcessButton>(vtable.write_vfunc(4, &ToggleRunHandler::ProcessButton));
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
    ToggleRunHandler::Hook();
    std::thread(Recover).detach();
}
} // namespace Hook
