
/* header */
#include "cmsis_os.h"
#include "fs.h"
#include "f_shell.h"
#include "string.h"
#include "f_ops.h"
#include "fs_allocate.h"
/* default the root pointer */
static inode_vmn * p_vmn_link_header = (void*)0;
/* some defes */
static inode_vmn * system_runtime = 0;
/* callback */
callback_function isr[64];
/* system insert */
static int system_insert(inode_vmn * p_vmn)
{
	/* ignore first time */
	if(p_vmn_link_header != (void*)0)
	{
			// some default
			struct inode * base;
			/* judge */
			for( base = p_vmn_link_header->inode ; base != 0 ; base = base->i_peer )
			{
					//zero
					if( base->i_child == 0 )
					{
						// from start
						if( p_vmn->inode->i_crefs > base->i_crefs )
						{
							 p_vmn->inode->i_peer = base;
							 base->i_child = p_vmn->inode;
							 p_vmn->inode->i_child = 0;
							 p_vmn_link_header = p_vmn;
							 /* break */
							 break;
						}
					}
					/*-------------------*/
					if( base->i_peer == 0 )
					{
						//last one
						base->i_peer = p_vmn->inode;
						p_vmn->inode->i_child = base;
						p_vmn->inode->i_peer = 0;
						/*----------------------------*/
						break;
						/*----------------------------*/
					}else
					{
						if( p_vmn->inode->i_crefs <= base->i_crefs && p_vmn->inode->i_crefs > base->i_peer->i_crefs )
						{
							 base->i_peer->i_child = p_vmn->inode;
							 p_vmn->inode->i_child = base;
							 p_vmn->inode->i_peer = base->i_peer;
							 base->i_peer = p_vmn->inode;
							 /* break */
							 break;
						}
					}
			}
	}
	else
	{
		p_vmn_link_header = p_vmn;
	}
	return 0;
}
/* search inode */
static inode_vmn * inode_search(unsigned int base , unsigned int size , unsigned int *base_offset)
{
		inode_identify *id_enter = 0;
		/* ----------------- */
		inode_vmn * p = NULL;
		/*-------------------*/
		int i = *base_offset;	
		/* get the base addr */
		id_enter = (inode_identify *)(base + i);
		/*--------------------------*/
		unsigned int * end_file_dec = (unsigned int *)(base + i);
		/*--------------------------*/
		/* search loop */
		for( ; i < size ; i += 4 )
		{
				if(id_enter->id_l == 0xAFBC562D && 
					 id_enter->id_h == 0x2b74113c)
				{
						/* check */
						if( (unsigned int)id_enter->entrance > base &&
							  (unsigned int)id_enter->entrance < ( base + size ) )
						{
							/* ------------ */
							i += 4;
							/*--------------*/
							p = (inode_vmn *)(((int(*)(void))id_enter->entrance)());
							/*--------------*/
							*base_offset = i;
							/*--------------*/
							return p;							
						}
				}
				/* incremter */
				id_enter = (inode_identify *)(((unsigned int)id_enter) + 4);
				/* end file decateltes */
				if( end_file_dec[0] == 0xF1F2F3F4 && 
					  end_file_dec[1] == 0xE1E2E3E4 && 
				    end_file_dec[2] == 0xD1D2D3D4 )
				{
					/* end of file */
					return NULL;
				}
				/*---------------------*/
				end_file_dec ++;
				/*---------------------*/
		}
		/* cannot found */
		return NULL;
		/*--------------*/	
}
static inode_vmn * inode_start(int seq)
{
	const unsigned int base[][2] = 
  {
		{ALLOCATE_RAM_ROM_START_ADDR,ALLOCATE_RAM_ROM_SIZE},
		{0x480000,0x20000},
		{0x460000,0x20000},
#if 0
		{0x490000,20*1024},
#endif		
	};
	/*-------------------*/
	if( seq == 0 )
	{
		  /* define the start inode */
			inode_vmn * p_vmn_start = inode_sched_getfiles();
			/*
			* set the default setting
			*/
			if(p_vmn_start != (void*)0)
			{ 
					p_vmn_start->inode->max.inode_max = inode_sched_limit();
					p_vmn_start->inode->max.shell_max = shell_sched_limit();
					p_vmn_start->inode->shell_i = shell_sched_getfiles();
			}
			/* ok find */
			return p_vmn_start;//inode of base files end of the file
  }
	else if( seq >= 1 )
	{   
		  /* initial some tasks */
		  static unsigned int cnt = 0;
		  static unsigned char step = 0;
		  /* start the gate */
		  inode_vmn * p = inode_search(base[step][0],base[step][1],&cnt);
		  /* ok ? */
		  if( p == NULL )
			{
				//clear the tourism
				cnt = 0;
				/* switch the base */
				step++;
				/*-----------------*/
				if( step >= sizeof(base)/sizeof(base[0]))
				{
					return NULL;//It's over
				}
				/* switch the base addr */
				return inode_search(base[step][0],base[step][1],&cnt);
			}				
		  /* return p */
			return p;
	}
	else
	{
		  return NULL;
	}
}
/* config */
static int config_inode(void)
{
  struct inode * p_inode,*p_start;
	/*---------------------------*/
  if( p_vmn_link_header != NULL )
	{
		 p_inode = p_vmn_link_header->inode;
	}
	else
	{
		 /* everyone are NULL ,return */
		return FS_ERR;
	}
    /* peer */
	for(p_start = p_inode;p_start != (void*)0;p_start = p_start->i_peer)
	{
		if(p_start->config != NULL)
		{
			p_start->config();
		}
	}
	/* child */
	for(p_start = p_inode->i_child;p_start != (void*)0;p_start = p_start->i_child)
	{
		if(p_start->config != NULL)
		{
			p_start->config();
		}		
	}
  	return FS_OK;
}
/* init config */
static int heap_init_inode(void)
{
   struct inode * p_inode,*p_start;;
   /*
    * get inode sched getfiles
    * p vmn start
    * nothing
    *
    * */
	/* is ? */
	if(p_vmn_link_header != NULL)
	{
		 p_inode = p_vmn_link_header->inode;
	}
	else
	{
		/* everyone are NULL ,return */
		return FS_ERR;
	}
	/* peer */
	for(p_start = p_inode;p_start != (void*)0;p_start = p_start->i_peer)
	{
		 // judge end?
		if( p_start->i_private != 0 )
		{
			/* It should have some tips and protects */
			/* save of linker */
			struct inode backup;
			/* copy */
			memcpy(&backup,p_start,sizeof(backup));
			/*----------------*/
			int ret = ((int(*)(void))p_start->i_private)();
			/* 
			   reset some data 
			*/
		  p_start->i_peer = backup.i_peer;
		  p_start->i_child = backup.i_child;
		  p_start->inode_name = backup.inode_name;
		  p_start->i_private = backup.i_private;
		  p_start->i_crefs = backup.i_crefs;			
		  p_start->shell_i = backup.shell_i;
			p_start->max.inode_max = backup.max.inode_max;
			p_start->max.shell_max = backup.max.shell_max;			
			/* judge */
			if( ret!= FS_OK )
			{
				 //delete this node
				 if( p_start->i_child == 0 )
				 {
					 if( p_start->i_peer != 0 )
					 {
						 p_start->i_peer->i_child = 0;
						 p_vmn_link_header->inode = p_start->i_peer;
						 p_start->i_peer = 0;
						 /* break*/
						 continue;
						 /*------------------------*/
           }
					 else
           {
						 //may be unreachable
           }	
				 }
				 /* tail */
				 if( p_start->i_peer == 0 )
				 {
					  p_start->i_child->i_peer = 0;
					  p_start->i_child = 0;
					  /* break*/
					  continue;
					  /*------------------------*/					 
				 }
				 else
				 {
					  struct inode * tmp_child,*tmp_ipeer;
					  /* */
					  tmp_child = p_start->i_child;
					  tmp_ipeer = p_start->i_peer;
					  /* link them */
					  tmp_child->i_peer = tmp_ipeer;
					  tmp_ipeer->i_child = tmp_child;
						/* break*/
						continue;
						/*------------------------*/					 
				 }
			}		
		}		 
	}
	/* get the higher and insert the shell node */
	struct shell_cmd * shell_i = 0;
	/* int inode */
  for(p_start = p_inode;p_start != (void*)0;p_start = p_start->i_peer)
  {	
		 if( p_start->shell_i != 0 && ((((unsigned int)p_start->shell_i)>>20) == 0x204) && p_start->shell_i != shell_i )
		 {
			 shell_i = p_start->shell_i;
			 /* init */
			 if( system_runtime != 0 )
			 {
				 fs_ioctl(&system_runtime->inode->flip,0,p_start->max.shell_max,shell_i);
			 }
		 }
  }
  /*----------*/
  return FS_OK;
}
/* previous open */
static int fs_previous_open(inode_vmn * pfo)
{
	/* first of all */
	if( strcmp(pfo->path , "/runtime.o") == 0 )
	{
		 /* 
		  previous initial the node to get the handle
		  got it 
		*/
		 system_runtime = pfo;
	}
	/* return */
	return FS_OK;
}
/*0---------------------------------------*/
int fs_system_initialization(void)
{
  int i,inode_max = 0,seq = 0;	
  inode_vmn * base = NULL;
	/* infint loop */
	while(1)
	{
		/* get the base address */
		 base = inode_start(seq);
		/* is NULL */ 
		 if( base != NULL )
		 {
			 inode_max = base->inode->max.inode_max;
		 }else
		 {
			 if(seq)
			 {
				  /* config heap */
				  heap_init_inode();
					/* config init */
					config_inode();
					/* --------- */
					 shell_execute("system_start",0xfff);
					/* -- start system -- */
					return 0;
			 }
			 else
			 {
			    seq++;
			    continue;
			 }
		 }
		 /* init inode init() */
		 for( i = 0 ; i < inode_max ; i++ )
		 {
				if( base->init != NULL )
				{
						/* inode init ok init the inode name */
						base->inode->inode_name = base->path;
						/* init the node seq */
						base->inode->i_crefs = base->seq & 0xff;
						/* init the node init */
						base->inode->i_private = base->init;
					  /* previous open */
					  fs_previous_open(base);
						/* inode insert to base linker */
						system_insert(base);
						/*--------------------------------------------------*/
						printf_d("the device init ok at path:%s\n",base->path);
						/*----------------------------------*/
				}
				base++;
		 }
		 /* next node */
     seq++;
   }
}
/* for test */
void task_run(int task_id)
{
	 /* init */
	 if( system_runtime != 0 )
	 {
		 fs_ioctl(&system_runtime->inode->flip,1,task_id,0);
	 }	
}








