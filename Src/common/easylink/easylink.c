
#include "easylink.h"
#include "string.h"

unsigned int head_lost = 0;
unsigned int tail_lost = 0;
unsigned int frame_lost = 0;
unsigned int easylink_err = 0;

unsigned char payload[256];

int iap_d_icr(unsigned char c,easylink *easy_link_d)
{
	static unsigned char step = 0;
	static unsigned char step_head = 0;
	static unsigned int cnt = 0;
	 
	switch(step_head)
	{
	 case 0:
		 if(c == EASYLINK_HEAD1)
		 {
			 step_head = 1;
		 }
		 else
		 {
			 step_head = 0;
		 }
		 break;
	 case 1:
		 if(c == EASYLINK_HEAD2)
		 {
			 step_head = 2;
		 }
		 else
		 {
			 if(c == EASYLINK_HEAD1)
			 {
				 step_head = 1;
			 }else
			 {
			   step_head = 0;
			 }
		 }
		 break;
	 case 2:
		 if(c == EASYLINK_HEAD3)
		 {
			 step_head = 3;
		 }
		 else
		 {
			 if(c == EASYLINK_HEAD1)
			 {
				 step_head = 1;
			 }else
			 {
			   step_head = 0;
			 }
		 }
		 break;
	 case 3:
		 if(c == EASYLINK_HEAD4)
		 {
			 step = 4;
		     cnt = 0;
		     step_head = 0;
		     head_lost++;
			 return (-1);
		 }
		 else
		 {
			 if(c == EASYLINK_HEAD1)
			 {
				 step_head = 1;
			 }else
			 {
			   step_head = 0;
			 }
		 }
		 break;
	}

	switch(step)
	{
	 case 4:
		 easy_link_d->len  = c;
		 step = 5;
		 break;
	 case 5:
		 easy_link_d->id = c;
		 step = 6;
		 break;
	 case 6:
		 easy_link_d->dir = c;
	     cnt = 0;
		 step = 7;
		 break;
	 case 7:
		 if((easy_link_d->len) > cnt )
		 {
			 payload[cnt++] = c;
		 }
		 else
		 {
			 cnt = 0;
			 if( c == check_sum(payload,easy_link_d) )
			 {
				 if(easy_link_d->dir == EASYLINK_DIR_SRC)
				 {
					 if( !check_id(easy_link_d->id) )
					 {
						 if(easy_link_d->len < EASYLINK_MAXLEN )
						 {
							/* good we got it */
							easy_link_d->pl = payload;
							//send the easy_link_d to queue;
							tail_lost++;
							/* calibration the lost frame per */
							frame_lost = head_lost - tail_lost;

							easy_link_d->is_available = 1;

						//	d_r_n(&easy_link_d);//do it right now
							return 0;
						 }
						 else
						 { 
							 /* len error*/
							 easylink_err = 4;
							 step = 0;
						 }
					 }
					 else
					 {
						 /* id error*/
						 easylink_err = 3;
						 step = 0;
					 }
				 }
				 else
				 {
					 /* dir error*/
					 easylink_err = 2;
					 step = 0;
				 }
			 }
			 else
			 {
				 /* check sum error*/
				 easylink_err = 1;
				 step = 0;
			 }
			 step = 0;
		 }
		 break;
	 default :
		 step = 0;
		 break;
	}
	
	if(easylink_err)
	{
		easylink_err = 0;
		return -2;
	}
	
	return (-1);
}

static int check_id(unsigned char id)
{
	return 0;
}

static unsigned char check_sum(unsigned char *pd,easylink * easy)
{
	int i = 0;
	unsigned char sum = 0;
	unsigned char *pd_t = pd;
	for(i=0;i<easy->len;i++)
	{
		if(pd_t != (void *)0)
		  sum += *pd_t;
		else
			return 0xff;
		pd_t++;
	}
	sum += easy->len;
	sum += easy->id;
	sum += easy->dir;
	return sum;
}
/* easylink status */
void easylink_status(unsigned int * head , unsigned int * tail , unsigned int * err)
{
	*head = head_lost;
	*tail = tail_lost;
	*err  = easylink_err; 
}
/* create packages */
int easylink_create(unsigned char * buffer , unsigned short buffer_len,unsigned char ID , unsigned char * payload , unsigned char len)
{
	  unsigned char sum = 0;
	  if( buffer_len < len + 8 )
	  {
		  /* buffer length not enough */
		  return (-1);
	  }
	  /* epuse */
		buffer[0] = EASYLINK_HEAD1;
		buffer[1] = EASYLINK_HEAD2;
		buffer[2] = EASYLINK_HEAD3;
		buffer[3] = EASYLINK_HEAD4;
		/* payload len */
		buffer[4] = len;
		/* ID */
		buffer[5] = ID;
		/* direction */
		buffer[6] = EASYLINK_DIR_SRC;
		/* payload */
		memcpy(&buffer[7],payload,len);
		/* check */
		for( int i = 0 ; i < len + 3 ; i ++ )
		{
			 sum += buffer[ 4 + i ];
		}
		/* last num */
		buffer[ len + 7 ] = sum;
	  
		return len + 8;
}
/* easylink thread */

















































