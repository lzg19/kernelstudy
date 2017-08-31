#include "testmodule.h"

#define LINUX_USER_BDE_MAJOR   	126

#define LINUX_USER_BDE_NAME "linux-user-bde"
static char _modname[] = "linux-user-bde";

static int
_init(void)
{
	printk("init linux user bde platform\n");
    return 0;
}


static int
_cleanup(void)
{
	printk("clean up bde platform\n");
    return 0;
}

static int
_pprint(void)
{
	printk("here is the information of the bde\n");
    return 0;
}

static int 
_ioctl(unsigned int cmd, unsigned long arg)
{
    lubde_ioctl_t io;
    unsigned int pbase, size;

    if (copy_from_user(&io, (void *)arg, sizeof(io))) {
        return -EFAULT;
    }
  
    io.rc = LUBDE_SUCCESS;
  
    switch(cmd) {
    case LUBDE_VERSION:
		printk("here is the version\n");
        break;
    case LUBDE_GET_NUM_DEVICES:
        break;
    case LUBDE_GET_DEVICE:
        break;
    case LUBDE_GET_DEVICE_TYPE:
        break;
    case LUBDE_GET_BUS_FEATURES:
        break;
    case LUBDE_PCI_CONFIG_PUT32:
        break;
    case LUBDE_PCI_CONFIG_GET32:
        break;
    case LUBDE_GET_DMA_INFO:
        break;
    case LUBDE_ENABLE_INTERRUPTS:
        break;
    case LUBDE_DISABLE_INTERRUPTS:
        break;
    case LUBDE_WAIT_FOR_INTERRUPT:
        break;
    case LUBDE_USLEEP:
        break;
    case LUBDE_UDELAY:
        break;
    case LUBDE_SEM_OP:
        break;
    case LUBDE_WRITE_IRQ_MASK:
        break;
    case LUBDE_SPI_READ_REG:
        break;
    case LUBDE_SPI_WRITE_REG:
        break;
    case LUBDE_READ_REG_16BIT_BUS:
        break;
    case LUBDE_WRITE_REG_16BIT_BUS:
        break;
    case LUBDE_DEV_RESOURCE:
        break;
    case LUBDE_IPROC_READ_REG:
        break;
    case LUBDE_IPROC_WRITE_REG:
        break;
    case LUBDE_ATTACH_INSTANCE:
        break;
    case LUBDE_GET_DEVICE_STATE:
        break;
    default:
        break;
    }
  
    if (copy_to_user((void *)arg, &io, sizeof(io))) {
        return -EFAULT;
    }

    return 0;
}

static gmodule_t _gmodule = 
{
    name: LINUX_USER_BDE_NAME, 
    major: LINUX_USER_BDE_MAJOR, 
    init: _init, 
    cleanup: _cleanup, 
    pprint: _pprint, 
    ioctl: _ioctl,
}; 

gmodule_t*
gmodule_get(void)
{
    _gmodule.name = _modname;
    return &_gmodule;
}
