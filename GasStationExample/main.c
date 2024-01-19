#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

int fuel = 0;
pthread_mutex_t fuel_mutex;
pthread_cond_t fuel_cond;

void* fuel_filling(void* arg) {
    printf("Filling started\n");
    for (size_t i = 0; i < 5; i++)
    {
        pthread_mutex_lock(&fuel_mutex);
        fuel += 15;
        printf("Filled tank with %d liters\n", fuel);
        pthread_mutex_unlock(&fuel_mutex);
        sleep(1); // 1 second break after each refueling
    }
    pthread_cond_signal(&fuel_cond); // signal that the tank is filled
    printf("Total filled in tank is %d liters\n", fuel);
}

void* car(void* args) {
    int tank = 0;
    printf("Car fueling has started\n");
    pthread_mutex_lock(&fuel_mutex);
    pthread_cond_init(&fuel_cond, NULL);
    while (fuel < 20 && tank < 100) {
        printf("Not enough fuel. waiting\n");
        pthread_cond_wait(&fuel_cond, &fuel_mutex); // wait
        // wait is equivalent to: unlock mutex -> wait -> lock mutex
    } 
    fuel -= 15;
    tank += 15;
    printf("Car fueling. Left in the main facility: %d\n", fuel);
    printf("Car fueling. Filled in car tank: %d\n", tank);
    pthread_mutex_unlock(&fuel_mutex);
    if (tank == 15)
        printf("Car fueled. Left in the main facility: %d\n", fuel);
    else
        printf("Car not fueled. Left in the main facility: %d\n", fuel);
    
}

int main(int argc, char const *argv[])
{
    pthread_t th[2];
    pthread_mutex_init(&fuel_mutex, NULL);
    for (size_t i = 0; i < 2; i++)
    {
        if (i == 1)
        {
            if (pthread_create(&th[i], NULL, &fuel_filling, NULL) != 0)
            {
                perror("Error starting filling service\n");
            }
        }
        else {
            if (pthread_create(&th[i], NULL, &car, NULL) != 0)
            {
                perror("Error starting car service\n");
            }    
        }
    }

    // processing

    for (size_t i = 0; i < 2; i++)
    {
        if (pthread_join(th[i], NULL) != 0)
        {
            perror("Error joining thread\n");
        }
    }
    pthread_mutex_destroy(&fuel_mutex);
    pthread_cond_destroy(&fuel_cond);
    
    return 0;
}
