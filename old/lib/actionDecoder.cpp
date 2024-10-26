#include "actionDecoder.h"

using namespace RE;
using namespace REL;

namespace ActionDecoder
{
    using PressType = Var::PressType;
    using ActionType = Var::ActionType;

    bool isInMenu()
    {
        if (Var::ui->numPausesGame > 0)
            return true;
        if (Var::ui->IsMenuOpen("Dialogue Menu"))
            return true;
        return false;
    }

    bool isBow()
    {
        auto lHand = Var::player->GetActorRuntimeData().currentProcess->GetEquippedLeftHand();
        if (lHand)
            return lHand->As<RE::TESObjectWEAP>()->GetWeaponType() == RE::WEAPON_TYPE::kBow;
        return false;
    }

    bool isCrossbow()
    {
        auto lHand = Var::player->GetActorRuntimeData().currentProcess->GetEquippedLeftHand();
        if (lHand)
            return lHand->As<RE::TESObjectWEAP>()->GetWeaponType() == RE::WEAPON_TYPE::kCrossbow;
        return false;
    }

    uint32_t isStaff()
    {
        return 0;
    }

    bool isDualWeapen()
    {
        return true;
    }

    typedef bool doAction_t(RE::TESActionData *);
    REL::Relocation<doAction_t> doAction{RELOCATION_ID(40551, 41557)};

    void NormalAttack(PressType &press, uint32_t &chargeable)
    {
        logger::trace("NormalAttack");
        BGSAction *RightAttack = (BGSAction *)TESForm::LookupByID(0x13005);
        BGSAction *LeftAttack = (BGSAction *)TESForm::LookupByID(0x13004);
        BGSAction *DualAttack = (BGSAction *)TESForm::LookupByID(0x50C96);
        BGSAction *BlockHit = (BGSAction *)TESForm::LookupByID(0x13AF4);
        BGSAction *action = RightAttack;
        if (Var::player->IsBlocking())
            action = BlockHit;
        while (press != PressType::release)
        {
            SKSE::GetTaskInterface()->AddTask([action]()
                                              { 
            std::unique_ptr<TESActionData> data(TESActionData::Create());
            data->source = NiPointer<TESObjectREFR>(Var::player);
            data->action = action;
            doAction(data.get()); });
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
            if (action == BlockHit)
                break;
        }
        chargeable = 1;
    }

    void PowerAttack(PressType &press, uint32_t &chargeable)
    {
        logger::trace("PowerAttack");
        BGSAction *RightPowerAttack = (BGSAction *)TESForm::LookupByID(0x13383);
        BGSAction *LeftPowerAttack = (BGSAction *)TESForm::LookupByID(0x2E2F6);
        BGSAction *DualPowerAttack = (BGSAction *)TESForm::LookupByID(0x2E2F7);
        BGSAction *action = RightPowerAttack;
        while (press != PressType::release)
        {
            SKSE::GetTaskInterface()->AddTask([action]()
                                              { 
            std::unique_ptr<TESActionData> data(TESActionData::Create());
            data->source = NiPointer<TESObjectREFR>(Var::player);
            data->action = action;
            doAction(data.get()); });
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
        SKSE::GetTaskInterface()->AddTask([]()
                                          { Var::player->NotifyAnimationGraph("attackRelease"); });
        chargeable = 1;
    }

    void Block(PressType &press, uint32_t &chargeable)
    {
        while (press != PressType::release)
        {
            if (!Var::player->IsBlocking())
                SKSE::GetTaskInterface()->AddTask([]()
                                                  { Var::player->NotifyAnimationGraph(Var::userEvent->blockStart); });
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
        SKSE::GetTaskInterface()->AddTask([]()
                                          { Var::player->NotifyAnimationGraph(Var::userEvent->blockStop); });
        chargeable = 1;

        //  Var::pCtrl->data.moveInputVec = NiPoint2(1.0, 1.0);
    }

    void BowAttack(PressType &press, uint32_t &chargeable)
    {
        logger::trace("BowAttack");
        SKSE::GetTaskInterface()->AddTask([]() { 
            Var::player->NotifyAnimationGraph("bowAttackStart");
            //Var::player->NotifyAnimationGraph("bowDrawStart");
        });

        while (press != PressType::release)
        {
            SKSE::GetTaskInterface()->AddTask([]()
                                              { Var::player->NotifyAnimationGraph("bowDrawn"); });
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
        SKSE::GetTaskInterface()->AddTask([]()
                                          { Var::player->NotifyAnimationGraph("BowRelease"); });
        chargeable = 1;
        logger::trace("BowAttack");
    }

    void BowAim(PressType &press, uint32_t &chargeable)
    {
        logger::trace("BowAim");
        SKSE::GetTaskInterface()->AddTask([]()
                                          { Var::player->NotifyAnimationGraph("bowZoomStart"); });
        while (press != PressType::release)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
        SKSE::GetTaskInterface()->AddTask([]()
                                          { Var::player->NotifyAnimationGraph("bowZoomEnd"); });
        chargeable = 1;
    }

    void ClickAction(BSFixedString userEvent, ActionType type)
    {
        logger::trace("UserEvent: {}     Type: {}", userEvent.c_str(), (int)type);
        switch (type)
        {
        case ActionType::MenuOpen:
            if (!isInMenu())
                Hook::MenuOpenHandler::PB(RE::ButtonEvent::Create(INPUT_DEVICE::kKeyboard, userEvent, 0, 1, 0));
            break;
        }
    }

    void HoldAction(BSFixedString userEvent, ActionType type)
    {
    }
    void ChargeAction(BSFixedString userEvent, ActionType type, PressType &press, uint32_t &chargeable)
    {
        if (isInMenu())
            return;
        logger::trace("UserEvent: {}     Type: {}", userEvent.c_str(), (int)type);
        switch (type)
        {
        case ActionType::Attack:
            if (userEvent == Var::userEvent->rightAttack)
                if (isBow())
                    std::thread(BowAttack, std::ref(press), std::ref(chargeable)).detach();
                else
                    std::thread(NormalAttack, std::ref(press), std::ref(chargeable)).detach();
            else if (userEvent == Var::userEvent->leftAttack)
                if (isBow())
                    std::thread(BowAim, std::ref(press), std::ref(chargeable)).detach();
                else
                    std::thread(PowerAttack, std::ref(press), std::ref(chargeable)).detach();
            break;
        case ActionType::Block:
            std::thread(Block, std::ref(press), std::ref(chargeable)).detach();
        default:
            break;
        }
    }
}