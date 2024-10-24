#include "actionDecoder.h"

using namespace RE;
using namespace REL;

namespace ActionDecoder
{

    typedef bool doAnimation_t(RE::TESActionData *);
    REL::Relocation<doAnimation_t> doAnimation{RELOCATION_ID(40551, 41557)};

    void Attack(BSFixedString userEvent)
    {
        auto action = userEvent;
        if (userEvent == Var::userEvent->rightAttack)
            action = Var::userEvent->attackStart;
        else if (userEvent == Var::userEvent->leftAttack)
            action = Var::userEvent->blockStart;
        else if (userEvent == Var::userEvent->attackPowerStart)
            action = "attackPowerStartH2HCombo";
        SKSE::GetTaskInterface()->AddTask([action]()
                                          { Var::player->NotifyAnimationGraph(action); });

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
        case ActionType::AttackBlock:
            Attack(userEvent);
            break;
        }
    }
}