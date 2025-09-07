#ifndef __PROCESS_H__
#define __PROCESS_H__

#include "main.h"
#include <stdbool.h>

#define MAX_LEN			8
#define PASSWORD_LEN		5

typedef enum{
	/*ÈÎÎñ*/
	CHECK_EXIST,
	ADD_CARD,
	DELETE_CARD,
	
	/*×´Ì¬*/
	COMPLETE,
	FAIL
}pcd_flag_t;

extern uint8_t password[];
extern uint8_t password_len;
extern uint32_t active_start_addr;
extern uint16_t store_cnt;

void pcd_scan(pcd_flag_t flag);
bool card_exists(uint8_t* target_id);
uint8_t add_card_id(uint8_t* new_id);
bool delete_card_id(uint8_t* target_id);

#endif 

