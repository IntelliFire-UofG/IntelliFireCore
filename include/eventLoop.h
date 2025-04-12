#ifndef EVENT_LOOP_H
#define EVENT_LOOP_H

#include <atomic>
#include <functional>
#include <vector>
#include <mutex>

class EventLoop{
    public:
        EventLoop();
        ~EventLoop();
        void run();
        void stop();
        int registerCallback(int eventType, std::function<void()> callback);
        bool removeCallback(int callbackId);

    private:
        std::atomic<bool> running;
        std::atomic<int> nextCallbackId;

        struct CallbackInfo {
            int id;
            int eventType;
            std::function<void()> callback;

        };

        std::vector<CallbackInfo> callbacks;
        std::mutex callbacksMutex;

        void processEvents();
};

#endif
