#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include <sys/ioctl.h>

#define LUBDE_MAGIC 'L'

#define LUBDE_VERSION             _IO(LUBDE_MAGIC, 0)

/* Ioctl control structure */
typedef struct  {
    unsigned int dev;   /* Device ID */
    unsigned int rc;    /* Operation Return Code */
    unsigned int d0;    /* Operation specific data */
    unsigned int d1;
    unsigned int d2;
    unsigned int d3;    
    union {
        unsigned int dw[2];
        unsigned char buf[64];
    } dx;
} lubde_ioctl_t;

static int _devfd = -1;

#define LINUX_USER_BDE_NAME 	"linux-user-bde"
#define LUBDE_DEVICE_NAME "/dev/" LINUX_USER_BDE_NAME

static int
_ioctl(unsigned int command, lubde_ioctl_t *pdevio)
{
    pdevio->rc = -1;
    ioctl(_devfd, command, pdevio);
    return pdevio->rc;
}

void main(int argc, char **argv){
	lubde_ioctl_t devio;
	int rc;
	_devfd = open(LUBDE_DEVICE_NAME, O_RDWR | O_SYNC | O_DSYNC | O_RSYNC);
	if (_devfd < 0){
		printf("can not open the file %s\n", LUBDE_DEVICE_NAME);
		return -1;
	}
	
	rc = _ioctl(LUBDE_VERSION, &devio);
	printf("ret of ioctl status is %d\n", rc);
	return 0;
}