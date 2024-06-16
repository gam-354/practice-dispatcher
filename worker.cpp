
#include "worker.h"
#include <chrono>

Worker::Worker(int numThreads)
{
    dispatcher_ = new std::thread(&Worker::dispatch, this);

    for(uint8_t i = 0; i<numThreads; i++)
    {
        threads_.push_back(std::make_unique<SmartThread>());
    }
}

Worker::~Worker()
{
    stop_ = true;
    dispatcher_->join();
    delete dispatcher_;
}

void Worker::addOperation(const Operation & op)
{
    pendingOperations_.push(op);
    printf("Adding operation %u to queue. Current queue size: %lu\n", op.id_, pendingOperations_.howManyWaiting());
}

void Worker::executeOperation(Operation & op)
{
    printf("Executing operation (%u) immediately\n", op.id_);
    op.execute();
}

void Worker::waitUntilFinished()
{
    // First wait until the queue is empty
    while(pendingOperations_.howManyWaiting());

    // Now wait until all threads are idle
    bool allIdle = false;
    while(!allIdle)
    {
        allIdle = true;

        for(std::unique_ptr<SmartThread> & thread : threads_)
        {
            if(!thread->isIdle())
            {
                allIdle = false;
                break;
            }
        }
    }

}

void Worker::dispatch()
{
    using namespace std::chrono_literals;

    printf("Dispatcher created\n");

    while (!stop_)
    {
        Operation op = pendingOperations_.receive();

        bool dispatched = false;

        // Loop until finding a thread ready to execute the operation
        while(!dispatched)
        {
            // Select a thread
            for(std::unique_ptr<SmartThread> & thread : threads_)
            {
                if(thread->isIdle())
                {
                    thread->work(op);
                    dispatched = true;
                    break;
                }
            }

            if(!dispatched)
            {
                printf("Warning: all threads are busy! Retrying in 100 ms...\n");
                std::this_thread::sleep_for(100ms);
            }

        }

    }

    printf("Dispatcher exit\n");
}