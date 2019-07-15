
#ifndef __EASYLINK_H__
#define __EASYLINK_H__
/* easylink
 *
 * head1    0xA8
 * head2    0xE2
 * head3    0x38
 * head4    0x20
 * len      sizeof(payload)
 * id       0xxx
 * director 0xxx
 * payload  p[]
 * check    0xff
 *
 * */

#define EASYLINK_HEAD1 (0xA8)
#define EASYLINK_HEAD2 (0xE2)
#define EASYLINK_HEAD3 (0x38)
#define EASYLINK_HEAD4 (0x20)

#define EASYLINK_DIR_SRC  (0xA5)
#define EASYLINK_DIR_INC  (0x5A)

#define EASYLINK_MAXLEN (0xff)

/* easylink config */
extern int shell_pushlish(unsigned char * buffer , unsigned short len);
#define easylink_logic_publish shell_pushlish//send 

/* */

typedef struct{
	unsigned char *pl;
	unsigned char len;
	unsigned char id;
	unsigned char dir;
	unsigned char is_available;
}easylink;

int check_id(unsigned char id);
static unsigned char check_sum(unsigned char *pd,easylink * easy);
int iap_d_icr(unsigned char c,easylink *easy_link_d);
int easylink_create(unsigned char * buffer , unsigned short buffer_len,unsigned char ID , unsigned char * payload , unsigned char len);
int easylink_publish(unsigned char ID,unsigned char * payload,unsigned char len);
#endif
