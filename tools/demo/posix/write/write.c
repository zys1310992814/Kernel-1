/*
 * POSIX system call: write
 *
 * (C) 2018.06.18 BiscuitOS <buddy.zhang@aliyun.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/kernel.h>
#include <linux/unistd.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/string.h>
#include <linux/mm.h>

#include <demo/debug.h>

static inline _syscall3(int, open, const char *, file, int, flag, int, mode);
static inline _syscall3(int, read, unsigned int, fd, char *, buf, 
                            unsigned int, count);
static inline _syscall1(int, close, int, fd);

#ifdef CONFIG_DEBUG_WRITE_ORIG
static inline _syscall3(int, write, unsigned int, fd, char *, buf, 
                            unsigned int, count);
#endif

static int write_rootfs(struct inode *inode, struct file *filp, 
                 char *buf, int count)
{
    off_t pos;
    int written, c;
    struct buffer_head *bh;
    char *p;

    if (!inode) {
        printk("minix_file_write: inode = NULL\n");
        return -EINVAL;
    }
    if (!S_ISREG(inode->i_mode)) {
        printk("minix_file_write: mode = %07o\n", inode->i_mode);
        return -EINVAL;
    }
    return 0;
}

asmlinkage int sys_demo_write(unsigned int fd, char *buf, 
                                 unsigned int count)
{
    int error;
    struct file *file;
    struct inode *inode;

    if (fd >= NR_OPEN || !(file = current->filp[fd]) ||
                         !(inode = file->f_inode)) 
        return -EBADF;
    if (!(file->f_mode & 2))
        return -EBADF;
    if (!file->f_op || !file->f_op->write)
        return -EINVAL;
    error = verify_area(VERIFY_READ, buf, count);
    if (error)
        return error;
#ifdef CONFIG_DEBUG_WRITE_ROOTFS
    return write_rootfs(inode, file, buf, count);
#else
    return file->f_op->write(inode, file, buf, count);
#endif
}

/* build a system call entry for write */
inline _syscall3(int, demo_write, unsigned int, fd, char *, buf,
                                         unsigned int, count);

/* userland code */
static int debug_write(void)
{
    int fd;
    char buf[20] = "BiscuitOS";

    fd = open("/etc/BiscuitOS.rc", O_RDWR | O_CREAT, 0);
    if (fd < 0) {
        printf("Unable open '/etc/BiscuitOS.rc'\n");
        return -1;
    }

#ifdef CONFIG_DEBUG_WRITE_ORIG
    write(fd, buf, 10);
#else
    demo_write(fd, buf, 10);
#endif
    memset(buf, 0, 10);
    read(fd, buf, 10);
    printf("Read: %s\n", buf);
    close(fd);
    return 0;
}
user1_debugcall_sync(debug_write);
