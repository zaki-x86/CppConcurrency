#include <thread>
#include <cstdio>
#include <vector>
#include <mutex>

// 2 Threads end up in a deadlock example
// Thread worker1 tries to lock resource_mutex, and then to unlock it, it needs to lock resource_mutex2
// Thread worker2 tries to lock resource_mutex2, and then to unlock it, it needs to lock resource_mutex
// None of the threads is able to unlock their mutexes, so they are stuck in a deadlock

std::mutex resource_mutex; 
int resource = 0;
std::mutex resource_mutex2;
int times = 1000000;

void increment_worker() {
    while (times > 0){
        resource_mutex.lock();
        resource_mutex2.lock();
        --times;
        resource_mutex2.unlock();
        resource++;
        resource_mutex.unlock();
    }
    printf("Worker done\n");
}

void decrement_worker() {
    resource_mutex2.lock();
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    resource_mutex.lock();
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    resource_mutex.unlock();
    resource_mutex2.unlock();
}

int main(int argc, char const *argv[])
{
    std::thread worker1(increment_worker);
    std::thread worker2(decrement_worker);
    
    printf("Resource: %d\n", resource);

    worker1.join();
    worker2.join();

    printf("Resource: %d\n", resource);

    return 0;
}
