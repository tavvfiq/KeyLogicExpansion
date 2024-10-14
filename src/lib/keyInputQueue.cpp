#include "keyInputQueue.h"

namespace keyQueue {

    std::deque<keyInput> keyQueue = {};

    void pushInKey(keyInput key) {
        std::unique_lock<std::mutex> lock(mtx);
        keyQueue.push_back(key);
        dumpKey(std::move(lock));
    }

    void updateKey(keyInput key) {
        if (keyQueue.empty())
            return;

        std::unique_lock<std::mutex> lock(mtx);
        logger::trace("Try update code: {}    held to {}",key.code, key.held);
        auto index = std::find_if(keyQueue.begin(), keyQueue.end(), [key](keyInput ki) {
            return ki.code == key.code && ki.held == 0;
        });

        if (index == keyQueue.end())
            return;
        else {
            logger::trace("Update code: {}    held to {}",key.code, key.held);
            keyQueue[std::distance(keyQueue.begin(), index)].held = key.held;
        }
        dumpKey(std::move(lock));
    }

    void dumpKey(std::unique_lock<std::mutex>&& lock) {
        // detect some key or combined key in keyQueue and store it
        for (int i = 0;i < keyQueue.size();i++)
            logger::trace("Dump - code: {}   held: {}", keyQueue[i].code, keyQueue[i].held);
        if (keyQueue.size() > 10)
            keyQueue.clear();
        lock.unlock();
        // do some action 
    }
}