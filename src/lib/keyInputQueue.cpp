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

    std::deque<RawInput> rawQueue = {};
    static std::mutex raw_mtx;
    static std::vector<uint32_t> combineList = {};
    static std::deque<keyInput> combineQueue = {};
    static std::deque<keyInput> keyQueue = {};
    static std::deque<Action> actQueue = {};
    static std::unordered_map<uint32_t, std::map<uint32_t, Action>> searchList;

    void insertSearchList(Config::AltKeyMap altKey, RE::BSFixedString userEvent, ActionType aType)
    {
        if (altKey.firstKey && altKey.secondKey)
        {
            combineList.push_back(altKey.firstKey);
            std::map<uint32_t, Action> tmp;
            tmp.insert(std::make_pair(altKey.priority, Action{altKey.firstKey, userEvent, aType}));
            searchList.insert(std::make_pair(altKey.secondKey, tmp));
        }
        if ((altKey.useShortKey && altKey.shortKey) || (altKey.firstKey && !altKey.secondKey))
        {
            std::map<uint32_t, Action> tmp;
            tmp.insert(std::make_pair(altKey.priority, Action{0, userEvent, aType}));
            searchList.insert(std::make_pair(altKey.firstKey, tmp));
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

        std::sort(combineList.begin(), combineList.end());
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

    void inputCleaner()
    {
        while (true)
        {
            raw_mtx.lock();
            if (rawQueue.empty())
                continue;
            if (raw.code == rawQueue.front().code && raw.held == rawQueue.front().held && raw.value == rawQueue.front().value)
            {
                if (((std::chrono::duration<double, std::milli>)(std::chrono::high_resolution_clock::now() - prevTime)).count() > Config::deleteTime)
                {
                    for (auto item = rawQueue.begin(); item < rawQueue.end();)
                    {
                        if (item->code == raw.code && item->held >= raw.held)
                            rawQueue.erase(item);
                        else
                            break;
                    }
                }
                else
                    prevTime = std::chrono::high_resolution_clock::now();
            }
            raw = rawQueue.front();
            raw_mtx.unlock();
        }
    }
    */

    void KeyTracker(keyInput &key)
    {
        auto pressTime = std::chrono::high_resolution_clock::now();
        auto prevTime = std::chrono::high_resolution_clock::now();
        logger::trace("Tracker of {} start", key.code);
        while (true)
        {
            if (((std::chrono::duration<double, std::milli>)(std::chrono::high_resolution_clock::now() - prevTime)).count() > Config::deleteTime)
            {
                logger::trace("Tracker of {} outtime", key.code);
                key.type = PressType::release;
                break;
            }
            raw_mtx.lock();
            if (rawQueue.empty())
            {
                raw_mtx.unlock();
                continue;
            }
            auto raw = rawQueue.front();
            if (raw.code != key.code)
            {
                raw_mtx.unlock();
                continue;
            }
            else
            {
                rawQueue.pop_front();
                raw_mtx.unlock();
            }
            prevTime = std::chrono::high_resolution_clock::now();
            if (!raw.value)
            {
                key.type = PressType::release;
                logger::trace("Tracker of {} release", key.code);
                break;
            }
            if (((std::chrono::duration<double, std::milli>)(std::chrono::high_resolution_clock::now() - pressTime)).count() > Config::longPressTime)
                key.type = PressType::hold;
        }
        logger::trace("Tracker of {} end", key.code);
    }

    bool isCombine(uint32_t code)
    {
        if (combineList.empty())
            return false;
        return std::binary_search(combineList.begin(), combineList.end(), code);
    }

    bool isNewInput(uint32_t code)
    {
        if (isCombine(code))
        {
            if (combineQueue.empty())
                return true;
            else
            {
                auto res = std::find_if(combineQueue.begin(), combineQueue.end(), [code](keyInput &key)
                                        { return code == key.code; });
                if (res == combineQueue.end())
                    return true;
                else
                    return false;
            }
        }
        if (keyQueue.empty())
            return true;
        else
        {
            auto res = std::find_if(keyQueue.begin(), keyQueue.end(), [code](keyInput &key)
                                    { return code == key.code; });
            if (res == keyQueue.end())
                return true;
            else
                return false;
        }
    }

    void inputDecoder()
    {
        std::thread(actionDecoder).detach();
        while (true)
        {
            raw_mtx.lock();
            if (rawQueue.empty())
            {
                raw_mtx.unlock();
                continue;
            }
            auto raw = rawQueue.front();
            if (isNewInput(raw.code))
            {
                rawQueue.pop_front();
                raw_mtx.unlock();
                if (isCombine(raw.code))
                {
                    combineQueue.push_back(keyInput{raw.code, PressType::click});
                    std::thread(KeyTracker, std::ref(combineQueue.back())).detach();
                }
                else
                {
                    keyQueue.push_back(keyInput{raw.code, PressType::click});
                    std::thread(KeyTracker, std::ref(keyQueue.back())).detach();
                }
            }
            else
                raw_mtx.unlock();
        }
    }

    void actionDecoder()
    {
        logger::trace("Action Decoder Start");
        while (true)
        {
            if (keyQueue.empty())
                continue;
        }
    }
}