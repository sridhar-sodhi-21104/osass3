#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_NAME "mySocket.socket"

int main(int argc, char *argv[])
{
    struct sockaddr_un servername;
    int cs;

    char buff[10];

    cs = socket(AF_UNIX, SOCK_SEQPACKET, 0);

    memset(&servername, 0, sizeof(servername));
    servername.sun_family = AF_UNIX;
    strncpy(servername.sun_path, SOCKET_NAME, sizeof(servername.sun_path) - 1);

    if (bind(cs, (const struct sockaddr *)&servername, sizeof(servername)) == -1)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if (listen(cs, 100) == -1)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    int mind = 0;
    int k = 1;
    int ds;
    int down_flag = 0;
    int l = 0;
    while (k == 1)
    {
        ds = accept(cs, NULL, NULL);
        if (ds == -1)
        {
            perror("couldn't accept");
            exit(EXIT_FAILURE);
        }

        mind = 0;
        int cind = mind;
        while (1)
        {
            if (read(ds, buff, sizeof(buff)) == -1)
            {
                perror("couldn't read");
                exit(EXIT_FAILURE);
            }
            l = 2;

            buff[sizeof(buff) - 1] = 0;
            cind = buff[0];
            int f = 1;
            if (!strncmp(buff, "DOWN", sizeof(buff)))
            {
                down_flag = 1;
                break;
            }

            printf("STRING SENT BY CLIENT: ");
            int o = 1;
            while (o < 9)
            {
                printf("%c", buff[o]);
                o++;
            }

            printf("\n");
            sprintf(buff, "%d", cind);
            printf("ID SENT BY CLIENT = %s\n", buff);

            if (cind == mind + 5)
            {
                mind = cind;
                write(ds, buff, sizeof(buff));
            }

            if (cind >= 50)
                break;
        }

        close(ds);
        int m = 0;
        if (down_flag)
        {
            m = 1;
            printf("SHUTTING SERVER");
            close(cs);
            unlink(SOCKET_NAME);
            exit(EXIT_SUCCESS);
            break;
        }
    }

    return 0;
}