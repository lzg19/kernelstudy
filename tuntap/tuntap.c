#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <linux/if.h>
#include <linux/if_tun.h>

int tun_open(char *devname){
    struct ifreq ifr;
    int fd, err;
    fd = open("/dev/net/tun", O_RDWR);
    memset(&ifr, 0, sizeof(ifr));
    ifr.ifr_flags = IFF_TUN;
    strncpy(ifr.ifr_name, devname, IFNAMSIZ);
    ioctl(fd, TUNSETIFF, (void*)&ifr);
    return fd;
}

int main(int argc, char *argv[]){
    int fd, nbytes, i;
    unsigned char buf[1600];
    fd = tun_open("asa0");
    printf("device asa0 open");

    while(1){
        nbytes = read(fd, buf, sizeof(buf));
        printf("-------------------read %d bytes from asa0\n", nbytes);
        for (i = 0; i < nbytes; i++){
            if (i % 16 == 0){
                printf("\n");
            }
            printf("%02x ",buf[i]);
        }
        printf("\n");
    }
    return 0;
}