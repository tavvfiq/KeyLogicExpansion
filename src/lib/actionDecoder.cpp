#include "actionDecoder.h"

using namespace RE;
using namespace REL;

namespace ActionDecoder
{
    void ClickAction(BSFixedString userEvent, ActionType type)
    {
        logger::trace("UserEvent: {}     Type: {}", userEvent.c_str(), (int)type);
        switch (type)
        {
        case ActionType::MenuOpen:
            Hook::MenuOpenHandler::PB(RE::ButtonEvent::Create(INPUT_DEVICE::kKeyboard, userEvent, 0, 1, 0));
            break;
        case ActionType::AttackBlock:
            //if (userEvent == Var::userEvent->rightAttack)
                //Hook::AttackBlockHandler::PB(RE::ButtonEvent::Create(INPUT_DEVICE::kMouse, userEvent, 0, 256, 0), &Var::pCtrl->data);
            // else if (userEvent == Var::userEvent->attackPowerStart)
            // Hook::AttackBlockHandler::PB(RE::ButtonEvent::Create(INPUT_DEVICE::kMouse, userEvent, 0, 256, 1), &Var::pCtrl->data);
            break;
        }
    }
}