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
    int flag = 0;
    char *shmemry;
    char buff[6];
    int shmid;
    int k = 1;
    shmid = shmget((key_t)2345, 1024, 0666 | IPC_CREAT);
    shmemry = (char *)shmat(shmid, NULL, 0);
    for (int i = 0; i < 50; i += 5)
    {
        int j = 0;
        while (j < 5)
        {
            while (*shmemry == -1)
            {
                printf("String ID: %d\n", i + j);
            }
            printf("String Value: %s\n", shmemry);
            *shmemry = -1;
            j++;
        }
        flag++;
        printf("\n");
    }
    return 0;
}