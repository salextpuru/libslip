#ifndef __SLIPCRC8_H__
#define __SLIPCRC8_H__

#include <stdint.h>
 
/**
* @brief Init CRC8
* 
* @return uint8_t
*/
uint8_t	slipcrc8init();

/**
* @brief Add next byte to crc8
* 
* @param crc8 - current crc8
* @param byte - additional byte
* @return uint8_t - crc8
*/
uint8_t	slipcrc8byte(uint8_t crc8, uint8_t byte);

/**
* @brief Calculate crc8
* 
* @param buf - buffer
* @param size - size of buffer
* @return uint8_t - CRC8
*/
uint8_t	slipcrc8buf(uint8_t* buf, uint16_t size);

#endif /* __SLIPCRC8_H__ */
