#ifndef __ZXSLIP_H__
#define __ZXSLIP_H__

#include <stdint.h>

enum zxslip_cmd_ids{
	//	Общие команды
	zxslip_cmd_gettxtinfo	= 0x01,
	zxslip_cmd_esp_poll	= 0x02,
	
	//	Команды работы с WiFi
	zxslip_cmd_wifi_config	= 0x08,
	zxslip_cmd_wifi_status	= 0x09,
	
	//	Команды управления сокетом
	zxslip_cmd_socket	= 0x10,
	zxslip_cmd_close	= 0x11,
	zxslip_cmd_fcntl	= 0x12,
	//	Клиентская часть (ZX соединяется с сервером)
	zxslip_cmd_connect	= 0x18,
	zxslip_cmd_recv		= 0x19,
	zxslip_cmd_send		= 0x1A,
	//	Серверная часть (ZX является сервером)
	zxslip_cmd_bind		= 0x20,
	zxslip_cmd_listen	= 0x21,
	zxslip_cmd_accept	= 0x22
};

uint8_t zxslip_getId();
void zxslip_setId(uint8_t id);

// Добавить в буфер заголовок, вернет оно лишь указатель, что за буфера концом
uint8_t* zxslip_addheader(uint8_t* buf, uint8_t cmd, uint8_t id);

// Создать команду
#define zxslip_cr_gettxtinfo(buf) zxslip_addheader(buf, zxslip_cmd_gettxtinfo, zxslip_getId())
#define zxslip_cr_esp_poll(buf) zxslip_addheader(buf, zxslip_cmd_esp_poll, zxslip_getId())

uint8_t* zxslip_cr_wifi_config(uint8_t* buf, uint8_t mode, const char* name, const char* pass, uint8_t auth);
#define	zxslip_cr_wifi_status(buf) zxslip_addheader(buf, zxslip_cmd_wifi_status, zxslip_getId())

uint8_t* zxslip_cr_socket(uint8_t* buf, int8_t domain, int8_t type, int8_t protocol);
uint8_t* zxslip_cr_close(uint8_t* buf, int8_t fd);
uint8_t* zxslip_cr_fcntl(uint8_t* buf, int fd, int cmd, uint8_t* data, uint16_t size);

uint8_t* zxslip_cr_connect(uint8_t* buf, int8_t fd, uint8_t *addr, int16_t addrlen);
uint8_t* zxslip_cr_recv(uint8_t* buf, int8_t fd, uint16_t len, int8_t flags);
uint8_t* zxslip_cr_send(uint8_t* buf, int8_t fd, uint8_t* data, uint16_t len, int8_t flags);
/*
uint8_t* zxslip_cr_bind
uint8_t* zxslip_cr_listen
uint8_t* zxslip_cr_accept
*/

#endif /* __ZXSLIP_H__ */
