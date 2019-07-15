
#ifndef __FS_ALLOCATE_H__
#define __FS_ALLOCATE_H__
/* RAM addr for allocate */
#define ALLOCATE_RAM_STACK_BASE       (0x20408000)//32K
/* according to the above */
#define ALLOCATE_RAM_STACK_START_ADDR (ALLOCATE_RAM_STACK_BASE)
#define ALLOCATE_RAM_STACK_END_ADDR   (ALLOCATE_RAM_STACK_START_ADDR + 20 * 1024 )
#define ALLOCATE_RAM_ROM_START_ADDR   (ALLOCATE_RAM_STACK_END_ADDR)
#define ALLOCATE_RAM_ROM_END_ADDR     (ALLOCATE_RAM_ROM_START_ADDR + 64 * 1024 )
#define ALLOCATE_RAM_ROM_SIZE         (ALLOCATE_RAM_ROM_END_ADDR - ALLOCATE_RAM_ROM_START_ADDR)

/* marks */
#define RAM_ID_HEAD (0x353ab756)
#define RAM_ID_TAIL (0xacb8562d)
/* rom marks */
#define ROM_ID_HEAD (0x38ab1ef0)
#define ROM_ID_TAIL (0x6184abe6)
/* config msg macro */
#define CONFIG_ENDBLE          (0x14E56800)
#define CONFIG_UNDELETE_ENABLE (0x3412DEF0)
/* config */
typedef struct
{
	char * path;
	unsigned int delete_flag;
	unsigned int config_enable;
	unsigned int flag1;
	unsigned int flag2;
	unsigned int flag3;
	unsigned int flag4;
	unsigned int flag5;
}allocate_config_msg;/* default the config msf*/
/* config msg */
typedef struct config
{
	char buffer[32];
	allocate_config_msg msg;
}config_def;
/* run or not run */
typedef struct
{
	unsigned int head;
	unsigned int flag;
	unsigned int tail;
	/* config msg */
	config_def config[20];
	/* end */
}allocate_config_def;/* default the data run or not */


#endif

