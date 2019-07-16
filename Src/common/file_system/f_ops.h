/*
 * f_ops.h
 *
 *  Created on: 2017年5月22日
 *      Author: YJ-User17
 */

#ifndef __F_OPS_H__
#define __F_OPS_H__
/* ips */
#include "fs.h"
/* fs write */
static int fs_write(struct file * filp,const char * buffer , unsigned int buflen)  
{ 
	/* judge */
	if( filp != 0 && filp->f_inode->ops.write != 0 )
	{
		return filp->f_inode->ops.write(filp,buffer,buflen);
	}
	else
	{
		return FS_ERR;
	}
}
/* fs read */
static int fs_read(struct file * filp,char *buffer , unsigned int buflen)   
{
	/* judge */
	if( filp != 0 && filp->f_inode->ops.read != 0 )
	{
		return filp->f_inode->ops.read(filp,buffer,buflen);
	}
	else
	{
		return 0;
	}
}
/* fs sync */
static int fs_sync(struct file * filp)
{
	/* judge */
	if( filp != 0 && 
		filp->f_inode->ops.storage_dev != 0 &&
		filp->f_inode->ops.storage_dev->sync != 0 )
	{	
	  return filp->f_inode->ops.storage_dev->sync(filp);
	}
	else
	{
		return FS_ERR;
	}
}
/* fs close */
static int fs_close(struct file * filp)
{
	/* judge */
	if( filp != 0 &&
		filp->f_inode->ops.storage_dev != 0 &&
	  filp->f_inode->ops.storage_dev->close != 0 )
	{	
	  return filp->f_inode->ops.storage_dev->close(filp);
	}
	else
	{
		return FS_ERR;
	}	
}           
/* fs opendir */
static int fs_opendir( struct file * filp , const char *path )     
{
	/* judge */
	if( filp != 0 && 
		filp->f_inode->ops.storage_dev != 0 &&
		filp->f_inode->ops.storage_dev->opendir != 0 )
	{		
	  return filp->f_inode->ops.storage_dev->opendir(path);
	}
	else
	{
		return FS_ERR;
	}
}
/* fs seek */
static int fs_seek(struct file *filp, unsigned int offset, unsigned int whence)
{
	/* judge */
	if( filp != 0 && 
		filp->f_inode->ops.storage_dev != 0 &&
		filp->f_inode->ops.storage_dev->lseek != 0 )
	{		
	  return filp->f_inode->ops.storage_dev->lseek(filp,offset,whence);
	}
	else
	{
		return FS_ERR;
	}
}
/*fs ioctrl */
static int fs_ioctl(struct file *filp, int cmd, unsigned long arg,void *pri_data)
{
	/* judge */
	if( filp != 0 && filp->f_inode->ops.ioctl != 0 )
	{	
	   return filp->f_inode->ops.ioctl(filp,cmd,arg,pri_data);
	}
	else
	{
		return FS_ERR;
	}
}
/* fs readdir */
static int fs_readdir( FAR struct file *filp, const char *path,readdir_entrance_def * buffer )     
{
	/* judge */
	if( filp != 0 && 
		filp->f_inode->ops.storage_dev != 0 &&
		filp->f_inode->ops.storage_dev->readir != 0 )
	{		
	  return filp->f_inode->ops.storage_dev->readir(filp,path,buffer);
	}
	else
	{
		return FS_ERR;
	}
}
#endif /* __F_OPS_H__ */





























