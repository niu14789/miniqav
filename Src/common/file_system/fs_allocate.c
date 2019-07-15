/*
  headers
*/
#include "fs.h"
#include "string.h"
#include "f_ops.h"
#include "f_shell.h"
#include "fs_allocate.h"
#include "board.h"
/* already allocated position */
static unsigned int alread_allocated_ram_stack_addr = ALLOCATE_RAM_STACK_BASE;
static unsigned int alread_allocated_ram_rom_addr   = ALLOCATE_RAM_ROM_START_ADDR;
static unsigned int *allocate_write_addr            = (unsigned int *)ALLOCATE_RAM_ROM_START_ADDR;
static unsigned int current_stack_size = 0;
static unsigned int current_ok_size = 0;
/* file config msg that read from .sh files */
static allocate_config_def * allocate_config = (allocate_config_def *)(ALLOCATE_RAM_ROM_END_ADDR - 1024);//1kb
/*--------------*/
#define ALLOCATE_RAM_WRITE(a)  {*allocate_write_addr = a ;	allocate_write_addr ++ ; current_ok_size += 4 ; }
/* static param */
static unsigned char step = 0;
/* some declides */
int allocate_init(void);
int allocate_start(struct file * f,config_def * config);
int allocate_write( unsigned int * b , unsigned int len );
/* fs inode system register */
FS_INODE_REGISTER("/allocate/",allocate,allocate_init,190);
/* heap init */
int allocate_init(void)
{
	/* full of zero */
	memset(&allocate,0,sizeof(allocate));
	/* shell init */
	allocate.shell_i = shell_sched_getfiles();
	allocate.config = NULL;
	/* file init */
	allocate.flip.f_path = "/allocate/";
	/* add you own code here */
	current_stack_size = 0;
	current_ok_size = 0;
	/* has already run ? */
	if( allocate_config->head == RAM_ID_HEAD &&
		  allocate_config->tail == RAM_ID_TAIL)
	{
		/* ok . we have already run . then clear the marks */
		allocate_config->head = 0;
		allocate_config->tail = 0;
		/* run them */
		return FS_OK;
	}
	/* open dev */
	struct file * fp = open("/storage/",__FS_CREATE_ALWAYS|__FS_WRITE);
	/* judge ok ? */
	if( fp == NULL )
	{
		return FS_OK;
	}
	/* some defines */
	readdir_entrance_def red;
	/* make structure of readdir */
	red.enter = allocate_config->config[0].buffer;
	red.length = sizeof(allocate_config->config[0]);
	red.num = sizeof(allocate_config->config) / sizeof(allocate_config->config[0]);
	/* read dir */
	volatile int num = fs_readdir(fp,"/storage/emmc/shell",&red);
	/* ok */
	if( num == FS_ERR || num == 0 )
	{
		 return FS_OK;// no file 
	}
	/* read config file */
	//allocate_config
	/* start */
	for( int i = 0 ; i < num ; i ++ )
	{
			/* set filter for *.ok . simple procotol */
			if( strstr(allocate_config->config[i].buffer,".ok") == NULL )
			{
				continue;//doesn't support this format
			}
			/* upgrade ROM and all ok files */
			struct file * fip = open(allocate_config->config[i].buffer,__FS_OPEN_EXISTING|__FS_READ);
			/* clear the stack size and firware size */
			current_stack_size = 0; 
			current_ok_size = 0;
			/* get current file size */
			volatile unsigned int size = fs_ioctl(fip,2,0,0);
			/* judge wherin less than 20KB */
			if( size % 4 != 0 || size > ALLOCATE_RAM_ROM_SIZE )
			{
				/* file size or align error . skip it */
				continue;//doesn't support this format
			}
			/* starting */
			if( allocate_start(fip,&allocate_config->config[i]) == FS_OK )
			{
					/* 
						one file has been allocated 
						file size equce with recorded .
						reallocate the stack and rom
					*/
					alread_allocated_ram_rom_addr   += current_ok_size;
					alread_allocated_ram_stack_addr += current_stack_size;
					allocate_write_addr = (unsigned int *)alread_allocated_ram_rom_addr;
					/*
						stack over flow
					*/
					if( alread_allocated_ram_stack_addr > ALLOCATE_RAM_STACK_END_ADDR )
					{
						/* stack over flow */
						//pintf to file
						return FS_ERR;
					}
					/*
						flash over flow
					*/
					if( alread_allocated_ram_rom_addr > ALLOCATE_RAM_ROM_END_ADDR )
					{
						/* stack over flow */
						//pintf to file
						return FS_ERR;								
					}
			}
			else
			{
				/* There is one file allocated fail */
				//pintf to file
				return FS_ERR;
			}
	}
	/* has already allocated */
	if( alread_allocated_ram_rom_addr == ALLOCATE_RAM_ROM_START_ADDR )
	{
		return FS_OK;//haven't allocated
	}
	/* done . write tail message into file */
	ALLOCATE_RAM_WRITE(0xF1F2F3F4);
	ALLOCATE_RAM_WRITE(0xE1E2E3E4);
	ALLOCATE_RAM_WRITE(0xD1D2D3D4);
	/* delete the file or set some marks */
	allocate_config->head = RAM_ID_HEAD;
	allocate_config->tail = RAM_ID_TAIL;
	/* delete files ? */
	NVIC_SystemReset();
	/*-----------------------------------*/
	return FS_OK;
}
/*-------------------------*/
int allocate_start(struct file * f,config_def * config)
{
	unsigned int buffer[64];//read buffer
	/* choose type of mode */
#if 0 // may be supported after this version.
	if( 1 )//default mode , ram mode
	{
		
	}
#endif
  while(1)
  {
	  /* read date from .ok files */
	  int len = fs_read(f,(char *)buffer,sizeof(buffer));
	  /* starting */
	  if( allocate_write( buffer , len / 4 ) != FS_OK )
	  {
		  return FS_ERR;//occur some error 
	  }
	  /* end */
	  if( len < sizeof(buffer) )
	  {
			/* 
			   read config file . delete or not
         may be supported at next version			
			*/
			/* delete as default */
			if( config->msg.config_enable == CONFIG_ENDBLE && config->msg.delete_flag == CONFIG_UNDELETE_ENABLE )
			{
				 /* nothing to do */
			}
			else
			{
			   fs_ioctl(f, 0, 0, (void *)config->buffer);
			}
			/*-------------------*/
		  break;
	  }
  }
	/* return OK */
	return FS_OK;
}
/*----------------*/
int allocate_write( unsigned int * b , unsigned int len )
{
	static unsigned int ram_tmp = 0 , rom_tmp = 0;
	/* switch */
	for( int i = 0 ; i < len ; i ++ )
	{
		/* search all of data */
	  switch( step )
		{
			case 0:
				/* jugding */
			  if( b[i] == RAM_ID_HEAD )
				{
					step = 1;
				}
				else if ( b[i] == ROM_ID_HEAD )
				{
					step = 2;
				}
				else
				{
					/* if ram mode . write into the ram zone directly */
          ALLOCATE_RAM_WRITE(b[i]);
					/* This should be add some flash mark */
				}
				break;
			case 1:
				/* backup the ram data */
				ram_tmp = b[i];
			  /* jump to the base */
			  step = 3;
			  /* break */
			  break;
			case 2:
				/* backup the ram data */
				rom_tmp = b[i];
			  /* jump to the base */
			  step = 4;
			  /* break */
			  break;			
			case 3:
				/* jugding */
			  if( b[i] == RAM_ID_TAIL )
				{
					/* good , ok , we got it */
					/* first time to write into the ram zone is the stack size */
					if( current_stack_size == 0 )
					{
						current_stack_size = ram_tmp;
					}					
					/* if inside the ram mode . write into the ram zone dirctly */
					ram_tmp += alread_allocated_ram_stack_addr;
					/* write into */
          ALLOCATE_RAM_WRITE(ram_tmp);
					/* This should be add some flash mark */
					step = 0;
				}
				else
        {
					/* not good . we may be lost some data , need some tips */
					step = 0;
					/* return bad */
					return FS_ERR;
				}				
        /* break */
        break;
			case 4:
				/* jugding */
			  if( b[i] == ROM_ID_TAIL )
				{
					/* good , ok , we got it */
					/* if inside the ram mode . write into the ram zone dirctly */
					rom_tmp += alread_allocated_ram_rom_addr;
					/* write into */
          ALLOCATE_RAM_WRITE(rom_tmp);
					/* This should be add some flash mark */
					step = 0;
				}
				else
        {
					/* not good . we may be lost some data , need some tips */
					step = 0;
					/* return bad */
					return FS_ERR;					
				}				
        /* break */
        break;
			default:
        /* may be unreachable */	
        step = 0;
				/* return bad */
				return FS_ERR;			
        /* break */		
		}
	}
	/* return */
	return FS_OK;
}













































