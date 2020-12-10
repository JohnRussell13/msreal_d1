#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/string.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/cdev.h>
#include <linux/kdev_t.h>
#include <linux/uaccess.h>
#include <linux/errno.h>
#define BUFF_SIZE 120
#define STORAGE_SIZE 100

MODULE_LICENSE("Dual BSD/GPL");

dev_t my_dev_id;
static struct class *my_class;
static struct device *my_device;
static struct cdev *my_cdev;

char storage[STORAGE_SIZE];
int pos = 0;
int endRead = 0;
char mods[6][9];
unsigned char mods_len[6];

int stred_open(struct inode *pinode, struct file *pfile);
int stred_close(struct inode *pinode, struct file *pfile);
ssize_t stred_read(struct file *pfile, char __user *buffer, size_t length, loff_t *offset);
ssize_t stred_write(struct file *pfile, const char __user *buffer, size_t length, loff_t *offset);

struct file_operations my_fops =
{
	.owner = THIS_MODULE,
	.open = stred_open,
	.read = stred_read,
	.write = stred_write,
	.release = stred_close,
};


int stred_open(struct inode *pinode, struct file *pfile) 
{
	printk(KERN_INFO "Succesfully opened file\n");
	return 0;
}

int stred_close(struct inode *pinode, struct file *pfile) 
{
	printk(KERN_INFO "Succesfully closed file\n");
	return 0;
}

ssize_t stred_read(struct file *pfile, char __user *buffer, size_t length, loff_t *offset) 
{
	int ret;
	char buff[BUFF_SIZE];
	long int len;
	if (endRead){
		endRead = 0;
		pos = 0;
		printk(KERN_INFO "Succesfully read from storage, size: %d\n", strlen(storage));
		return 0;
	}
	len = scnprintf(buff,BUFF_SIZE , "%c", storage[pos]);
	ret = copy_to_user(buffer, buff, len);
	if (ret)
		return -EFAULT;
	pos ++;
	if (pos == STORAGE_SIZE) {
		endRead = 1;
	}
	return len;
}

ssize_t stred_write(struct file *pfile, const char __user *buffer, size_t length, loff_t *offset) 
{
	char buff[BUFF_SIZE];
	char str_in[STORAGE_SIZE];
	int ret;
	int trunc = 0;
	char * needle_p;
	char str_help[STORAGE_SIZE];
	int i;
	int j;

	ret = copy_from_user(buff, buffer, length);
	if (ret)
		return -EFAULT;
	buff[length-1] = '\0';

	//error handling for too large input
	if (length + strlen(storage) <= STORAGE_SIZE){
		//sscanf(buff,"%s", str_in);
		strcpy(str_in, buff);
	}
	else{
		printk(KERN_ERR "Not enough space for your input.\n");
		return -EFAULT;
	}


	//there are only few possibilities, so this won't be hard
	if (!strncmp(str_in, mods[0], mods_len[0])){
		strcpy(storage, str_in + mods_len[0]);//copy string into storage, without mod
		//not required if clear used, but just in case..
		for (i = strlen(storage); i < STORAGE_SIZE; i++){
			storage[i] = 0;
		}
		printk(KERN_INFO "String stored correctly.\n");
	}
	else if (!strncmp(str_in, mods[1], mods_len[1])){
		//init storage
		for (i = 0; i < STORAGE_SIZE; i++)
			storage[i] = 0;
		printk(KERN_INFO "String removed successfully.\n");
	}
	else if (!strncmp(str_in, mods[2], mods_len[2])){
		//stop on first not-space
		for (i = 0; i < STORAGE_SIZE; i++){
			if (storage[i] != ' ')
				break;
		}
		//remove spaces
		strcpy(storage, storage+i);
		//remove spaces
		for (i = strlen(storage) - 1; i >= 0; i--){
			if (storage[i] == ' '){
				storage[i] = 0;
			}
			else{
				break;
			}
		}
		printk(KERN_INFO "Spaces eliminated successfully.\n");
	}
	else if (!strncmp(str_in, mods[3], mods_len[3])){
		strcat(storage, str_in + mods_len[3]);//append string, without mod
		printk(KERN_INFO "Substrings appended successfully.\n");
	}
	else if (!strncmp(str_in, mods[4], mods_len[4])){
		//string to int, with error handling
		ret = !kstrtoint(str_in + mods_len[4], 10, &trunc);
		if (ret == 0){
			printk(KERN_ERR "Truncate length not valid.\n");
			return -EFAULT;
		}
		if (trunc > strlen(storage)){
			printk(KERN_ERR "Truncate length too large.\n");
			return -EFAULT;
		}
		printk(KERN_INFO "Truncate length: %d\n", trunc);
		for (i = 1; i <= trunc; i++){
			storage[strlen(storage) - 1] = 0;
		}
		printk(KERN_INFO "String truncated successfully.\n");
	}
	else if (!strncmp(str_in, mods[5], mods_len[5])){
		ret = strlen(str_in);
		strcpy(str_in, str_in + mods_len[5]);
		for (i = strlen(str_in); i < ret; i++){
			str_in[i] = 0;
		}
		j = strlen(storage);
		ret = strlen(str_in);
		while(1) {
			needle_p = strstr(storage, str_in);
			if (needle_p){
				for(i = strlen(str_help) - 1; i >= 0; i--){
					str_help[i] = 0;
				}
				strncpy(str_help, storage, needle_p - storage);
				strcat(str_help, needle_p + ret);
				strcpy(storage, str_help);
			}
			else {
				break;
			}
		}
		for (i = strlen(storage); i < j; i++){
			storage[i] = 0;
		}
		printk(KERN_INFO "Substrings removed successfully.\n");
	}
	else{
		printk(KERN_ERR "Invalid input.\n");
		return -EFAULT;
	}
	return length;
}

