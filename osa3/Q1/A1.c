#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int forks[5] = {1, 1, 1, 1, 1};
pthread_t philosophers[5];

void think(int philNo, int k)
{
    printf("Philosopher %d is thinking\n", philNo);
    sleep(k + 1);
}

void eat(int philNo, int k)
{
    int right = (philNo + 1) % 5;
    int left = (philNo + 5) % 5;
    if (philNo != 4)
    {
        while (left <= 0)
        {
        }
        left--;
        while (right <= 0)
        {
        }
        right--;

        // pthread_mutex_lock(&forks[left]);
        // pthread_mutex_lock(&forks[right]);
        printf("Philosopher %d is eating\n", philNo);
        sleep(k + 1);
    }
    else if (philNo == 4)
    {
        while (right <= 0)
        {
        }
        right--;
        while (left <= 0)
        {
        }
        left--;
        // pthread_mutex_lock(&forks[right]);
        // pthread_mutex_lock(&forks[left]);

        printf("Philosopher %d is eating\n", philNo);
        sleep(k + 1);
    }

    printf("Philosopher %d will put down their forks\n", (philNo + 1));
    right++;
    left++;
    // pthread_mutex_unlock(&forks[(philNo + 1) % 5]);
    // pthread_mutex_unlock(&forks[(philNo + 5) % 5]);
}

void *philosopher(void *philNo)
{
    while (1)
    {
        int k = 0;
        think(philNo, k);
        eat(philNo, k);
    }
}

int main()
{
    // for (int i = 0; i < 5; i++)
    // {
    //     pthread_mutex_init(&forks[i], NULL);
    // }

    for (int i = 0; i < 5; i++)
    {
        if (pthread_create(&philosophers[i], NULL, philosopher, (void *)(i)) != 0)
        {
            perror("FAILED TO CREATE THREAD");
            return 1;
        }
    }

    for (int i = 0; i < 5; i++)
    {
        if (pthread_join(philosophers[i], NULL) != 0)
        {
            return 1;
        }
    }

    return 0;
}