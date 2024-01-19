#include <thread>
#include <cstdio>
#include <vector>
#include <atomic>
#include <mutex>
#include <chrono>

// Mutex vs Atomic. Which is faster for simpler ops?

std::atomic_uint resource1 = 0;
unsigned int resource2 = 0;

std::mutex lock;

void increment_atomic_worker() {
    int times = 1000000;
    while (times > 0){
        resource1++;
        --times;
    }
}

void increment_mutex_worker() {
    int times = 10000;
    while (times > 0){
        lock.lock();
        resource2++;
        lock.unlock();
        --times;
    }
}
        

int main(int argc, char const *argv[])
{
    const size_t threads_num = 200;
    std::vector<std::thread> workersA;
    std::vector<std::thread> workersB;

    auto start = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < threads_num; i++)
    {
        workersA.push_back(std::thread(increment_atomic_worker));
    }

    for (auto &&worker : workersA)
    {
        worker.join();
    }

    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    printf("Time taken by function <increment_atomic_worker>: %ld microseconds\n", duration.count());
    printf("Resource count: %d\n", resource1.load());

    start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < threads_num; i++){
        workersB.push_back(std::thread(increment_mutex_worker));
    }
    for (auto &&worker : workersB) {
        worker.join();
    }
    end = std::chrono::high_resolution_clock::now();

    printf("Time taken by function <increment_mutex_worker>: %ld microseconds\n", duration.count());
    printf("Resource count: %d\n", resource2);

    return 0;
}
