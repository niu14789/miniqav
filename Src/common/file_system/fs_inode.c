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
#include "f_shell.h"
/****************************************************************************
 * Private Functions
 ****************************************************************************/
/* get valid inode */
FAR inode_vmn *inode_valid(void)
{
	int i;
	int inode_max;
	/* get it */
	inode_vmn * p_valid = (inode_vmn *)&fs_vmn$$Base;
	/* get max num */
	inode_max = inode_sched_limit();
	/* search */
	for( i = 0 ; i < inode_max ; i ++ )
	{
		if( p_valid->inode->i_peer != (void*)0 || p_valid->inode->i_child != (void*)0)
		{
			/* ok we got it*/
			return  p_valid;
		}
		p_valid++;
	}
	/* return ERROR */
	return (void*)0;
}
/* get valid inode */
FAR struct shell_cmd * shell_node_valid(void)
{	
	int i;
	int shell_max;
	/* buffer */
	struct shell_cmd * p_shell;
	/* get it */
	inode_vmn * p_valid = (inode_vmn *)&fs_vmn$$Base;
	/* get max num */
	shell_max = shell_sched_limit();
	/* get the base */
	p_shell = p_valid->inode->shell_i;
	/* search */
	for( i = 0 ; i < shell_max ; i ++ )
	{
		if( p_shell->i_peer != (void*)0 || p_shell->i_child != (void*)0)
		{
			/* ok we got it*/
			return  p_shell;
		}
		p_shell++;
	}
	return (void*)0;
}
/*
 get head of inode
*/
FAR inode_vmn *inode_sched_getfiles(void)
{
	return (inode_vmn *)&fs_vmn$$Base;
}
/*
  get length of inode
*/
FAR int inode_sched_limit(void)
{
	return (&fs_vmn$$Limit-&fs_vmn$$Base);
}
/*
   get shell head
*/
FAR struct shell_cmd *shell_sched_getfiles(void)
{
	return (struct shell_cmd *)&fs_shell$$Base;
}
/*
  get length of shell
*/
FAR int shell_sched_limit(void)
{
	return (&fs_shell$$Limit-&fs_shell$$Base);
}
/* init all of inodes */
int system_initialization(void)
{
	/* heap init */
	shell_stack_init();
  /* as start */
	inode_vmn * p_vmn_start = inode_sched_getfiles();
	/* dectecte the node */
	if(p_vmn_start != (void*)0)
	{
			p_vmn_start->inode->max.inode_max = inode_sched_limit();
			p_vmn_start->inode->max.shell_max = shell_sched_limit();
		  p_vmn_start->inode->shell_i = shell_sched_getfiles();
	}
	/* reutrn OK */
	return FS_OK;
}
/* end of file */











