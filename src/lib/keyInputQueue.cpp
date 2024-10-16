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

    enum PressType {
        click = 0,
        hold,
        release
    };

    typedef struct
    {
        uint32_t code;
        PressType type;
    } keyInput;
    
    typedef struct
    {
        RE::BSFixedString userEvent;
        ActionType type;
    } Action;

    typedef struct
    {
        uint32_t fisrt;
        uint32_t second;
        uint32_t priority;
        Action action;
    } SearchNode;


    std::deque<RawInput> rawQueue = {};
    static std::mutex mtx;
    static std::deque<keyInput> keyQueue;
    static std::deque<Action> actQueue = {};
    static std::vector<SearchNode> searchList;

    void insertSearchList(Config::AltKeyMap altKey, RE::BSFixedString userEvent, ActionType aType)
    {
        if (altKey.firstKey)
            searchList.push_back(SearchNode{altKey.firstKey, altKey.secondKey, altKey.priority, Action{userEvent, aType}});
        if (altKey.useShortKey && altKey.shortKey)
            searchList.push_back(SearchNode{altKey.shortKey, 0, altKey.priority, Action{userEvent, aType}});
    }

    void buildKeySearchList()
    {
        if (Config::enableAltTweenMenu)
            insertSearchList(Config::AltTweenMenu, Var::userEvent->tweenMenu, ActionType::MenuOpen);
        if (Config::enableAltQuickInventory)
            insertSearchList(Config::AltQuickInventory, Var::userEvent->quickInventory, ActionType::MenuOpen);
        if (Config::enableAltQuickMagic)
            insertSearchList(Config::AltQuickMagic, Var::userEvent->quickMagic, ActionType::MenuOpen);
        if (Config::enableAltQuickStats)
            insertSearchList(Config::AltQuickStats, Var::userEvent->quickStats, ActionType::MenuOpen);
        if (Config::enableAltQuickMap)
            insertSearchList(Config::AltQuickMap, Var::userEvent->quickMap, ActionType::MenuOpen);
        if (Config::enableAltAttack)
            insertSearchList(Config::AltAttack, Var::userEvent->attackStart, ActionType::AttackBlock);
        if (Config::enableAltPowerAttack)
            insertSearchList(Config::AltPowerAttack, Var::userEvent->attackPowerStart, ActionType::AttackBlock);
        if (Config::enableAltBlock)
            insertSearchList(Config::AltBlock, Var::userEvent->blockStart, ActionType::AttackBlock);
        if (Config::enableAltTogglePOV)
            insertSearchList(Config::AltTogglePOV, Var::userEvent->togglePOV, ActionType::Misc);
        if (Config::enableAltAutoMove)
            insertSearchList(Config::AltAutoMove, Var::userEvent->autoMove, ActionType::AutoMove);
        if (Config::enableAltToggleRun)
            insertSearchList(Config::AltToggleRun, Var::userEvent->toggleRun, ActionType::Misc);

        std::sort(searchList.begin(), searchList.end(), [](SearchNode &a, SearchNode &b)
                  { return a.priority > b.priority; });
    }

    void pushInKey(keyInput key)
    {
        std::unique_lock<std::mutex> lock(mtx);
        keyQueue.push_back(key);
        dumpKey(std::move(lock));
    }

    void updateKey(keyInput key)
    {
        if (keyQueue.empty())
            return;

        std::unique_lock<std::mutex> lock(mtx);
        auto index = std::find_if(keyQueue.begin(), keyQueue.end(), [key](keyInput ki)
                                  { return ki.code == key.code && ki.held == 0; });

        if (index == keyQueue.end())
            return;
        else
            index->held = key.held;
        dumpKey(std::move(lock));
    }

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

    void PressTypeDetect(keyInput& key) {
    }

    void CombineDetect() {

    }

    void inputDecoder() {
        while(true) {
            if (rawQueue.empty())
                continue;
            auto raw = rawQueue.front();
            rawQueue.pop_front();
            {
                if (!raw.held && raw.value) {
                    keyQueue.push_back(keyInput{raw.code, PressType::click});
                    std::thread(PressTypeDetect, keyQueue.back()).detach();
                }
            }
        }
    }

    void actionDecoder()
    {
        logger::trace("Action Decoder Start");
        while (true)
        {
            if (actQueue.empty())
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(20));
                continue;
            }
            Action act = actQueue.front();
            actQueue.pop_front();
            logger::trace("String: {}   Type: {}", act.userEvent.c_str(), (int)act.type);
            if (act.type == ActionType::End)
                break;
            switch (act.type) {
                case ActionType::MenuOpen:
                    break;
                case ActionType::AttackBlock:
                    break;
                case ActionType::AutoMove:
                    break;
                default:
                    break;
            }
        }
    }
}