#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <sys/un.h>
void forerror()
{
    perror("Couldn't access FIFO1");
}
int main()
{
    struct timespec start;
    struct timespec end;

    int fd1, fd2, ret;
    char buff[100];

    char rgs[51][10] = {{0}};

    srand(time(NULL));

    int i = 0;

    while (i <= 50)
    {
        rgs[i][0] = i;
        i++;
    }

    for (int i = 0; i <= 50; i++)
    {
        for (int j = 1; j <= 8; j++)
        {
            rgs[i][j] = rand() % 26 + 97;
        }
    }

    for (int i = 1; i <= 50; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (j != 0)
            {
                printf("%c", rgs[i][j]);
            }
            else if (j == 0)
            {
                printf("%d ", rgs[i][j]);
            }
        }
        printf("\n");
    }

    printf("\n%s\n\n", "The above strings are random strings created");

    char *fifo1 = "FIFO1";
    char *fifo2 = "FIFO2";
    mkfifo(fifo1, 0777);
    mkfifo(fifo2, 0777);
    int k = 1;
    int lind = 1;
    clock_gettime(CLOCK_REALTIME, &start);
    while (k == 1)
    {
        if (fd1 == -1)
        {
            forerror();
        }
        printf("Sending Strings from : %d to %d\n", lind, lind + 4);
        int i = lind;
        while (i < lind + 5)
        {
            fd1 = open(fifo1, O_WRONLY);
            ret = write(fd1, rgs[i], sizeof(rgs[i]) + 1);
            sleep(1);
            close(fd1);
            i++;
        }
        fd2 = open(fifo2, O_RDONLY);

        if (read(fd2, buff, sizeof(buff)) == -1)
        {
            perror("read");
            exit(EXIT_FAILURE);
        }
        close(fd2);

        int find = atoi(buff);

        printf("MAX ID RECEIVED FROM SERVER = %s\n\n", buff);

        lind = find + 1;
        if (lind >= 50)
        {
            clock_gettime(CLOCK_REALTIME, &end);
            double totalrt = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;

            printf("The runtime is %f seconds\n", totalrt);
            exit(EXIT_SUCCESS);
        }
    }

    return 0;
}