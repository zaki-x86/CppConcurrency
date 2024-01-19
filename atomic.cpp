#include <thread>
#include <cstdio>
#include <vector>
#include <atomic>

// Atomic
// ensures thread safety same as mutex.
// best used for simple operations, and read/write operations when working w/ multiple threads.

std::atomic_int resource = 0;

void increment_worker() {
    int times = 10000;
    while (times > 0){
        resource++;
        --times;
    }
    printf("Worker done\n");
}

int main(int argc, char const *argv[])
{
    const size_t threads_num = 20;
    std::vector<std::thread> workers;
    for (size_t i = 0; i < threads_num; i++)
    {
        workers.push_back(std::thread(increment_worker));
    }
    

    for (auto &&worker : workers)
    {
        worker.join();
    }
    
    printf("Resource: %d\n", resource.load());

    return 0;
}
