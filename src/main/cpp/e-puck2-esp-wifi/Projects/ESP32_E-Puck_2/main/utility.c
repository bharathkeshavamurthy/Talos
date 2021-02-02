/*

File    : utility.c
Author  : Stefano Morgani
Date    : 2 March 2018
REV 1.0

Various utility functions
*/

#include "esp_efuse.h"
#include "utility.h"

static uint16_t robot_id = 0;

void robot_read_id(void) {
	uint32_t blk3_rdata0, blk3_rdata1;
	uint64_t temp;
	
	 // The id of the robot is stored in the first 64 bits of the block3.
	 // Each id is 2 bytes long, thus there is space for writing 4 times the id in case there is the need to change it.
	 // The old id need to be programmed to 0xFFFF in order to be identified as invalid.
	blk3_rdata0 = REG_READ(EFUSE_BLK3_RDATA0_REG);
	blk3_rdata1 = REG_READ(EFUSE_BLK3_RDATA1_REG);
	temp = ((uint64_t)blk3_rdata1<<32)|blk3_rdata0;
	//printf("  Block3: %x (data0) %x (data1)\r\n", blk3_rdata0, blk3_rdata1);
	for(int i=0; i<4; i++) {
		robot_id = (temp>>(i*16))&0xFFFF;
		if(robot_id != 0xFFFF) {
			break;
		}
	}
	//printf("  Robot id: %05d\r\n", robot_id);
}

uint16_t robot_get_id(void) {
	return robot_id;
}


