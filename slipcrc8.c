#include "slipcrc8.h"

enum{magicByte=0xAA};

uint8_t slipcrc8init() {
	return magicByte;
}

uint8_t slipcrc8byte(uint8_t crc8, uint8_t byte) {
	uint16_t ret = crc8;
	ret+=((byte==0)?magicByte:byte);
	ret=((ret>=0x100)?ret+1:ret)&0xFF;
	return ret;
}

uint8_t	slipcrc8buf(uint8_t* buf, uint16_t size){
	uint8_t crc8=slipcrc8init();
	
	while(size--){
		crc8 = slipcrc8byte(crc8,*(buf++));
	}
	
	return crc8;
}
