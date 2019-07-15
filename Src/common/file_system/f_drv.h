/*
 * f_drv.h
 *
 *  Created on: 2017年5月17日
 *      Author: YJ-User17
 */

#ifndef __FS_F_DRV_H__
#define __FS_F_DRV_H__

#define drv_write( filp , type , buffer , width , size )                   \
if(filp != (void*)0 && filp->f_inode->drv_ops.write != (void*)0 )          \
{                                                                          \
	filp->f_inode->drv_ops.write(type , buffer , width , size);            \
}                                                                          \

#define drv_read( filp , type , buffer , width , size )                    \
if(filp != (void*)0 && filp->f_inode->drv_ops.read != (void*)0 )           \
{                                                                          \
	filp->f_inode->drv_ops.read(type , buffer , width , size);             \
}                                                                          \

#define drv_enable( filp , type )                                          \
if(filp != (void*)0 && filp->f_inode->drv_ops.enable != (void*)0 )         \
{                                                                          \
	filp->f_inode->drv_ops.enable(type);                                   \
}                                                                          \

#define drv_disable( filp , type )                                         \
if(filp != (void*)0 && filp->f_inode->drv_ops.disable != (void*)0 )        \
{                                                                          \
	filp->f_inode->drv_ops.disable(type);                                  \
}                                                                          \

#define drv_chipselect( filp , type , status )                             \
if(filp != (void*)0 && filp->f_inode->drv_ops.selectchip != (void*)0 )     \
{                                                                          \
	filp->f_inode->drv_ops.selectchip(type,status);                        \
}                                                                          \

#define drv_config( filp , p_arg , argc )                             \
if(filp != (void*)0 && filp->f_inode->drv_ops.config != (void*)0 )     \
{                                                                          \
	filp->f_inode->drv_ops.config( p_arg , argc );                        \
}                                                                          \

#endif /* __FS_F_DRV_H__ */
