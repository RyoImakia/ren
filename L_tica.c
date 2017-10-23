#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void){

        printf("Content-type: text/html\n\n");
        int fd = 0;


        fd = open("/sys/class/gpio/export", O_WRONLY);
        if (fd < 0) {
                printf("GPIO export open ƒGƒ‰[.\n");
                exit(1);
        }
        write(fd,"21",3);
        close(fd);


        usleep(100000);

        fd = open("/sys/class/gpio/gpio21/direction", O_WRONLY);
        if (fd < 0) {
                printf("GPIO direction open error.\n");
                exit(1);
        }
        write(fd,"out",4);
        close(fd);

        fd = open("/sys/class/gpio/gpio21/value", O_WRONLY);
        if (fd < 0) {
                printf("GPIO value open error.\n");
                exit(1);
        }
        write(fd,"1",2);

        usleep(3 * 1000 * 1000);

        write(fd,"0",2);
        close(fd);

        fd = open("/sys/class/gpio/unexport", O_WRONLY);
        if (fd < 0) {
                printf("GPIO unexport open error.\n");
                exit(1);
        }
        write(fd,"21",3);
        close(fd);

        return 0;
}