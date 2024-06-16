#ifndef OPERATION_H
#define OPERATION_H


#include "Result.h"
#include "ICallback.h"
#include <atomic>
#include <cmath>

class ICallback;

static std::atomic<uint32_t> num_requests{0};

enum OperationType
{
    ADD,
    MULTIPLY,
    DIVISION,
    EXP,
};

class Operation
{
public:

    uint32_t id_ = 0;

    OperationType type_;
    uint32_t arg1_;
    uint32_t arg2_;
    ICallback * callbacker_ = nullptr;

    Result execResult_ = ERROR;
    uint32_t result_ = 0;

    Operation(OperationType type, uint32_t arg1, uint32_t arg2, ICallback * callbacker = nullptr) 
        : type_(type), arg1_(arg1), arg2_(arg2), callbacker_(callbacker)
    {
        id_ = num_requests++;
    }

    Operation()
    {
        id_ = 0;
        execResult_ = ERROR;
    }

    void execute()
    {
        switch (type_)
        {
            case OperationType::ADD:
                result_ = arg1_ + arg2_;
                execResult_ = Result::OK;
                break;
            
            case OperationType::MULTIPLY:
                result_ = arg1_ * arg2_;
                execResult_ = Result::OK;
                break;

            case OperationType::DIVISION:
                if (arg2_ == 0)
                {
                    execResult_ = Result::ERROR;
                }
                else
                {
                    result_ = arg1_ / arg2_;
                    execResult_ = Result::OK;
                }
                break;

            case OperationType::EXP:
                result_ = static_cast<uint32_t>(pow(arg1_, arg2_));
                execResult_ = Result::OK;
                break;
        }
    }

};


#endif