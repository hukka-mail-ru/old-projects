#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/version.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/ioport.h>
#include <asm/system.h>
#include <asm/io.h>
#include <linux/proc_fs.h> // proc
#include <linux/ioctl.h> /* needed for the _IOW etc stuff used later */
 
 
MODULE_LICENSE("Dual BSD/GPL");


// program parameters
static int major = 0;
static int minor = 0;
module_param(major, int, S_IRUGO);
module_param(minor, int, S_IRUGO);

// structures 
static DECLARE_MUTEX(sem);
static struct cdev* my_cdev;


// functions

int hello_open(struct inode *inode, struct file *filp)
{
    return 0;          /* success */
}

int hello_release(struct inode *inode, struct file *filp)
{
    return 0;
}

//////////////// WRITE ///////////////////////////////////////////

ssize_t hello_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos) 
{
    return count;
}

//////////////// READ ///////////////////////////////////////////

ssize_t hello_read(struct file *filp, char __user *buf, size_t count,
                   loff_t *f_pos)
{

    return count;
}



//////////////// FILE OPERATIONS ///////////////////////////////////////////

static struct file_operations my_ops = 
{
    .owner      = THIS_MODULE,
    .read       = hello_read,
    .write      = hello_write,
    .open       = hello_open,
    .release    = hello_release,
};


//////////////// PROC ///////////////////////////////////////////

int hello_read_procmem(char *buf, char **start, off_t offset,
                       int count, int *eof, void *data)
{
    int len = 0;

    down(&sem);

    len += sprintf(buf+len, "The process is '%s' (pid %i)\n",
                             current->comm, current->pid);
    up(&sem);

    *eof = 1;
    return len;
}

static void hello_create_proc(void)
{
    create_proc_read_entry("hellomem", 0 /* default mode */,
            NULL /* parent dir */, hello_read_procmem,
            NULL /* client data */);
}

static void hello_remove_proc(void)
{
    remove_proc_entry("hellomem", NULL /* parent dir */);
}
//////////////// EO PROC ///////////////////////////////////////////





static int startup(void)
{
    dev_t dev = MKDEV(major, minor);
    int result = 0;

    printk(KERN_INFO "hello: startup\n");
    
    // get a driver number
    if (major) 
    {
        dev = MKDEV(major, minor);
        result = register_chrdev_region(dev, 1, "hello");
    } 
    else 
    {
        result = alloc_chrdev_region(&dev, minor, 1, "hello");
        major = MAJOR(dev);
    }
    
    if (result < 0) 
    {
        printk(KERN_WARNING "hello: can't get version %d:%d\n", major, minor);
        return result;
    }

       
    // Initialize the device.	
    my_cdev = cdev_alloc();
    if(!my_cdev)
    {
       printk(KERN_WARNING "hello:  cdev_alloc failed");
       return -1;
    }

    my_cdev->ops = &my_ops;
    
    if(cdev_add(my_cdev, dev, 1) < 0)
    {
       printk(KERN_WARNING "hello:  cdev_add failed");
       return -1;
    }

    
    hello_create_proc(); // proc debugging

    printk(KERN_WARNING "hello: got version %d:%d\n", major, minor);
    printk(KERN_WARNING "hello: my_cdev allocated\n");
    printk(KERN_WARNING "hello: my_cdev added\n");
    
    return 0;
}

static void kickoff(void)
{
    dev_t dev = MKDEV(major, minor);
    
    hello_remove_proc(); // proc debugging
    
    cdev_del(my_cdev); // Denitialize the device

    unregister_chrdev_region(dev, 1); // we had only 1 device

    printk(KERN_INFO "hello: kicked off\n");
}




module_init(startup);
module_exit(kickoff);
