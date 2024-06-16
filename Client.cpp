#include "Client.h"
#include "Operation.h"


#include <iostream>
#include <chrono>


Client::Client(uint32_t numOperations, Worker * worker) : numOperations_(numOperations) 
{
    assert(worker != nullptr);
    worker_ = worker;

    thread_ = new std::thread(&Client::threadCode, this);
};

Client::~Client()
{
    thread_->join();
    delete thread_;
}

void Client::onOperationComplete(Operation op)
{
    if(op.execResult_ == Result::OK)
    {
        printf("Operation %u has been completed SUCCESSFULLY. Result: %u \n", op.id_, op.result_);
    }
    else
    {
        printf("Operation %u has FAILED! \n", op.id_);
    }
}



void Client::threadCode()
{
    for (int i = 0; i < numOperations_; i++)
    {
        Operation op (OperationType::ADD, 1, 1, this);

        // Some operations will be requested to be executed at the moment
        if(i % 10 == 0)
        {
            worker_->executeOperation(op);
        }
        else        // The rest will be enqueued
        {
            worker_->addOperation(op);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}


