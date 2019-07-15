

#include "fs.h"
#include "string.h"
#include "f_shell.h"
/* static memory manager */
static struct shell_cmd dynamic_shell[20];
static unsigned char position = 0;
/* static backup back */
static struct shell_cmd * p_timer_link_base_backup[7] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL};
static unsigned char task_mark_sermphone = 0;
static struct shell_cmd *b_base = NULL; 
static struct shell_cmd * p_idle;
static unsigned char semi_p_idle; 
/* heap init */
void shell_stack_init(void)
{
	memset(dynamic_shell,0,sizeof(dynamic_shell));
	memset(&position,0,sizeof(position));
	memset(&p_timer_link_base_backup,0,sizeof(p_timer_link_base_backup));
	memset(&task_mark_sermphone,0,sizeof(task_mark_sermphone));
	memset(&b_base,0,sizeof(b_base));
	p_idle = 0;
	semi_p_idle = 0;
}
/* static functions */
struct shell_cmd * shell_find(const char * shell_cmd)
{
	struct shell_cmd * p_shell,*p_start;;
	/* get shell start */
	p_shell = shell_node_valid();

	if(p_shell == (void*)0)
	{
	  /* get nothing */
	  return (void*)0;
	}
	/* search all node */
	for(p_start = p_shell ; p_start != (void*)0 ; p_start = p_start->i_peer)
	{ 
		if( strcmp(shell_cmd,p_start->cmd) == 0)
		{
			/* good ! we found it */
			return p_start;
		}
	}
	/* search all node */
	for(p_start = p_shell ; p_start != (void*)0 ; p_start = p_start->i_child)
	{ 
		if( strcmp(shell_cmd,p_start->cmd) == 0)
		{
			/* good ! we found it */
			return p_start;
		}
	}
	return (void*)0;
}
struct shell_cmd * dynamic_find(const char * shell_cmd , unsigned char * index)
{
	struct shell_cmd * p_shell,*p_start;
	/* void * p */
	void * pt = 0;
	/* we found */
	/* get shell start */
	for( int i = 0 ; i < 7 ; i ++ )
	{ 
		pt = base_found(i);
    /*-------------*/
		if( pt == (void*)0 )
		{
			/* get nothing */
			return NULL;
		}
		/* get */
		p_shell = *(( struct shell_cmd ** )pt);
		/* search all node */
		for(p_start = p_shell ; p_start != (void*)0 ; p_start = p_start->i_peer)
		{ 
			if( strcmp(shell_cmd,p_start->cmd) == 0)
			{
				/* good ! we found it */
				*index = i;
				return p_start;
			}
		}
		/* search all node */
		for(p_start = p_shell ; p_start != (void*)0 ; p_start = p_start->i_child)
		{ 
			if( strcmp(shell_cmd,p_start->cmd) == 0)
			{
				/* good ! we found it */
				*index = i;
				return p_start;
			}
		}
  }
	return (void*)0;
}

int shell_write(const char * cmd,void *data,unsigned int size,unsigned int offset)
{
	 struct shell_cmd * p_shell;
	 unsigned char * p_t;
	 /* find the cmd */
	 p_shell = shell_find(cmd);
	 /* is NULL ? */
	 if( p_shell == (void*)0 )
	 {
		  return FS_ERR;//can not find the cmd 
	 }
	 
	 p_t = (unsigned char * )p_shell->enter + offset;
	 
	 memcpy(p_t,data,size); 
	 
	 return FS_OK; 
}

int shell_read(const char * cmd,void *data,unsigned int size,unsigned int offset)
{
	 struct shell_cmd * p_shell;
	 unsigned int  p_t;
	 /* find the cmd */
	 p_shell = shell_find(cmd);
	 /* is NULL ? */
	 if( p_shell == (void*)0 )
	 {
		  return FS_ERR;//can not find the cmd 
	 }
	 p_t = (unsigned int )p_shell->enter + offset;
	 
	 memcpy(data,&p_t,size); 
	 
	 return FS_OK; 
}
	
