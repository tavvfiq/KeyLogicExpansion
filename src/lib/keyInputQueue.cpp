#include "keyInputQueue.h"

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
    } keyInput;

    typedef struct
    {
        uint32_t withCombine;
        RE::BSFixedString userEvent;
        ActionType type;
    } Action;

    static std::shared_mutex raw_mtx;
    static std::deque<RawInput> rawQueue = {};

    static std::shared_mutex key_mtx;
    static std::deque<keyInput> keyQueue = {};

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
        raw_mtx.lock();
        rawQueue.push_front(raw);
        raw_mtx.unlock();
    }

    void KeyTracker(keyInput &key, bool isCombine)
    {
        auto pressTime = std::chrono::high_resolution_clock::now();
        auto prevTime = std::chrono::high_resolution_clock::now();
        logger::trace("Tracker of {} start", key.code);
        while (true)
        {
            if (((std::chrono::duration<double, std::milli>)(std::chrono::high_resolution_clock::now() - prevTime)).count() > Config::pressInterval)
            {
                logger::trace("Tracker of {} outtime", key.code);
                if (isCombine)
                    combineList[key.code] = false;
                key.type = PressType::release;
                break;
            }
            raw_mtx.lock_shared();
            if (rawQueue.empty())
            {
                raw_mtx.unlock_shared();
                continue;
            }
            auto raw = rawQueue.front();
            raw_mtx.unlock_shared();
            if (raw.code != key.code)
                continue;
            else
            {
                raw_mtx.lock();
                if (!rawQueue.empty())
                    rawQueue.pop_front();
                raw_mtx.unlock();
            }
            prevTime = std::chrono::high_resolution_clock::now();
            if (!raw.value)
            {
                if (isCombine)
                    combineList[key.code] = false;
                key.type = PressType::release;
                logger::trace("Tracker of {} release", key.code);
                break;
            }
            if (((std::chrono::duration<double, std::milli>)(std::chrono::high_resolution_clock::now() - pressTime)).count() > Config::clickTime)
            {
                if (isCombine)
                    combineList[key.code] = true;
                key.type = PressType::click;
            }
            else if (((std::chrono::duration<double, std::milli>)(std::chrono::high_resolution_clock::now() - pressTime)).count() > Config::longPressTime)
            {
                if (isCombine)
                    combineList[key.code] = true;
                key.type = PressType::hold;
            }
        }
        logger::trace("Tracker of {} end", key.code);
    }

    bool isNewInput(uint32_t code)
    {
        key_mtx.lock_shared();
        if (keyQueue.empty())
        {
            key_mtx.unlock_shared();
            return true;
        }
        auto res = std::find_if(keyQueue.begin(), keyQueue.end(), [code](keyInput &key)
                                { return code == key.code; });
        if (res == keyQueue.end())
        {
            key_mtx.unlock_shared();
            return true;
        }
        key_mtx.unlock_shared();
        return false;
    }

    void inputDecoder()
    {
        logger::trace("Input Decoder Start");
        while (true)
        {
            raw_mtx.lock_shared();
            if (rawQueue.empty())
            {
                raw_mtx.unlock_shared();
                continue;
            }
            auto raw = rawQueue.front();
            raw_mtx.unlock_shared();
            if (isNewInput(raw.code))
            {
                logger::trace("New Input {}", raw.code);
                raw_mtx.lock();
                if (!rawQueue.empty())
                    rawQueue.pop_front();
                raw_mtx.unlock();
                key_mtx.lock();
                keyQueue.push_back(keyInput{raw.code, PressType::disable});
                std::thread(KeyTracker, std::ref(keyQueue.back()), isCombine(raw.code)).detach();
                key_mtx.unlock();
            }
        }
    }

    void actionDecoder()
    {
        std::thread(inputDecoder).detach();
        logger::trace("Action Decoder Start");
        while (true)
        {
            if (keyQueue.empty())
                continue;
            key_mtx.lock();
            keyQueue.erase(std::remove_if(keyQueue.begin(), keyQueue.end(), [](keyInput &key)
                                          { return key.type == PressType::release; }),
                           keyQueue.end());
            key_mtx.unlock();
            key_mtx.lock_shared();
            for (auto item : keyQueue)
                logger::trace("KeyQueue List : {}", item.code);
            key_mtx.unlock_shared();
        }
    }
}