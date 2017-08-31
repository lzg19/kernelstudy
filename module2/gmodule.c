
#include <linux/init.h>
#include <linux/seq_file.h>
#include <linux/cdev.h>

#include "gmodule.h"
#include "testmodule.h"

//here the interfaces used here are also very old

static gmodule_t* _gmodule = NULL;


static struct seq_file* _proc_buf = NULL;


static int _dbg_enable = 0;

static int
gvprintk(const char* fmt, va_list args)
    __attribute__ ((format (printf, 1, 0)));

static int
gvprintk(const char* fmt, va_list args)
{
    static char _buf[256];
  
    strcpy(_buf, "");
    sprintf(_buf, "%s (%d): ", _gmodule->name, current->pid); 
    vsprintf(_buf+strlen(_buf), fmt, args);
    printk(_buf);
  
    return 0;
}

int
gprintk(const char* fmt, ...)
{
    int rv;

    va_list args;
    va_start(args, fmt);
    rv = gvprintk(fmt, args);
    va_end(args);
    return rv;
}

int 
gdbg(const char* fmt, ...)
{
    int rv = 0;

    va_list args;
    va_start(args, fmt);
    if(_dbg_enable) {
	rv = gvprintk(fmt, args);
    }
    va_end(args);
    return rv;
}

int 
pprintf(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    seq_vprintf(_proc_buf, fmt, args);
    va_end(args);
    return 0;
}

static int _gmodule_proc_show(struct seq_file *m, void *v){
    _proc_buf = m;
    _gmodule->pprint();
    return 0;
}

static int
_gmodule_proc_open(struct inode * inode, struct file * file) {
    if(_gmodule->open) {
        _gmodule->open();
    }

    return single_open(file, _gmodule_proc_show, NULL);
}

static ssize_t
_gmodule_proc_write(struct file *file, const char *buffer,
                   size_t count, loff_t *loff)
{
    /* Workaround to toggle debugging */
    if(count > 2) {
	if(buffer[0] == 'd') {
	    _dbg_enable = buffer[1] - '0';
	    GDBG("Debugging Enabled");
	}
    }
    return count;
}

static int _gmodule_proc_release(struct inode * inode, struct file * file) {
    if(_gmodule->close) {
        _gmodule->close();
    }

    return single_release(inode, file);
}

struct file_operations _gmodule_proc_fops = {
    owner:      THIS_MODULE,
    open:       _gmodule_proc_open,
    read:       seq_read,
    llseek:     seq_lseek,
    write:      _gmodule_proc_write,
    release:    _gmodule_proc_release,
};

static int
_gmodule_create_proc(void)
{
    struct proc_dir_entry* ent;
	printk("create proc file here\n");
    if((ent = proc_create(_gmodule->name,
                          S_IRUGO | S_IWUGO,
                          NULL,
                          &_gmodule_proc_fops)) != NULL) {
		printk("create proc successfully\n");
        return 0;
    }
	printk("create proc failed\n");
    return -1;
}

static void 
_gmodule_remove_proc(void)
{
	printk("try to remove the proc %s\n", _gmodule->name);
    remove_proc_entry(_gmodule->name, NULL);
}


static int
_gmodule_open(struct inode *inode, struct file *filp)
{
    if(_gmodule->open) {
	_gmodule->open();
    }
    return 0;
}

static int 
_gmodule_release(struct inode *inode, struct file *filp)
{
    if(_gmodule->close) {
	_gmodule->close();
    }
    return 0;
}

#ifdef HAVE_UNLOCKED_IOCTL
static long
_gmodule_unlocked_ioctl(struct file *filp,
                        unsigned int cmd, unsigned long arg)
{
    if(_gmodule->ioctl) {
	return _gmodule->ioctl(cmd, arg);
    } else {
	return -1;
    }
}
#else
static int 
_gmodule_ioctl(struct inode *inode, struct file *filp,
	       unsigned int cmd, unsigned long arg)
{
    if(_gmodule->ioctl) {
	return _gmodule->ioctl(cmd, arg);
    } else {
	return -1;
    }
}
#endif

#ifdef HAVE_COMPAT_IOCTL
static long
_gmodule_compat_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    if(_gmodule->ioctl) {
	return _gmodule->ioctl(cmd, arg);
    } else {
	return -1;
    }
}
#endif

static int
_gmodule_mmap(struct file *filp, struct vm_area_struct *vma)
{
    if (_gmodule->mmap) {
        return _gmodule->mmap(filp, vma);
    }
    return -EPERM;
}


struct file_operations _gmodule_fops = {
#ifdef HAVE_UNLOCKED_IOCTL
    unlocked_ioctl: _gmodule_unlocked_ioctl,
#else
    ioctl:      _gmodule_ioctl,
#endif
    open:       _gmodule_open,
    release:    _gmodule_release,
    mmap:       _gmodule_mmap,
#ifdef HAVE_COMPAT_IOCTL
    compat_ioctl: _gmodule_compat_ioctl,
#endif
};

//struct cdev g_cdev;
static struct cdev g_cdev;
void __exit
cleanup_module(void)
{
    if(!_gmodule) return;
  
    /* Specific Cleanup */
    if(_gmodule->cleanup) {
		_gmodule->cleanup();
    }
  
    /* Remove any proc entries */
    if(_gmodule->pprint) {
		//_gmodule_remove_proc();
    }
  
    /* Finally, remove ourselves from the universe */
    //unregister_chrdev(_gmodule->major, _gmodule->name);
	cdev_del(&g_cdev);
}

int __init init_module(void){
	int rc, err, devno;
	
	printk(KERN_INFO "enter init module\n");

    /* Get our definition */
    _gmodule = gmodule_get();
    if(!_gmodule) {
		printk("get gmodule null here\n");
		return -ENODEV;
	}
	
	devno = MKDEV(_gmodule->major, 0);
	cdev_init(&g_cdev, &_gmodule_fops);
	err = cdev_add(&g_cdev, devno, 1);
	if (err){
		printk(KERN_INFO "Error %d adding device\n", err);
	}
#if 0
    /* Get our definition */
    _gmodule = gmodule_get();
    if(!_gmodule) {
		printk("get gmodule null here\n");
		return -ENODEV;
	}
	
	rc = register_chrdev(_gmodule->major, 
			 _gmodule->name, 
			 &_gmodule_fops);
    if (rc < 0) {
		printk(KERN_WARNING "%s: can't get major %d",
			   _gmodule->name, _gmodule->major);
		return rc;
    }
    if(_gmodule->major == 0) {
		_gmodule->major = rc;
    }
#endif

    if(_gmodule->init) {
		if((rc = _gmodule->init()) < 0) {
            unregister_chrdev(_gmodule->major, _gmodule->name);
			return rc;
		}
    }

    /* Add a /proc entry, if valid */
	#if 0
    if(_gmodule->pprint) {    
		_gmodule_create_proc();
    }
	#endif
	return 0;
}
