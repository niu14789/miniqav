
#ifndef __F_SHELL_H__
#define __F_SHELL_H__

/*-------------------------callback config------------------------*/
/* callback type */
#define _CB_IT_               (0x80000000)
#define _CB_TIMER_            (0x70000000)
#define _CB_EXE_              (0x60000000)
#define _CB_VAR_              (0x50000000)
#define _CB_ARRAY_            (0x40000000)
#define _CB_IDLE_             (0x30000000)
#define _CB_RT_               (0x20000000)
/* for IT feature */
#define _IS_CB_TYPE_(x)       (x&0xf0000000)

#define _CB_PRIORITY_(x)      (x<<24)  /* 0-15 */
#define _IS_CB_PARAM1_(x)     ((x&0x0f000000)>>24)

#define _CB_IT_TPYE_(x)       (x<<20)/* 0-15 */
#define _IS_CB_PARAM2_(x)    ((x&0x00f00000)>>20)

#define _CB_DISABLE_          (0x00000000) /* 0 or 1 */
#define _CB_ENABLE_           (0x00080000)

#define _IS_CB_PARAM3_(x)     (x&_CB_ENABLE_?1:0)

#define _CB_PERIOD_(x)        (x) /* 0-65535 */
#define _IS_CB_PARAM4_(x)     (x&0xffff)  //ms

#define _CB_IT_IRQN_(x)        (x) /* 0-65535 */
#define _IS_CB_PARAM5_(x)     (x&0xffff)

#define TASK_PERIOD0           (1000)   // ms
#define TASK_PERIOD1           (4000)   // ms
#define TASK_PERIOD2           (12000)  // ms
#define TASK_PERIOD3           (335 * 10) // 100ms
#define TASK_PERIOD4           (335 * 20) // 200msms
#define TASK_PERIOD5           (335 * 50) // 500msms

#define TASK_PERIOD0_ID           (0xf1)   // ms
#define TASK_PERIOD1_ID           (0xf2)   // ms
#define TASK_PERIOD2_ID           (0xf3)  // ms
#define TASK_PERIOD3_ID           (0xf4) // ms
#define TASK_PERIOD4_ID           (0xf5) // ms
#define TASK_PERIOD5_ID           (0xf6) // ms

typedef enum{
	shell_error = 0,
	shell_it, /* interrupt remap */
	shell_tc, /* call by time */
	shell_exe,
	shell_array,
}shell_type;

struct shell_cmd * shell_find(const char * shell_cmd);
extern __inline int shell_timer_thread(int task_id);
int shell_read(const char * cmd,void *data,unsigned int size,unsigned int offset);
int shell_write(const char * cmd,void *data,unsigned int size,unsigned int offset);
int shell_execute(const char * cmd,unsigned int param);
int system_shell_insert(struct shell_cmd ** p_link,struct shell_cmd * p_shell);
int shell_create_dynamic( char * id , void * task , unsigned int freq );
int shell_delete_dynamic( const char * id , unsigned char mode );
int shell_revert_dynamic( const char * id );
int shell_enable_dynamic(unsigned char index);
int shell_disable_dynamic(unsigned char index);
static void * base_found(unsigned int index);
int system_shell_insert(struct shell_cmd ** p_link,struct shell_cmd * p_shell);
void shell_stack_init(void);
#endif










