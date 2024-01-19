#include <thread>
#include <cstdio>
#include <vector>

// Data race example
// given 10 threads that will increment resource 10,000 times, we should get final result
// resource = 100,000
// but actually we don't!
// and this is b/c of data race

int resource = 0;

void increment_worker(int times) {
    while (times > 0){
        resource++;
        --times;
    }
    printf("Worker done\n");
}

int main(int argc, char const *argv[])
{
    const size_t threads_num = 10;
    std::vector<std::thread> workers;
    for (size_t i = 0; i < threads_num; i++)
    {
        workers.push_back(std::thread(increment_worker, 10000));
    }
    

    for (auto &&worker : workers)
    {
        worker.join();
    }
    
    printf("Resource: %d\n", resource);

    return 0;
}
