#ifndef CLIENT_H
#define CLIENT_H

#include "Result.h"
#include "ICallback.h"
#include "worker.h"

#include <thread>
#include <assert.h>

class Client : public ICallback
{
    public:

        Client(uint32_t numOperations, Worker * worker);

        ~Client();

        //Result stop();

        virtual void onOperationComplete(Operation op) override final;

        void threadCode();

    private:

        uint32_t numOperations_ = 0;
        std::thread * thread_ = nullptr;
        Worker * worker_ = nullptr;

        bool stop_ = false;

   
};



#endif