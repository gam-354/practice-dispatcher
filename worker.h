#ifndef WORKER_H
#define WORKER_H

#include <thread>
#include <vector>
#include <memory>

#include "Result.h"
#include "Operation.h"
#include "SafeQueue.h"
#include "SmartThread.h"

class Worker
{
    public:

        Worker(int numThreads);
        ~Worker();

        //Result stopOneThread(uint32_t timeout_us);
        //Result addOneThread();

        void addOperation(const Operation & op);

        void executeOperation(Operation & op);

        void waitUntilFinished();

    private:

        std::vector<std::unique_ptr<SmartThread>> threads_;

        std::thread * dispatcher_;

        SafeQueue<Operation> pendingOperations_;

        //const uint32_t MAX_NUM_THREADS = 10;

        bool stop_ {false};

        void dispatch();


};

#endif