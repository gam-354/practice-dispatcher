#ifndef SAFEQUEUE_H
#define SAFEQUEUE_H

#include <queue>
#include <condition_variable>
#include <mutex>

template <typename T> class SafeQueue
{
    public:
        SafeQueue() {}

        void push(const T & elem)
        {
            std::unique_lock lock(mutex_);

            internalQueue_.push(elem);

            // If no longer empty, notify
            if (internalQueue_.size() == 1)
            {
                cond_.notify_all();
            }
        }

        T receive()
        {
            std::unique_lock lock(mutex_);

            // Wait until not empty
            while(internalQueue_.empty())
            {
                cond_.wait(lock);
            }

            T next = internalQueue_.front();
            internalQueue_.pop();
            return next;
        }

        size_t howManyWaiting()
        {
            return internalQueue_.size();
        }

    private:

        std::queue<T> internalQueue_;
        std::mutex mutex_;
        std::condition_variable cond_;

};

#endif