#ifndef ICALLBACK_H
#define ICALLBACK_H

#include "stdio.h"
#include "Operation.h"

// Forward declaration
class Operation;

class ICallback
{
    public:
        virtual void onOperationComplete(Operation op) = 0;
};

#endif