static int __init stred_init(void)
{
	int ret = 0;
   	int i = 0;

	//init storage
	for (i = 0; i < STORAGE_SIZE; i++)
		storage[i] = 0;
	//init mods
	strcpy(mods[0], "string=");
	strcpy(mods[1], "clear");
	strcpy(mods[2], "shrink");
	strcpy(mods[3], "append=");
	strcpy(mods[4], "truncate=");
	strcpy(mods[5], "remove=");
	//init mods_len (faster than to do strlen every single time)
	mods_len[0] = 7;
	mods_len[1] = 5;
	mods_len[2] = 6;
   	mods_len[3] = 7; 
	mods_len[4] = 9; 	
	mods_len[5] = 7;
		
	ret = alloc_chrdev_region(&my_dev_id, 0, 1, "stred");
   	if (ret){
   		printk(KERN_ERR "failed to register char device\n");
      		return ret;
   	}
   	printk(KERN_INFO "char device region allocated\n");

   	my_class = class_create(THIS_MODULE, "stred_class");
   	if (my_class == NULL){
      		printk(KERN_ERR "failed to create class\n");
      		goto fail_0;
   	}
   	printk(KERN_INFO "class created\n");
   
   	my_device = device_create(my_class, NULL, my_dev_id, NULL, "stred");
   	if (my_device == NULL){
      		printk(KERN_ERR "failed to create device\n");
      		goto fail_1;
   	}
   	printk(KERN_INFO "device created\n");

	my_cdev = cdev_alloc();	
	my_cdev->ops = &my_fops;
	my_cdev->owner = THIS_MODULE;
	ret = cdev_add(my_cdev, my_dev_id, 1);
	if (ret){
      		printk(KERN_ERR "failed to add cdev\n");
		goto fail_2;
	}
   	printk(KERN_INFO "cdev added\n");
   	printk(KERN_INFO "Hello world\n");

   	return 0;

   	fail_2:
      		device_destroy(my_class, my_dev_id);
   	fail_1:
      		class_destroy(my_class);
   	fail_0:
      		unregister_chrdev_region(my_dev_id, 1);
   	return -1;
}

static void __exit stred_exit(void)
{
   	cdev_del(my_cdev);
   	device_destroy(my_class, my_dev_id);
   	class_destroy(my_class);
    	unregister_chrdev_region(my_dev_id,1);
   	printk(KERN_INFO "Goodbye, cruel world\n");
}


module_init(stred_init);
module_exit(stred_exit);
