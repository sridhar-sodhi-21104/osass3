#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <sys/un.h>

int main()
{
    int fd1, fd2;
    char buff[100];
    int mind = 0;
    char *fifo1 = "FIFO1";
    char *fifo2 = "FIFO2";

    mkfifo(fifo1, 0777);
    mkfifo(fifo2, 0777);

    int k = 1;
    mind = 0;
    int cind = mind;
    while (k == 1)
    {

        while (cind < mind + 5)
        {
            sleep(2);
            fd1 = open(fifo1, O_RDONLY);

            if (read(fd1, buff, sizeof(buff)) == -1)
            {
                perror("couldn't read");
                exit(EXIT_FAILURE);
            }
            close(fd1);
            cind = buff[0];
            printf("STRING RECEIVED FROM CLIENT: ");
            int i = 0;
            while (i < 10)
            {
                printf("%c", buff[i]);
                i++;
            }
            printf("\n");

            sprintf(buff, "%d", cind);

            printf("ID RECEIVED FROM CLIENT : %s\n", buff);
        }

        mind = cind;
        fd2 = open(fifo2, O_WRONLY);
        int r = write(fd2, buff, sizeof(buff));
        close(fd2);
        if (mind >= 50)
            exit(EXIT_SUCCESS);
    }
    return 0;
}