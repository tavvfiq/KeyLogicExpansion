#include "keyDecoder.h"

namespace KeyDecoder
{
    using RawInput = Var::RawInput;
    using PressType = Var::PressType;
    using ActionType = Var::ActionType;

    typedef struct
    {
        uint32_t code;
        PressType press;
    } KeyInput;

    typedef struct
    {
        uint32_t withCombine;
        uint32_t chargeable;
        RE::BSFixedString userEvent;
        ActionType action;
    } Action;

    static std::shared_mutex raw_mtx;
    static std::deque<RawInput> rawQueue = {};
    static std::unordered_map<uint32_t, std::deque<RawInput> *> rawList;

    static std::shared_mutex key_mtx;
    static std::map<uint64_t, KeyInput> keyQueue = {};

    static std::deque<Action> actQueue = {};

    static std::unordered_map<uint32_t, bool> combineList;
    static std::unordered_map<uint32_t, std::map<uint32_t, Action, std::greater<uint32_t>>> searchList;

    bool isCombine(uint32_t code)
    {
        if (combineList.empty())
            return false;
        auto res = combineList.find(code);
        if (res == combineList.end())
            return false;
        return true;
    }

    void insertSearchList(Config::AltKeyMap altKey, RE::BSFixedString userEvent, ActionType aType)
    {
        if (!altKey.firstKey && !altKey.shortKey)
        {
            altKey.firstKey = KeyUtils::GetVanillaKeyMap(userEvent);
        }
        if (altKey.firstKey && altKey.secondKey)
        {
            auto res = searchList.find(altKey.secondKey);
            if (res == searchList.end())
            {
                if (!isCombine(altKey.firstKey))
                    combineList.insert(std::make_pair(altKey.firstKey, false));
                std::map<uint32_t, Action, std::greater<uint32_t>> tmp;
                tmp.insert(std::make_pair(altKey.priority, Action{altKey.firstKey, altKey.chargeable, userEvent, aType}));
                searchList.insert(std::make_pair(altKey.secondKey, tmp));
            }
            else
                res->second.insert(std::make_pair(altKey.priority, Action{altKey.firstKey, altKey.chargeable, userEvent, aType}));
        }
        if (altKey.shortKey || (altKey.firstKey && !altKey.secondKey))
        {
            if (altKey.shortKey)
            {
                auto res = searchList.find(altKey.shortKey);
                if (res == searchList.end())
                {
                    std::map<uint32_t, Action, std::greater<uint32_t>> tmp;
                    tmp.insert(std::make_pair(altKey.priority, Action{0, altKey.chargeable, userEvent, aType}));
                    searchList.insert(std::make_pair(altKey.shortKey, tmp));
                }
                else
                    res->second.insert(std::make_pair(altKey.priority, Action{0, altKey.chargeable, userEvent, aType}));
            }
            if (altKey.firstKey)
            {
                auto res = searchList.find(altKey.firstKey);
                if (res == searchList.end())
                {
                    std::map<uint32_t, Action, std::greater<uint32_t>> tmp;
                    tmp.insert(std::make_pair(altKey.priority, Action{0, altKey.chargeable, userEvent, aType}));
                    searchList.insert(std::make_pair(altKey.firstKey, tmp));
                }
                else
                    res->second.insert(std::make_pair(altKey.priority, Action{0, altKey.chargeable, userEvent, aType}));
            }
        }
    }

    void buildKeySearchList()
    {
        insertSearchList(Config::AltPauseMenu, Var::userEvent->pause, ActionType::MenuOpen);
        insertSearchList(Config::AltTweenMenu, Var::userEvent->tweenMenu, ActionType::MenuOpen);
        insertSearchList(Config::AltJournalMenu, Var::userEvent->journal, ActionType::MenuOpen);
        insertSearchList(Config::AltQuickInventory, Var::userEvent->quickInventory, ActionType::MenuOpen);
        insertSearchList(Config::AltQuickMagic, Var::userEvent->quickMagic, ActionType::MenuOpen);
        insertSearchList(Config::AltQuickStats, Var::userEvent->quickStats, ActionType::MenuOpen);
        insertSearchList(Config::AltQuickMap, Var::userEvent->quickMap, ActionType::MenuOpen);

        insertSearchList(Config::AltAttack, Var::userEvent->rightAttack, ActionType::Attack);
        insertSearchList(Config::AltPowerAttack, Var::userEvent->leftAttack, ActionType::Attack);
        insertSearchList(Config::AltBlock, Var::userEvent->blockStart, ActionType::Block);

        insertSearchList(Config::AltTogglePOV, Var::userEvent->togglePOV, ActionType::Misc);
        insertSearchList(Config::AltAutoMove, Var::userEvent->autoMove, ActionType::AutoMove);
        insertSearchList(Config::AltToggleRun, Var::userEvent->toggleRun, ActionType::Misc);

        combineList.insert(std::make_pair(0, true));

        for (auto item : searchList)
            for (auto i : item.second)
                logger::trace("Key: {} -> Priority: {} -> CombineKey: {} : UserEvent: {}", item.first, i.first, i.second.withCombine, i.second.userEvent.c_str());
    }

