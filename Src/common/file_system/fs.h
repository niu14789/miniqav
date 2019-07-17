/*
 * fs.h
 *
 *  Created on: 2016??4??19??
 *      Author: YJ-User17
 */

#ifndef __FS_H__
#define __FS_H__

/* file system config */
#define   ALLOCATE_EXAPP_SUPPLY   0  /* Whether Extended Applications are Supported */
#define   EXAPP_PREVIOUS_SUPPLY   0  /* Initialization of necessary components */
#define   EXA_TIMER_TASK_SUPPLY   0  /* Timers' task supply */
/* system printf default none */
#define printf_d(...)
/* callback function typedef */
typedef int (*callback_function)(int type,void * data,int len);

/* callback defines */
#define FS_CALLBACK_STATIC(function_name,deep)                                        \
callback_function __FS_##function_name[deep];                                         \
/* register task and export some gps message */                                       \
FS_SHELL_STATIC(__FS_##function_name,__FS_##function_name,0x040000 + deep,_CB_ARRAY_);\

/* call back insert */
#define FS_CALLBACK_INSERT(function_name,type,data,len,to)                            \
if( __FS_##function_name[to]!= 0 ) {__FS_##function_name[to](type,data,len);}         \
/* general option */
#define FAR
#define FS_INODE_USABLE       (0x0000)  /* magic */
#define FS_ERR  (-1)
#define FS_OK   (0)
#define  DEVICE_END  0x8

/* open option */
#define __FS_ONLYREAD            (0x01)
#define	__FS_READ				         (0x01)
#define	__FS_OPEN_EXISTING	     (0x00)
#define __FS_ERROR			         (0x80)

#if !_FS_READONLY
#define	__FS_WRITE			         (0x02)
#define	__FS_CREATE_NEW		       (0x04)
#define	__FS_CREATE_ALWAYS	     (0x08)
#define	__FS_OPEN_ALWAYS		     (0x10)
#define __FS_WRITTEN			       (0x20)
#define __FS_DIRTY  			       (0x40)
#endif

/* inode message */
#define __FS_IS_INODE_OK         (0x8000)
#define __FS_IS_INODE_FAIL       (0x4000)
#define __FS_IS_INODE_INIT       (0x2000)
#define __FS_IS_INODE_NODATA     (0x1000)
/* ------------- */

/* init sequence */
#define __FS_NORMAL              (0x8000)  /* normal don,t care */
#define __FS_SEQ(x)              (x)
/*---------------*/
/* magic num */
#define __MAGIC_L__  (0xAFBC562D)
#define __MAGIC_H__  (0x2b74113c)
/*-----------*/

/* default path */

#define SYSTEM_DEFAULT_PATH_LOG    "0:/log"
#define SYSTEM_DEFAULT_PATH_FW     "0:/firmware"
#define SYSTEM_DEFAULT_PATH_SYS    "0:/system"
#define SYSTEM_DEFAULT_PATH_SHELL  "0:/shell"

struct inode;

typedef struct inode_vmn_t
{  
   char *path;
   struct inode *inode;
	 int seq;
	 int (*init)(void);
}inode_vmn;

#define __FS_DEVICE__      "/dev/"
#define __FS_FILE__        "emmc"
#define __FS_DRV__         "/drv/"
#define __FS_INPUT__       "/dev/input/"
#define __FS_EEPROM__      "/less/eeprom"
#define __FS_SPIFLASH__    "/less/spiflash"
#define __FS_FLASH__       "flash"
#define __FS_STORAGE__     "/storage/"

#define FS_STORAGE(...)    (__FS_STORAGE__)
#define FS_DEVICE(x)       (__FS_DEVICE__##x)
#define FS_FILE(...)       (__FS_FILE__)
#define FS_DRV(x)          (__FS_DRV__##x)
#define FS_INPUT(x)        (__FS_INPUT__##x)
#define FS_EEPROM(...)     (__FS_EEPROM__)
#define FS_SPIFLASH(x)     (__FS_SPIFLASH__##x)
#define FS_FLASH(x)        (__FS_FLASH__##x)

#define __SHELL__          "__FS_SHELL_"
#define __FS__(x)          (#x)

#define FS_SHELL_SIZE_MAGIC (0xacb40000)
#define FS_SHELL_SIZE(x)   (sizeof(x)|FS_SHELL_SIZE_MAGIC)
	
typedef struct inode_identify_t inode_identify;
struct callback_t;
/* ARM complies setting */
extern inode_vmn  fs_vmn$$Base;
extern inode_vmn  fs_vmn$$Limit;
extern struct shell_cmd  fs_shell$$Base;
extern struct shell_cmd  fs_shell$$Limit;
/*       end        */
#define __INIT __attribute__((unused, section("fs_vmn")))
#define FS_INODE_REGISTER(inode_path,node_t,init_t,seq)                            \
static struct inode node_t;                                                        \
const inode_vmn __FS_##node_t                                                      \
__INIT =                                                                           \
{                                                                                  \
	inode_path,                                                                      \
	&node_t,                                                                         \
  seq,                                                                             \
	init_t                                                                           \
}                                                                                  \

#define FS_SHELL_REGISTER(enter)                                                   \
struct shell_cmd __FS_SHELL_##enter                                                \
__attribute__((unused, section("fs_shell")));                                      \

#define FS_SHELL_INIT(sh,entrance,size_t,cb_type)                                  \
{                                                                                  \
	 __FS_SHELL_##sh.i_peer = (void *)0;                                             \
	 __FS_SHELL_##sh.i_child = (void *)0;                                            \
   __FS_SHELL_##sh.cmd = #entrance;                                                \
	 __FS_SHELL_##sh.size = size_t;                                                  \
	 __FS_SHELL_##sh.it_type = cb_type;                                              \
 	 __FS_SHELL_##sh.enter = (void *)&entrance;                                      \
}                                                                                  \

#define FS_SHELL_STATIC(sh,entrance,size_t,cb_type)                                \
struct shell_cmd __FS_SHELL_##sh                                                   \
__attribute__((unused, section("fs_shell"))) =                                     \
{                                                                                  \
	(void *)0,                                                                       \
	(void *)0,                                                                       \
  #entrance,                                                                       \
	size_t,                                                                          \
	cb_type,                                                                         \
 	(void *)&entrance                                                                \
}                                                                                  \

#define FS_REGISTER_ENTRANCE(entrance_name,sequence)                               \
const inode_identify __FS_##entrance_name =                                        \
{                                                                                  \
	0xAFBC562D,                                                                      \
	entrance_name,                                                                   \
	sequence,                                                                        \
	0x2b74113c                                                                       \
}                                                                                  \

typedef struct inode_identify_t
{
	unsigned int id_l;
	void *entrance;
	unsigned sequence;
	unsigned int id_h;
}inode_identify;

struct file
{
  int               f_oflags; /* Open mode flags */ 
  FAR struct inode *f_inode;  /* Driver interface */
#if 0	
  void             *f_priv;   /* Per file driver private data */	
  char             *f_path;   /* file path */
	int               f_dev;    /* storage device identify */
  int               f_multi;  /* open multiple files */
#endif	
#if 0
	int               f_res;    /* result */
#endif
};

struct fd_find
{
	inode_vmn * inode_find;
	int fd;
};	
/* read dir struction */
typedef struct
{
	unsigned int num;
	void * enter;
	unsigned int length;
}readdir_entrance_def;
	/* storage dev */
typedef struct
{
	/* high-end function just for storage's files */
	int (*opendir)(const char *path);
	int (*readir)(FAR struct file *filp, const char *path,readdir_entrance_def * buffer);
	int (*mkdir)(const char *dir);
	int (*mkfs)(unsigned char drv,unsigned char sfd,unsigned int au);
	int (*lseek)(FAR struct file *filp, unsigned int offset, unsigned int whence);
	int (*sync)(FAR struct file *filp);	
	int (*close)(FAR struct file *filp);
}STORAGE_DEV;
/*--------------------*/
struct file_operations
{  
	/* The following methods must be identical in signature and position because
	 * the struct file_operations and struct mountp_operations are treated like
	 * unions.
	 */
	/* The device driver open method differs from the mountpoint open method */
	struct file * (*open) (FAR struct file *filp);
	int (*write)(FAR struct file *filp, FAR const void * buffer, unsigned int buflen);
	unsigned int (*read )(FAR struct file *filp, FAR void * buffer, unsigned int buflen);
  /* ioctrl */
	int (*ioctl)(FAR struct file *filp, int cmd, unsigned long arg,void *pri_data);
	/* storage device */
	STORAGE_DEV * storage_dev;
	/* end of data */
};
/*------------*/
struct shell_cmd
{
	struct shell_cmd *i_peer;
	struct shell_cmd *i_child;
	char   *cmd;
	int    size;/* function or param or others and param numbers and returns or not */
	int    it_type;
	void  *enter;
};

struct callback_t //it interface
{
	callback_function enter;
	unsigned int    it_type;//dma or RXNEIE or others
};

struct size_of_all{
	int inode_max;
  int shell_max;
};

/* This structure represents one inode in the niufs pseudo-file system */

/* These are the various kinds of operations that can be associated with
 * an inode.
 */

struct inode
{       
	char * inode_name;                        /* inode name for search */
  FAR struct inode          *i_peer;        /* Pointer to same level inode */
  FAR struct inode          *i_child;       /* Pointer to lower level inode */
  unsigned short             i_crefs;       /* References to inode */
  unsigned short             i_flags;       /* Flags for inode */
  struct file_operations     ops;           /* Inode operations */
	struct shell_cmd           *shell_i;      /* shell command entrance */
	struct size_of_all         max;           /* max of allthings */
#ifdef CONFIG_FILE_MODE
  mode_t                     i_mode;        /* Access mode flags */
#endif
  FAR void                  *i_private;     /* Per inode driver private data */
  int (*config)(void);                      /* inode config */
	struct file               flip;           /* file interface */
};

extern int fs_system_initialization(void);
extern FAR inode_vmn *inode_sched_getfiles(void);
extern const inode_vmn __FS_START__;
extern FAR inode_vmn * inode_find(inode_vmn *inode,FAR const char *path);
extern int system_initialization(void);
extern int inode_sched_limit(void);
extern int cb_sched_limit(void);
extern int shell_sched_limit(void);
FAR struct shell_cmd *shell_sched_getfiles(void);
FAR struct shell_cmd * shell_node_valid(void);
FAR inode_vmn *inode_valid(void);
FAR struct callback_t *cb_sched_getfiles(void);
extern struct file * open(const char *path, int oflags);

#endif /* FS_FS_H_ */



