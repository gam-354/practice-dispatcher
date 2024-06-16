#ifndef SMARTTHREAD_H
#define SMARTTHREAD_H

#include <thread>
#include <atomic>
#include <cstdlib>

#include "Operation.h"



class SmartThread
{
    public:

        SmartThread()
        {
            innerThread_ = new std::thread(&SmartThread::threadCode, this);
        }

        ~SmartThread()
        {
            innerThread_->join();
            delete innerThread_;
        }

        const bool isIdle()
        {
            return idle_.load();
        }

        void work(Operation op)
        {
            idle_ = false;
            currentOp_ = op;
            idle_.notify_one();
        }

    private:

        std::atomic<bool> idle_ {true};
        std::atomic<bool> stop_ {false};
        Operation currentOp_;
        std::thread * innerThread_ = nullptr;

        void threadCode()
        {
            printf("[%u] thread created \n", innerThread_->get_id());

            while(!stop_)
            {
                // Wait until no longer idle
                idle_.wait(true);

                printf("[%u] executing operation %u... \n", innerThread_->get_id(), currentOp_.id_);
                currentOp_.execute();

                // Calculate random wait
                std::srand(std::time(nullptr));
                std::this_thread::sleep_for(std::chrono::milliseconds(std::rand()%1000));

                // Notify the requestor
                currentOp_.callbacker_->onOperationComplete(currentOp_);

                idle_ = true;
            }

            printf("[%u] thread exit \n", innerThread_->get_id());
        }
};

#endif