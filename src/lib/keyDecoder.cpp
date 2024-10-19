#include "keyDecoder.h"

namespace KeyQueue
{

    enum ActionType
    {
        MenuOpen = 0,
        AttackBlock,
        AutoMove,
        Misc,

        End = 65536
    };

    enum PressType
    {
        release = 0,
        disable,
        click,
        hold
    };

    typedef struct
    {
        uint32_t code;
        PressType type;
    } KeyInput;

    typedef struct
    {
        uint32_t withCombine;
        RE::BSFixedString userEvent;
        ActionType type;
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
        logger::trace("Is Combine {}", code);
        return true;
    }

    void insertSearchList(Config::AltKeyMap altKey, RE::BSFixedString userEvent, ActionType aType)
    {
        if (altKey.firstKey && altKey.secondKey)
        {
            auto res = searchList.find(altKey.secondKey);
            if (res == searchList.end())
            {
                if (!isCombine(altKey.firstKey))
                    combineList.insert(std::make_pair(altKey.firstKey, false));
                std::map<uint32_t, Action, std::greater<uint32_t>> tmp;
                tmp.insert(std::make_pair(altKey.priority, Action{altKey.firstKey, userEvent, aType}));
                searchList.insert(std::make_pair(altKey.secondKey, tmp));
            }
            else
                res->second.insert(std::make_pair(altKey.priority, Action{altKey.firstKey, userEvent, aType}));
        }
        if ((altKey.useShortKey && altKey.shortKey) || (altKey.firstKey && !altKey.secondKey))
        {
            if (altKey.shortKey)
            {
                auto res = searchList.find(altKey.shortKey);
                if (res == searchList.end())
                {
                    std::map<uint32_t, Action, std::greater<uint32_t>> tmp;
                    tmp.insert(std::make_pair(altKey.priority, Action{0, userEvent, aType}));
                    searchList.insert(std::make_pair(altKey.shortKey, tmp));
                }
                else
                    res->second.insert(std::make_pair(altKey.priority, Action{0, userEvent, aType}));
            }
            if (altKey.firstKey)
            {
                auto res = searchList.find(altKey.firstKey);
                if (res == searchList.end())
                {
                    std::map<uint32_t, Action, std::greater<uint32_t>> tmp;
                    tmp.insert(std::make_pair(altKey.priority, Action{0, userEvent, aType}));
                    searchList.insert(std::make_pair(altKey.firstKey, tmp));
                }
                else
                    res->second.insert(std::make_pair(altKey.priority, Action{0, userEvent, aType}));
            }
        }
    }

    void buildKeySearchList()
    {
        insertSearchList(Config::AltTweenMenu, Var::userEvent->tweenMenu, ActionType::MenuOpen);
        insertSearchList(Config::AltQuickInventory, Var::userEvent->quickInventory, ActionType::MenuOpen);
        insertSearchList(Config::AltQuickMagic, Var::userEvent->quickMagic, ActionType::MenuOpen);
        insertSearchList(Config::AltQuickStats, Var::userEvent->quickStats, ActionType::MenuOpen);
        insertSearchList(Config::AltQuickMap, Var::userEvent->quickMap, ActionType::MenuOpen);
        insertSearchList(Config::AltAttack, Var::userEvent->attackStart, ActionType::AttackBlock);
        insertSearchList(Config::AltPowerAttack, Var::userEvent->attackPowerStart, ActionType::AttackBlock);
        insertSearchList(Config::AltBlock, Var::userEvent->blockStart, ActionType::AttackBlock);
        insertSearchList(Config::AltTogglePOV, Var::userEvent->togglePOV, ActionType::Misc);
        insertSearchList(Config::AltAutoMove, Var::userEvent->autoMove, ActionType::AutoMove);
        insertSearchList(Config::AltToggleRun, Var::userEvent->toggleRun, ActionType::Misc);

        for (auto item : searchList)
            for (auto i : item.second)
                logger::trace("Key: {} -> Priority: {} -> CombineKey: {} : UserEvent: {}", item.first, i.first, i.second.withCombine, i.second.userEvent.c_str());
    }
    /*
        void dumpKey(std::unique_lock<std::mutex> &&lock)
        {
            // detect key in keyQueue and sent to decoder
            for (auto index = keyQueue.begin(); index < keyQueue.end(); index++)
                for (auto node : searchList)
                    if (index->code == node.fisrt)
                    {
                        if (node.second == 0)
                            actQueue.push_back(node.action);
                        else
                            for (auto i = index + 1; i < keyQueue.end(); i++)
                                if (index->code == node.second)
                                    actQueue.push_back(node.action);
                    }
            for (auto index = keyQueue.begin(); index != keyQueue.end();)
                if (index->held > 0)
                    index = keyQueue.erase(index);
                else
                    index++;
            lock.unlock();
        }
    */

    void RawQueuePusher(RawInput raw)
    {
        rawQueue.push_back(raw);
    }

    void KeyTracker(std::unordered_map<uint32_t, std::deque<RawInput> *>::iterator iter_raw, std::map<uint64_t, KeyInput>::iterator iter_key, bool isCombine)
    {
        auto prevTime = TimeUtils::GetTime();
        logger::trace("Tracker of {} start", iter_raw->first);
        while (true)
        {
            if ((TimeUtils::GetTime() - prevTime) / 1000.0 > Config::pressInterval)
            {
                logger::trace("Tracker of {} outtime", iter_raw->first);
                if (isCombine)
                    combineList[iter_raw->first] = false;
                iter_key->second.type = PressType::release;
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
                iter_key->second.type = PressType::release;
                logger::trace("Tracker of {} release", iter_raw->first);
                break;
            }
            if ((TimeUtils::GetTime() - iter_key->first) / 1000.0 > Config::clickTime)
            {
                if (isCombine)
                    combineList[iter_raw->first] = true;
                iter_key->second.type = PressType::click;
            }
            else if ((TimeUtils::GetTime() - iter_key->first) / 1000.0 > Config::longPressTime)
            {
                if (isCombine)
                    combineList[iter_raw->first] = true;
                iter_key->second.type = PressType::hold;
            }
        }
        logger::trace("Tracker of {} end", iter_raw->first);
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
                logger::trace("New Start");
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

    void doAction(BSFixedString userEvent, ActionType type) {

    }

    void actionDecoder()
    {
        std::thread(inputDecoder).detach();
        logger::trace("Action Decoder Start");
        while (true)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
            key_mtx.lock_shared();
            if (keyQueue.empty())
                continue;
            for (auto key : keyQueue) {
                auto res = searchList.find(key.second.code);
                if (res == searchList.end())
                    continue;
                for (auto item : res->second) {
                    if (combineList[item.second.withCombine])
                        doAction(item.second.userEvent, item.second.type);
                }
            }
            key_mtx.unlock_shared();
        }
    }
}