int shell_execute(const char * cmd,unsigned int param)
{
	struct shell_cmd * p_shell;
	unsigned int  p_t;
	int(*app)(unsigned int);
	/* find the cmd */
	p_shell = shell_find(cmd);
	/* is NULL ? */
	if( p_shell == (void*)0 )
	{
	  return FS_ERR;//can not find the cmd 
	}
	p_t = (unsigned int )p_shell->enter;

	app = (int(*)(unsigned int))p_t;

	p_t = app(param);

	return p_t;
}
/*
  Reuse the idle resources in the system
*/
static int dynamic_memory_recovery(struct shell_cmd * p_shell)
{
	/* LOCK */
	if( semi_p_idle )
	{
		return FS_ERR;
	}
	/* GET LOCK */
	semi_p_idle = 1;
	/* RELEASE */
	struct shell_cmd * b = p_idle;
	/* protect from stack over flow */
	unsigned short protect_count = 200;//
	/* break the original link */
	p_shell->i_peer = 0;
	p_shell->i_child = 0;
	/* first time ? */
	if( p_idle != 0 )
	{
		/* searching */
		 while(protect_count--)
		 {
			  /* find the tail of linking */
			  if( b->i_peer == 0 )
				{
					/* insert to the tail */
					b->i_peer = p_shell;
					/* bresk */
					break;
				}
				/* useless */
				b = b->i_peer;
		 }
	}
	else
	{
		p_idle = p_shell;
	}
	/* RELEASE */		
	semi_p_idle = 0;
	/* return ok */
	return FS_OK;
}
/*
 Reuse the idle resources in the system
*/
static struct shell_cmd * dynamic_mem_allocate(void)
{
	/* LOCK */
	if( semi_p_idle )
	{
		return NULL;
	}
	/* GET LOCK */
	semi_p_idle = 1;
	/* RELEASE */	
	struct shell_cmd * t;
	/* move the header to next node */
	if( p_idle != 0 )
	{
		/* Judge if it's the last one */
		if( p_idle->i_peer != 0 )
		{
			/* not the last one */
			t = p_idle;
			/* move */
			p_idle = p_idle->i_peer;
			/* RELEASE */		
			semi_p_idle = 0;			
			/* return the valid black */
			return t;
		}
		else
		{
			/* move */
			t = p_idle;
			/* reset */
			p_idle = 0;
			/* RELEASE */		
			semi_p_idle = 0;	
			/* return the valid black */			
			return t;
		}
	}
	/* RELEASE */		
	semi_p_idle = 0;	
	/* return fail */
	return NULL;
}
/* create a task */
int shell_create_dynamic( char * id , void * task , unsigned int freq )
{
	/* ids */
	struct shell_cmd * p_shell = NULL;
	struct shell_cmd **base;
	/* task index */
	unsigned char index;
	int ret;
	/* max to six*/
	if( freq > 6 )
	{
		return FS_ERR;//not supply this 
	}
	/* find the task name */
	p_shell = dynamic_find(id,&index);
	/* get base */
	base = ( struct shell_cmd ** )base_found(freq);	
	/* empty */
	if( p_shell != NULL )
	{
		 return FS_ERR;//can not insert
	}
	/* GET FREE BLACKS */
	p_shell = dynamic_mem_allocate();
	/* Judge if available */
	if( p_shell == NULL )
	{
		/* Judge if over flow */
		if( ( position ) >= ( sizeof(dynamic_shell) / sizeof(dynamic_shell[0]) ) )
		{
			return FS_ERR;
		}
		/* available */
		p_shell = &dynamic_shell[position];
		/* intermeter */
		position++;
 	}
	/* equt */
	p_shell->cmd = id;
	p_shell->enter = task;
	p_shell->size = 4;
	p_shell->it_type = _CB_TIMER_;
	p_shell->i_peer = (void *)0;
	p_shell->i_child = (void *)0;
	/*-----*/
	ret = system_shell_insert(base,p_shell);
  /* return */
	return ret;
}
/* delete a task */
int shell_delete_dynamic( const char * id , unsigned char mode )
{
	/* ids */
	struct shell_cmd * p_shell = NULL;
	struct shell_cmd **base;
	/* task index */
	unsigned char index;
	/* find the task name */
	p_shell = dynamic_find(id,&index);
	/* get base */
	base = ( struct shell_cmd ** )base_found(index);	
	/* found? */
	if( p_shell != NULL )
	{
		if( p_shell->i_peer == NULL )
		{
			/* end of the linker */
			 if( p_shell->i_child != NULL )
			 {
				 *base = p_shell->i_child;
				 p_shell->i_child->i_peer = p_shell->i_peer;
			 }else
			 {
				 *base = NULL;
			 }
		}else
		{
			 /* middle of the linker */
			 p_shell->i_child->i_peer = p_shell->i_peer;
			 p_shell->i_peer->i_child = p_shell->i_child;
		}
		/* memory recovery */
		dynamic_memory_recovery(p_shell);
	}
	return FS_OK;
}
/* revert a task */
int shell_revert_dynamic( const char * id )
{
	/* ids */
	struct shell_cmd * p_shell = NULL;
	struct shell_cmd **base;	
	/* task index */
	unsigned char index;
	/* find the task name */
	p_shell = dynamic_find(id,&index);
	/* get base */
	base = ( struct shell_cmd ** )base_found(index);	
  /* insert */
  return system_shell_insert(base,p_shell);	
}
/* disable all task */
int shell_disable_dynamic(unsigned char index)
{
	struct shell_cmd **base;	
	/* release */
	if( task_mark_sermphone )
	{
		return FS_ERR;
	}
	/* get base */
	base = ( struct shell_cmd ** )base_found(0);
	/* ------------------ */
	for( int i = 0 ; i < 7 ; i ++ )
	{
		 if( index & ( 1 << i ) )
		 {
			  p_timer_link_base_backup[i] = base[i];
			  base[i] = NULL;
		 }
	}
	task_mark_sermphone = 1;
	/* -------- return --------*/
	return FS_OK;
}
/* enable all task */
int shell_enable_dynamic(unsigned char index)
{
	struct shell_cmd **base;
	/*-----------------*/
	if( !task_mark_sermphone )
	{
		return FS_ERR;//
	}
	/* get base */
	base = ( struct shell_cmd ** )base_found(0);
	/* for each others */
	for( int i = 0 ; i < 7 ; i ++ )
	{
		 if( index & ( 1 << i ) )
		 {
			  if( p_timer_link_base_backup[i] != NULL )
				{
					base[i] = p_timer_link_base_backup[i];
				}
		 }
	}
	/*--------------*/
  task_mark_sermphone = 0;
  /* return */
  return FS_OK;	
}
/* time base found */
static void * base_found(unsigned int index)
{
	struct shell_cmd * p_shell;
	/* format supply ? */
	if( index >=16 )
	{
		return NULL;
	}
	/* get */
	if( b_base == NULL )
	{
		/* get base */
		p_shell = shell_find("p_timer_link_base");
		/* found ? */
		if( p_shell != NULL )
		{
			b_base = p_shell->enter;
		}else
		{
			return NULL;
		}
	}
	/*-----------*/
	return ( (int *)b_base + index );
}
/* shell insert */
int system_shell_insert(struct shell_cmd ** p_link,struct shell_cmd * p_shell)
{
	if(*p_link != NULL)
	{
		 (*p_link)->i_peer = p_shell;
		 p_shell->i_child = *p_link;
		 *p_link = p_shell;
	}else
	{
		 *p_link = p_shell;
	}
	return FS_OK;
}



































