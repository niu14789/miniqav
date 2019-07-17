
#ifndef __F_SHELL_H__
#define __F_SHELL_H__

/*-------------------------callback config------------------------*/
/* callback type */
#define _CB_IT_               (0x80)
#define _CB_TIMER_            (0x70)
#define _CB_EXE_              (0x60)
#define _CB_VAR_              (0x50)
#define _CB_ARRAY_            (0x40)
#define _CB_IDLE_             (0x30)
#define _CB_RT_               (0x20)

#define TYPE_OTHER            (0x00)
#define TYPE_CHAR             (0x01)
#define TYPE_UCHAR            (0x02)
#define TYPE_SHORT            (0x03)
#define TYPE_USHORT           (0x04)
#define TYPE_INT              (0x05)
#define TYPE_UINT             (0x06)
#define TYPE_STRUCT           (0x07)

/*----------------------------------------------------------------*/
#define SHELL_TYPE(cmd_p,dat_p,size_p)  ((((unsigned char)cmd_p) << 24 ) | (((unsigned char)dat_p) << 16 ) | ((unsigned short)size_p))
/* for IT feature */

struct shell_cmd * shell_find(const char * shell_cmd);
extern __inline int shell_timer_thread(int task_id);
int shell_read(const char * cmd,void *data,unsigned int size,unsigned int offset);
int shell_write(const char * cmd,void *data,unsigned int size,unsigned int offset);
int shell_execute(const char * cmd,unsigned int param);
int system_shell_insert(struct shell_cmd ** p_link,struct shell_cmd * p_shell);
#if DYNAMIC_FUNCTIONS // these functions are not be uesed under freertos. 
int shell_create_dynamic( char * id , void * task , unsigned int freq );
int shell_delete_dynamic( const char * id , unsigned char mode );
int shell_revert_dynamic( const char * id );
int shell_enable_dynamic(unsigned char index);
int shell_disable_dynamic(unsigned char index);
#endif
static void * base_found(unsigned int index);
int system_shell_insert(struct shell_cmd ** p_link,struct shell_cmd * p_shell);
void shell_stack_init(void);
#endif










