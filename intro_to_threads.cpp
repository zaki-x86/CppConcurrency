#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <string>

// Logger: an example of a daemon thread that provide services to other user threads
// This logger has a fatal flow, some messages are not displayed, depending on the execution

bool finished = false;

struct message
{
    message(std::string text) : _text(text), _isLogged(false) {}

    const bool& isLogged() const {
        return _isLogged;
    }

    bool& isLogged() {
        return _isLogged;
    }

    const char* text() const {
        return _text.c_str();
    }

    void setText(std::string text) {
        _text = text;
        _isLogged = false;
    }

private:
    std::string _text;
    bool _isLogged;
};

struct Logger
{
    Logger(std::string intro) : msg(intro) {}

    void start() {
        logger_task = std::thread(&Logger::logger_service, this);
        logger_task.detach();
    }
    
    void logger_service() {
        while (true)
        {
            if (!msg.isLogged()) {
                printf("Logger: %s\n", msg.text());
                msg.isLogged() = true;
            }
        
            else
                continue;
        }
    }

    void log(const std::string& log_msg) {
        msg.setText(log_msg);
    }

private:
    std::thread logger_task;
    message msg;
};


Logger logger("logger started!");

void thread_worker(const char* name) {
    std::string who(name);
    logger.log(who + " started");
    std::this_thread::sleep_for(std::chrono::seconds(3));
    logger.log(who + " finished");
    return;
}

int main(int argc, char const *argv[])
{ 
    logger.start();

    std::thread worker1(thread_worker, "Thread 1");
    logger.log("Thread 1 started with id");

    logger.log("Workers are working now, i am monitoring\n");
    std::this_thread::sleep_for(std::chrono::seconds(1));
    logger.log("Is thread 1 still working? Yes");

    worker1.join();
    logger.log("Is thread 1 still working? No");
    logger.log("Workers have finished their work\n");

    return 0;
}
