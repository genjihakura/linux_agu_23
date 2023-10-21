#include <linux/module.h> // cho phep module_int() module_exit()
#include <linux/fs.h> //cho phep alloca_chrdev_region() va register_chrdev_region()
#include <linux/device.h> // cho phep device_create() va class_create()
#include <linux/cdev.h> // cho phep cdev_int() cdev_remove()
#include <linux/slab.h>  /* Define kfree() kzalloc() */
#include <linux/uaccess.h> /* Define copy_to_user(), copy_from_user() */


#define DRIVER_DESCRIPTION "Driver character"
#define DRIVER_AUTHOR "phatlh phattlh@gmail.com"
#define DRIVER_VERSION "1.0"

struct m_foo_dev{
    char *kmalloc_ptr;
    char size_kmalloc;
    dev_t dev_num;
    struct class * m_class;
    struct cdev m_cdev;
} mdev;

static int __init chrdev_init(void);
static void __exit chrdev_exit(void);
static int m_open_dev(struct inode *inode, struct file *filep);
static int m_release_dev(struct inode *inode, struct file *filep);
static ssize_t m_read_dev(struct file *filep, char __user * buff, size_t size, loff_t * offset);
static ssize_t m_write_dev(struct file *filep, const char * buff, size_t size, loff_t * offset);

static struct file_operations fops ={
    .owner = THIS_MODULE,
    .open = m_open_dev,
    .release = m_release_dev,
    .read = m_read_dev,
    .write = m_write_dev,
};

void str_trim_lf(char *str,int size){
    int i =0;
    for( i = 0 ;i< size; i++)
    {
        if(str[i]=='\n'){
            str[i]='\0';
        }
    }
}

void str_trim_en(char *str,int size){
    int i =0;
    for( i = 0 ;i < size; i++)
    {
        if(str[i]=='\0'){
            str[i]='\n';
        }
    }
}

static int m_open_dev(struct inode *inode, struct file *filep){
    pr_info("system call open() .... \n");
    return 0;
}
static int m_release_dev(struct inode *inode, struct file *filep){
    pr_info("system call release() .... \n");
    return 0;
}
static ssize_t m_read_dev(struct file *filep, char __user * buff, size_t size, loff_t * offset){
    size_t to_read = 0;
    pr_info("system call read() .... \n");
    to_read = size > (mdev.size_kmalloc - *offset) ? (mdev.size_kmalloc - *offset) : size;
    if(to_read < 0 ){
         pr_info("to_read is error size \n");
        return -1;
    }

    if(copy_to_user(buff, mdev.kmalloc_ptr ,to_read)){
        return -EFAULT;
    }
    pr_info(KERN_INFO  "Data send to user : %s \n",  mdev.kmalloc_ptr);

    *offset += to_read;
    return to_read;
}

static ssize_t m_write_dev(struct file *filep, const char * buff, size_t size, loff_t * offset){
    size_t to_write = 0;    
    pr_info("system call write() .... \n");
    to_write = size > (mdev.size_kmalloc - *offset) ? (mdev.size_kmalloc - *offset) : size;
    if(to_write < 0 ){
         pr_info("to_write is error size \n");
        return -1;
    }

    if(copy_from_user(mdev.kmalloc_ptr + *offset, buff, to_write) != 0 ){
        return -EFAULT;
    }
    str_trim_lf(mdev.kmalloc_ptr,to_write);

    pr_info(KERN_INFO  "Data from user : %s \n",mdev.kmalloc_ptr);

    //handle data


    *offset += to_write;
    return size;
}

static int __init chrdev_init(void){
    // 1 alloc chdev region
    if(alloc_chrdev_region(&mdev.dev_num, 0, 1, "m_cdev") < 0) {
        pr_info("Error \n");
        return -1;
    }

    pr_info("Major = %d Minor = %d\n", MAJOR(mdev.dev_num), MINOR(mdev.dev_num));

    // 2. class create 
    if((mdev.m_class = class_create(THIS_MODULE, "m_class")) == NULL){
        pr_info("Error create classe\n");
        goto destroy_allocate;
    }

    // 3. device create
    if(device_create( mdev.m_class, NULL, mdev.dev_num, NULL, "m_cdev") < 0){
        pr_info("Error create device class \n");
        goto delete_class;
    }
    // 4. chdev_int
    cdev_init(&mdev.m_cdev, &fops);
    // 5. chdev_add
    if ((cdev_add(&mdev.m_cdev, mdev.dev_num, 1)) < 0) {
         pr_info("Error create device class \n");
        goto delete_device;
    }
    mdev.size_kmalloc = 100;
    mdev.kmalloc_ptr = kzalloc(mdev.size_kmalloc*sizeof(char),GFP_KERNEL);
    pr_info(KERN_INFO  "Charator tlhphat module\n");
    pr_info(KERN_INFO  "Hello world kernel module\n");


    return 0;

delete_device:
    device_destroy(mdev.m_class, mdev.dev_num);
delete_class:
    class_destroy(mdev.m_class);
destroy_allocate:
    unregister_chrdev_region(mdev.dev_num,1);
    kfree(mdev.kmalloc_ptr);
    return -1;
}

static void __exit chrdev_exit(void){
    kfree(mdev.kmalloc_ptr);
    cdev_del(&mdev.m_cdev);
    device_destroy(mdev.m_class, mdev.dev_num);
    class_destroy(mdev.m_class);
    unregister_chrdev_region(mdev.dev_num,1);
    pr_info("character tlhphat goodbye!!\n");
}

module_init(chrdev_init);
module_exit(chrdev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESCRIPTION);
MODULE_VERSION(DRIVER_VERSION);