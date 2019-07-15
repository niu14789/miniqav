/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : notify.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
	* BEEP TIM3 CHANNEL1 PWM Gerente
	* LED is TIM4 CH3 and CH4
  */
/* USER CODE END Header */
#include "fs.h"


/****************************************************************************
 * Name: _inode_compare
 *
 * Description:
 *   Compare two inode names
 *
 ****************************************************************************/

static int _inode_compare(FAR const char *user_path,//path longer
                          FAR char *node_path) //inode path
{
   char *nname = node_path;

   if (!nname)
    {
      return 1;
    }

   if (!user_path)
    {
      return -1;
    }

    while( (*nname == *user_path) || ((*nname == '\0')) )
    {
        if(*nname == '\0')
            return FS_OK;

        nname++;
        user_path++;
    }
    return *nname - *user_path;
}

struct file * open(const char *path, int oflags)
{ 
	/* some defaults */
  struct inode * p_inode,*p_start;
  struct file filp,*filp_return;
	inode_vmn * o_get;
  /*
   * get inode sched getfiles
   * p vmn start
   * nothing
   *
   * */
	o_get = inode_valid();
	/*got nothing */
	if( o_get == (void*)0)
	{
		/* got nothing */
		return (void*)0;
	}
	/*--------------------------*/
  p_inode = o_get->inode;
  /* peer */
	for(p_start = p_inode;p_start != (void*)0;p_start = p_start->i_peer)
  {
		if(_inode_compare(path,p_start->inode_name) == FS_OK)
		{
			filp.f_path = (char *)path;
			filp.f_oflags = oflags;
			if(p_start->ops.open != (void*)0)
			{
				filp_return = p_start->ops.open(&filp);
				return filp_return;
			}else
			{
				return &p_start->flip;
			}
		}
	}
	/* child */
	for(p_start = p_inode;p_start != (void*)0;p_start = p_start->i_child)
  {
		if(_inode_compare(path,p_start->inode_name) == FS_OK)
		{
			filp.f_path = (char *)path;
			filp.f_oflags = oflags;
			if(p_start->ops.open != (void*)0)
			{
				filp_return = p_start->ops.open(&filp);
				return filp_return;
			}else
			{
				return &p_start->flip;
			}
		}
	}
  return (void*)0;
}
/* end of files */















