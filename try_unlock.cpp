#include <cstdio>
#include <thread>
#include <mutex>

int resource = 0;
std::mutex m;

void worker(const char* name) {
    int times = 100;
    int tries = 0;

    while (times) {
        if(m.try_lock()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            m.unlock();
            times--;
        }
        else
            printf("%s: try_lock failed, waiting\n", name);
            ++tries;
    }
    printf("%s done\n", name);
    printf("%s tries = %d\n", name, tries);
}

int main(int argc, char const *argv[])
{
    std::thread t1(worker, "worker1");
    std::thread t2(worker, "worker2");

    t1.join();
    t2.join();
    printf("main done\n");
    printf("resource = %d\n", resource);
    
    return 0;
}
