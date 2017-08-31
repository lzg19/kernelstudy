#ifndef TESTMODULE_H
#define TESTMODULE_H

#include "gmodule.h"

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

#define LUBDE_MAGIC 'L'

#define LUBDE_VERSION             _IO(LUBDE_MAGIC, 0)
#define LUBDE_GET_NUM_DEVICES     _IO(LUBDE_MAGIC, 1)
#define LUBDE_GET_DEVICE          _IO(LUBDE_MAGIC, 2)
#define LUBDE_PCI_CONFIG_PUT32    _IO(LUBDE_MAGIC, 3)
#define LUBDE_PCI_CONFIG_GET32    _IO(LUBDE_MAGIC, 4)
#define LUBDE_GET_DMA_INFO        _IO(LUBDE_MAGIC, 5)
#define LUBDE_ENABLE_INTERRUPTS   _IO(LUBDE_MAGIC, 6)
#define LUBDE_DISABLE_INTERRUPTS  _IO(LUBDE_MAGIC, 7)
#define LUBDE_USLEEP              _IO(LUBDE_MAGIC, 8)
#define LUBDE_WAIT_FOR_INTERRUPT  _IO(LUBDE_MAGIC, 9)
#define LUBDE_SEM_OP              _IO(LUBDE_MAGIC, 10)
#define LUBDE_UDELAY              _IO(LUBDE_MAGIC, 11)
#define LUBDE_GET_DEVICE_TYPE     _IO(LUBDE_MAGIC, 12)
#define LUBDE_SPI_READ_REG        _IO(LUBDE_MAGIC, 13)
#define LUBDE_SPI_WRITE_REG       _IO(LUBDE_MAGIC, 14)
#define LUBDE_READ_REG_16BIT_BUS  _IO(LUBDE_MAGIC, 19)
#define LUBDE_WRITE_REG_16BIT_BUS _IO(LUBDE_MAGIC, 20)
#define LUBDE_GET_BUS_FEATURES    _IO(LUBDE_MAGIC, 21)
#define LUBDE_WRITE_IRQ_MASK      _IO(LUBDE_MAGIC, 22)
#define LUBDE_CPU_WRITE_REG       _IO(LUBDE_MAGIC, 23)
#define LUBDE_CPU_READ_REG        _IO(LUBDE_MAGIC, 24)
#define LUBDE_CPU_PCI_REGISTER    _IO(LUBDE_MAGIC, 25)
#define LUBDE_DEV_RESOURCE        _IO(LUBDE_MAGIC, 26)
#define LUBDE_IPROC_READ_REG      _IO(LUBDE_MAGIC, 27)
#define LUBDE_IPROC_WRITE_REG     _IO(LUBDE_MAGIC, 28)
#define LUBDE_ATTACH_INSTANCE     _IO(LUBDE_MAGIC, 29)
#define LUBDE_GET_DEVICE_STATE    _IO(LUBDE_MAGIC, 30)

#define LUBDE_SEM_OP_CREATE       1
#define LUBDE_SEM_OP_DESTROY      2
#define LUBDE_SEM_OP_TAKE         3
#define LUBDE_SEM_OP_GIVE         4

#define LUBDE_SUCCESS 0
#define LUBDE_FAIL ((unsigned int)-1)


/* 
 * Version history
 * 1:add LUBDE_GET_DEVICE_STATE to support PCI hot plug
 */
#define KBDE_VERSION    1

#define LUBDE_INTERRUPT_SIGNAL    SIGUSR1
#define LUBDE_ETHER_INTERRUPT_SIGNAL    SIGUSR2

extern gmodule_t* gmodule_get(void);

#endif


