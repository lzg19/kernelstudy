#ifndef GMODULE_H
#define GMODULE_H

#include <linux/init.h>
#include <linux/version.h>
#include <linux/kconfig.h>
#include <linux/slab.h>
#include <linux/module.h>

#include <linux/kernel.h>   /* printk() */
#include <linux/fs.h>       /* everything... */
#include <linux/errno.h>    /* error codes */
#include <linux/types.h>    /* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h>    /* O_ACCMODE */
#include <linux/pci.h>
#include <linux/interrupt.h>
#include <linux/stat.h>
#include <linux/sched.h>
#include <linux/delay.h>

#include <asm/io.h>
#include <asm/hardirq.h>
#include <asm/uaccess.h>



typedef struct gmodule_s { 
    const char* name;
    int         major;
    int		minor; 

    int (*init)(void);
    int (*cleanup)(void);
  
    int (*pprint)(void);
  
    int (*open)(void);
    int (*ioctl)(unsigned int cmd, unsigned long arg);
    int (*close)(void);
    int (*mmap) (struct file *filp, struct vm_area_struct *vma);

} gmodule_t;


/* The framework will ask for your module definition */
extern gmodule_t* gmodule_get(void);


/* Proc Filesystem information */
extern int pprintf(const char* fmt, ...)
    __attribute__ ((format (printf, 1, 2)));
extern int gmodule_vpprintf(char** page, const char* fmt, va_list args)
    __attribute__ ((format (printf, 2, 0)));
extern int gmodule_pprintf(char** page, const char* fmt, ...)
    __attribute__ ((format (printf, 2, 3)));

extern int gprintk(const char* fmt, ...)
    __attribute__ ((format (printf, 1, 2)));

extern int gdbg(const char* fmt, ...)
    __attribute__ ((format (printf, 1, 2)));
#define GDBG gdbg

#endif