    void RawQueuePusher(RawInput raw) { rawQueue.push_back(raw); }

    void KeyTracker(std::unordered_map<uint32_t, std::deque<RawInput> *>::iterator iter_raw, std::map<uint64_t, KeyInput>::iterator iter_key, bool isCombine)
    {
        logger::trace("{} start", iter_raw->first);
        auto prevTime = TimeUtils::GetTime();
        while (true)
        {
            if ((TimeUtils::GetTime() - prevTime) / 1000.0 > Config::pressInterval)
            {
                if (isCombine)
                    combineList[iter_raw->first] = false;
                iter_key->second.press = PressType::release;
                break;
            }
            if (iter_raw->second->empty())
                continue;
            auto raw = iter_raw->second->front();
            iter_raw->second->pop_front();
            prevTime = TimeUtils::GetTime();
            if (!raw.value)
            {
                if (isCombine)
                    combineList[iter_raw->first] = false;
                iter_key->second.press = PressType::release;
                break;
            }
            if ((TimeUtils::GetTime() - iter_key->first) / 1000.0 > Config::clickTime)
            {
                if (isCombine)
                    combineList[iter_raw->first] = true;
                iter_key->second.press = PressType::click;
            }
            else if ((TimeUtils::GetTime() - iter_key->first) / 1000.0 > Config::longPressTime)
            {
                if (isCombine)
                    combineList[iter_raw->first] = true;
                iter_key->second.press = PressType::hold;
            }
        }
        //std::this_thread::sleep_for(std::chrono::milliseconds(200));
        logger::trace("{} end", iter_raw->first);
        raw_mtx.lock();
        key_mtx.lock();
        delete iter_raw->second;
        rawList.erase(iter_raw);
        keyQueue.erase(iter_key);
        key_mtx.unlock();
        raw_mtx.unlock();
    }

    void inputDecoder()
    {
        logger::trace("Input Decoder Start");
        while (true)
        {
            if (rawQueue.empty())
                continue;
            auto raw = rawQueue.front();
            rawQueue.pop_front();
            raw_mtx.lock_shared();
            auto res = rawList.find(raw.code);
            if (res == rawList.end())
            {
                auto ptr = new std::deque<RawInput>;
                ptr->push_back(raw);
                auto iter_raw = rawList.insert(std::make_pair(raw.code, ptr)).first;
                auto iter_key = keyQueue.insert(std::make_pair(TimeUtils::GetTime(), KeyInput{raw.code, PressType::disable})).first;
                std::thread(KeyTracker, iter_raw, iter_key, isCombine(raw.code)).detach();
            }
            else
                res->second->push_back(raw);
            raw_mtx.unlock_shared();
        }
    }

    void actionDistributor()
    {
        std::thread(inputDecoder).detach();
        logger::trace("Action Decoder Start");
        while (true)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
            key_mtx.lock_shared();
            if (keyQueue.empty())
            {
                key_mtx.unlock_shared();
                continue;
            }
            for (auto &key : keyQueue)
            {
                if (key.second.press == PressType::processed || key.second.press == PressType::release)
                    continue;
                auto res = searchList.find(key.second.code);
                if (res == searchList.end())
                    continue;
                for (auto &item : res->second)
                {
                    if (combineList[item.second.withCombine])
                    {
                        if (item.second.chargeable == 2)
                            break;
                        if (item.second.chargeable == 1)
                        {
                            item.second.chargeable = 2;
                            ActionDecoder::ChargeAction(item.second.userEvent, item.second.action, std::ref(key.second.press), std::ref(item.second.chargeable));
                            break;
                        }
                        if (key.second.press == PressType::click)
                            ActionDecoder::ClickAction(item.second.userEvent, item.second.action);
                        else if (key.second.press == PressType::hold)
                            ActionDecoder::HoldAction(item.second.userEvent, item.second.action);
                        key.second.press = PressType::processed;
                        break;
                    }
                }
            }
            key_mtx.unlock_shared();
        }
    }
}