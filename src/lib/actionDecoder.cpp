#include "actionDecoder.h"

using namespace RE;
using namespace REL;

namespace ActionDecoder
{
    using PressType = Var::PressType;
    using ActionType = Var::ActionType;

    typedef bool doAction_t(RE::TESActionData *);
    REL::Relocation<doAction_t> doAction{RELOCATION_ID(40551, 41557)};

    void NormalAttack(PressType &press, uint32_t &chargeable)
    {
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
        //  Var::pCtrl->data.moveInputVec = NiPoint2(1.0, 1.0);
    }

    void PowerAttack(PressType &press, uint32_t &chargeable)
    {
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
        chargeable = 1;

        //  Var::pCtrl->data.moveInputVec = NiPoint2(1.0, 1.0);
    }

    void Block(PressType &press, uint32_t &chargeable)
    {
        while (press != PressType::release)
        {
            SKSE::GetTaskInterface()->AddTask([]()
                                              { Var::player->NotifyAnimationGraph(Var::userEvent->blockStart); });
        }
        SKSE::GetTaskInterface()->AddTask([]()
                                          { Var::player->NotifyAnimationGraph(Var::userEvent->blockStop); });
        chargeable = 1;

        //  Var::pCtrl->data.moveInputVec = NiPoint2(1.0, 1.0);
    }

    void ClickAction(BSFixedString userEvent, ActionType type)
    {
        logger::trace("UserEvent: {}     Type: {}", userEvent.c_str(), (int)type);
        switch (type)
        {
        case ActionType::MenuOpen:
            Hook::MenuOpenHandler::PB(RE::ButtonEvent::Create(INPUT_DEVICE::kKeyboard, userEvent, 0, 1, 0));
            break;
        }
    }

    void HoldAction(BSFixedString userEvent, ActionType type)
    {
    }
    void ChargeAction(BSFixedString userEvent, ActionType type, PressType &press, uint32_t &chargeable)
    {
        switch (type)
        {
        case ActionType::Attack:
            if (userEvent == Var::userEvent->rightAttack)
                std::thread(NormalAttack, std::ref(press), std::ref(chargeable)).detach();
            else if (userEvent == Var::userEvent->attackPowerStart)
                std::thread(PowerAttack, std::ref(press), std::ref(chargeable)).detach();
            break;
        case ActionType::Block:
            std::thread(Block, std::ref(press), std::ref(chargeable)).detach();
        default:
            break;
        }
    }
}