#include <thread>
#include <cstdio>
#include <vector>
#include <mutex>

// Mutex
// protects a resource from being accessed simultaneously by multiple thread
// by locking the resource, only one thread can access it at a time during atomic operations.

std::mutex resource_mutex; 
int resource = 0;

void increment_worker() {
    int times = 10000;
    while (times > 0){
        resource_mutex.lock();
        resource++;
        resource_mutex.unlock();
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
    
    printf("Resource: %d\n", resource);

    return 0;
}
