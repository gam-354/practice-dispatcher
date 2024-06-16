#include "worker.h"
#include "Client.h"

const int INITIAL_NUM_THREADS = 2;
const int NUM_OPERATIONS = 10;



int main ()
{
    Worker myWorker(INITIAL_NUM_THREADS);

    // Wait some time until worker ready
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    Client myClient(NUM_OPERATIONS, &myWorker);

    myWorker.waitUntilFinished();

    return 0;
}