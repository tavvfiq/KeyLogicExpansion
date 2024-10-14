#include "keyInputQueue.h"

namespace keyQueue
{

    std::deque<keyInput> keyQueue = {};

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
        {
            index->held = key.held;
        }
        dumpKey(std::move(lock));
    }

    void dumpKey(std::unique_lock<std::mutex> &&lock)
    {
        // detect some key or combined key in keyQueue and store it
        for (int i = 0; i < keyQueue.size(); i++)
            logger::trace("Dump - code: {}   held: {}", keyQueue[i].code, keyQueue[i].held);
        for (auto index = keyQueue.begin(); index != keyQueue.end();)
            if (index->held > 0)
                index = keyQueue.erase(index);
            else
                index++;
        lock.unlock();
        // do some action
    }
}