#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <time.h>

#define SOCKET_NAME "mySocket.socket"

int main(int argc, char *argv[])
{
    struct timespec start;
    struct timespec end;

    struct sockaddr_un addr;
    char buff[10];

    int ds = socket(AF_UNIX, SOCK_SEQPACKET, 0);

    memset(&addr, 0, sizeof(addr));

    // For local connections
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_NAME, sizeof(addr.sun_path) - 1);
    if (connect(ds, (const struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        perror("Server is down\n");
        exit(EXIT_FAILURE);
    }

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
    int m = 0;
    if (argc > 1)
    {
        m = 1;
        strncpy(buff, "DOWN", sizeof("DOWN"));
        write(ds, buff, sizeof(buff));
        close(ds);
        exit(EXIT_SUCCESS);
    }
    else
    {
        int k = 2;
        int lind = 1;
        k -= 1;
        clock_gettime(CLOCK_REALTIME, &start);
        while (k == 1)
        {
            printf("sending Strings Indexed from %d to %d\n", lind, lind + 4);
            for (int i = lind; i < lind + 5; i++)
            {

                if (write(ds, rgs[i], strlen(rgs[i]) + 1) == -1)
                {
                    perror("couldn't write");
                }
            }

            if (read(ds, buff, sizeof(buff)) == -1)
            {
                perror("read");
                exit(EXIT_FAILURE);
            }
            int find;
            buff[sizeof(buff) - 1] = 0;
            find = atoi(buff);

            printf("MAX ID SENT BACK BY SERVER = %s\n\n", buff);
            if (find != 50)
            {
                lind = find + 1;
            }
            else if (find == 50)
            {
                printf("Successfully sent all Strings\n");
                clock_gettime(CLOCK_REALTIME, &end);
                double totalrt = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;
                printf("The runtime is %f seconds\n", totalrt);
                strncpy(buff, "DOWN", sizeof("DOWN"));
                write(ds, buff, sizeof(buff));
                close(ds);
                exit(EXIT_SUCCESS);
                break;
            }
        }
    }
    return 0;
}