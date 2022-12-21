#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>

int main()
{
    struct timespec start;
    struct timespec end;

    int flag = 0;
    char *shmemry;
    int shmid;
    int k = 1;
    shmid = shmget((key_t)2345, 1024, 0666 | IPC_CREAT);
    shmemry = (char *)shmat(shmid, NULL, 0);
    char rgs[51][10] = {{0}};
    for (int i = 0; i <= 50; i++)
    {
        for (int j = 1; j <= 8; j++)
        {
            rgs[i][j] = rand() % 26 + 97;
        }
    }
    clock_gettime(CLOCK_REALTIME, &start);
    for (int i = 0; i < 50; i += 5)
    {
        int j = 0;
        int m = 0;
        while (j < 5)
        {
            strcpy(shmemry, rgs[i + j]);
            while (*shmemry != -1)
                ;
            j++;
        }
        printf("Maximum ID recieved: %d\n", i + 5);
    }
    clock_gettime(CLOCK_REALTIME, &end);
    double totalrt = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("The runtime is %f seconds\n", totalrt);
    return 0;
}