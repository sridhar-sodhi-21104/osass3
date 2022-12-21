#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <time.h>

sem_t forks[5];

void eat(int flag, int p)
{
    char buff[50] = {'\0'};
    if (flag != 0)
    {
        sprintf(buff, "Philosopher %d has finished eating\n", p + 1);
        write(1, buff, 50);
    }
    else if (flag == 0)
    {
        sprintf(buff, "Philosopher %d is eating\n", p + 1);
        write(1, buff, 50);
    }
}

void *philosopher(void *args1)
{
    int k = 1;
    while (k == 1)
    {

        if (*(int *)args1 != 4)
        {
            sem_wait(&forks[*(int *)args1]);
            sem_wait(&forks[(*(int *)args1 + 1) % 5]);
        }
        else if (*(int *)args1 == 4)
        {
            sem_wait(&forks[(*(int *)args1 + 1) % 5]);
            sem_wait(&forks[*(int *)args1]);
        }

        eat(0, *(int *)args1);
        eat(1, *(int *)args1);

        if (*(int *)args1 != 4)
        {
            sem_post(&forks[(*(int *)args1 + 1) % 5]);
            sem_post(&forks[*(int *)args1]);
        }
        else if (*(int *)args1 == 4)
        {
            sem_post(&forks[*(int *)args1]);
            sem_post(&forks[(*(int *)args1 + 1) % 5]);
        }
    }
}

int main()
{
    pthread_t philosophers[5];

    for (int i = 0; i < 5; i++)
    {
        sem_init(&forks[i], 0, 1);
    }
    int args1[5];
    for (int i = 0; i < 5; ++i)
    {
        args1[i - 1] = i - 1;
        if (pthread_create(&philosophers[i - 1], NULL, philosopher, (void *)(&args1[i - 1])) != 0)
        {
            perror("FAILED TO CREATE THREAD");
            return 1;
        }
    }

    for (int i = 0; i < 5; ++i)
    {
        if (pthread_join(philosophers[i], NULL) != 0)
        {
            return 1;
        }
    }
}