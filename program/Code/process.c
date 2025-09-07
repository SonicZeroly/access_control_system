/**
  ******************************************************************************
  * @file    process.c
  * @brief   处理门禁系统
  ******************************************************************************
  */

#include "process.h"
#include "RC522.h"
#include "w25qxx.h"
#include "as608.h"
#include <string.h>

uint8_t open_flag = 0;

/*************************************************************/
/*                      密码开门                             */
/*************************************************************/
uint8_t password[PASSWORD_LEN] = {6,3,5,4,2};	//密码
uint8_t password_len = 8;

/*************************************************************/
/*                      M1卡刷门                             */
/*************************************************************/
#define ENTRIES_PER_SECTOR		818	//(4096)/5

typedef struct {
    uint8_t status;    // 状态标记:0xFF（空白）、0xAA（有效）、0x55（无效/已删除）
    uint8_t card_id[4];  // 4字节卡号
} card_entry_t;


//扇区前两个字节用于表明自己就是存储卡号的扇区
uint32_t active_start_addr = 0x00;
uint16_t store_cnt = 0;			//已存储的卡数目（保存到内部FLASH）

/**
  * @brief  检测PCD读卡情况
  * @param	void
  * @retval 无
  */
void pcd_scan( pcd_flag_t flag ){
	uint8_t Card_Type[2] = {0x00,0x00};  	//Mifare One(S50)卡
	uint8_t card_id[4];   //卡ID
	uint8_t status;
	
	status = PcdRequest(PICC_REQIDL, Card_Type);//寻卡函数，如果成功返回MI_OK，Card_Typr接收卡片的型号代码

	if(status == MI_OK)  
	{
		status = PcdAnticoll(card_id);//防冲撞 如果成功返回MI_OK
		if(status == MI_OK){
			printf("Card Num:%.2x%.2x%.2x%.2x\r\n",card_id[0],card_id[1],card_id[2],card_id[3]);					
		}
		status = PcdSelect(card_id);
		status = PcdHalt();  //卡片进入休眠状态
		
		//按照相应标志位执行代码
		if(flag==CHECK_EXIST){
			card_exists(card_id);
		}
		else if(flag==ADD_CARD){
			add_card_id(card_id);
		}
		else if(flag==DELETE_CARD){
			delete_card_id(card_id);
		}
	}
	HAL_Delay(10);
}

/**
  * @brief  检查卡号是否存在
  * @param	uint8_t数组的卡号
  * @retval true/false
  */
bool card_exists(uint8_t* target_id) {
    uint32_t current_addr = active_start_addr;
    card_entry_t entry;
    
    // 只遍历当前活跃扇区！
    for (int i = 0; i < ENTRIES_PER_SECTOR; i++) {
        W25QXX_BufferRead(&(entry.status), current_addr, 1);
        
        if (entry.status == 0xFF) {
            break; // 遇到空白条码，说明后面都没数据，停止遍历
        }
        else if (entry.status == 0xAA) {
			W25QXX_BufferRead(entry.card_id, current_addr+1, 4);
			if(memcmp(entry.card_id, target_id, 4) == 0){
				printf("卡号匹配成功\r\n");
				return true; // 找到有效卡号
			}
			else{
				printf("Card Num:%.2x%.2x%.2x%.2x\r\n",entry.card_id[0],entry.card_id[1],entry.card_id[2],entry.card_id[3]);
			}
        }
        current_addr += 5; // 地址递增，指向下一个条目
    }
	printf("卡号未登记\r\n");
    return false;
}

/**
  * @brief  添加卡号
  * @param	uint8_t数组的卡号
  * @retval true/false
  */
uint8_t add_card_id(uint8_t* new_id){
	//先查找存储中是否存在该卡号
	if(!card_exists(new_id)){
		uint32_t current_addr = active_start_addr;
		card_entry_t entry;
		
		// 寻找第一个空白位置
		for (int i = 0; i < ENTRIES_PER_SECTOR; i++) {
			W25QXX_BufferRead(&(entry.status), current_addr, 1);
			if (entry.status == 0xFF) {
				// 找到空白位置，准备写入
				entry.status = 0xAA;
				W25QXX_BufferWrite(&(entry.status), current_addr, 1);
				W25QXX_BufferWrite(new_id, current_addr+1, 4);
				printf("登记成功\r\n");
				return 1;
			}
			current_addr += 5;
		}
		printf("扇区满了\r\n");
		// 如果执行到这里，说明当前扇区满了！
//		garbage_collection(); // 触发垃圾回收，腾出空间
//		add_card(new_id);     // 递归调用一次，此时就有空间了
	}
}

/**
  * @brief  删除卡号（只是设置标志而已）
  * @param	uint8_t数组的卡号
  * @retval true/false
  */
bool delete_card_id(uint8_t* target_id) {
    uint32_t current_addr = active_start_addr;
    card_entry_t entry;
    
    for (int i = 0; i < ENTRIES_PER_SECTOR; i++) {
        W25QXX_BufferRead(&(entry.status), current_addr, 1);
        
        if (entry.status == 0xFF) break; // 到头了，没找到
        
        // 找到有效且匹配的卡号
        if (entry.status == 0xAA && memcmp(entry.card_id, target_id, 4) == 0) {
            // 关键操作：只修改状态标记为“无效”
            uint8_t invalid_status = 0x55;
            // 注意：这是写入操作，不是擦除！只需写入状态字节所在的位置。
			W25QXX_BufferWrite(&(invalid_status), current_addr, 1);
            return true; // 删除成功
        }
        current_addr += 5;
    }
    return false; // 没找到这个卡号
}

/**
  * @brief  获取已存储的卡的数量
  * @param	void
  * @retval true/false
  */
uint16_t get_card_num(void){
	
}

///**
//  * @brief  保存卡存储的相关信息到内部flash
//  * @param	void
//  * @retval void
//  */
//void save_card_info(uint16_t cnt, uint32_t base){
//	//存储卡数目
//	card_entry_t entry;
//	
//}

//void renew_card_info(){
//	//已扇区为单位开始查找
//}

/*************************************************************/
/*                      指纹识别                             */
/*************************************************************/


void process_init(void){
	
	
}
/*************************************************************/
/*                      人脸识别（暂无）                     */
/*************************************************************/
