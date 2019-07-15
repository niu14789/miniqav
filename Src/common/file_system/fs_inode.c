/*
 * fs.c
 *
 *  Created on: 2016骞�5鏈�7鏃�
 *      Author: YJ-User17
 */

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


FAR inode_vmn *inode_sched_getfiles(void)
{
	return (inode_vmn *)&fs_vmn$$Base;
}

FAR int inode_sched_limit(void)
{
	return (&fs_vmn$$Limit-&fs_vmn$$Base);
}
FAR struct shell_cmd *shell_sched_getfiles(void)
{
	return (struct shell_cmd *)&fs_shell$$Base;
}

FAR int shell_sched_limit(void)
{
	return (&fs_shell$$Limit-&fs_shell$$Base);
}

int system_initialization(void)
{
	/* heap init */
	shell_stack_init();
  /* as start */
	inode_vmn * p_vmn_start = inode_sched_getfiles();
	
	if(p_vmn_start != (void*)0)
	{
			p_vmn_start->inode->max.inode_max = inode_sched_limit();
			p_vmn_start->inode->max.shell_max = shell_sched_limit();
		  p_vmn_start->inode->shell_i = shell_sched_getfiles();
	}
	
	return 0;
}
