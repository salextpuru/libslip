#ifndef __ZXSLIP_H__
#define __ZXSLIP_H__

#include <stdint.h>

/**
* @brief Максимальное количество сокетов
*/
enum{zxslip_max_sockets	= 4};

/**
* @brief Максимальная длина данных
*/
enum{zxslip_max_datalen	= 0x400};

/**
* @brief Коды команд
*/
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

/**
* @brief Получить текущий идентификатор пакета
* 
* @return uint8_t
*/
uint8_t zxslip_getId();

/**
* @brief Установить текущий идентификатор пакета
*/
void zxslip_setId(uint8_t id);

/**
* @brief Добавить в буфер заголовок, вернет оно лишь указатель, что за буфера концом
* 
* @param buf - буфер куда помещается команда
* @param cmd - код команды
* @param id - идентификатор
* @return uint8_t* - указатель на конец пакета
*/
uint8_t* zxslip_addheader(uint8_t* buf, uint8_t cmd, uint8_t id);


/** --------------------- Функции создания команд --------------------- */

#define zxslip_cr_gettxtinfo(buf) (zxslip_addheader(buf, zxslip_cmd_gettxtinfo, zxslip_getId()))
#define zxslip_cr_esp_poll(buf) (zxslip_addheader(buf, zxslip_cmd_esp_poll, zxslip_getId()))

uint8_t* zxslip_cr_wifi_config(uint8_t* buf, uint8_t mode, const char* name, const char* pass, uint8_t auth);
#define	zxslip_cr_wifi_status(buf) (zxslip_addheader(buf, zxslip_cmd_wifi_status, zxslip_getId()))

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

/** --------------------- Функции парсинга команд --------------------- */

/**
* @brief Заголовок пакета
*/
typedef struct zxslip_pkt_header{
	uint8_t cmd_code;
	uint8_t id;
}zxslip_pkt_header;

/**
* @brief Тело ответа на команду zxslip_cmd_gettxtinfo
*/
typedef struct zxslip_apkt_gettxtinfo{
	uint16_t	size;
	uint8_t		text[zxslip_max_datalen];
}zxslip_apkt_gettxtinfo;

/**
* @brief Тело ответа на команду zxslip_cmd_esp_poll
*/
typedef struct zxslip_apkt_esp_poll{
	uint8_t		nsock;
	uint8_t		sock_status[zxslip_max_sockets];
}zxslip_apkt_esp_poll;

/**
* @brief Тело ответа на команду zxslip_cmd_wifi_config
*/
typedef struct zxslip_apkt_wifi_config{
	int8_t		exit_code;
	uint8_t		status;
}zxslip_apkt_wifi_config;

/**
* @brief Тело ответа на команду zxslip_cmd_wifi_status
*/
typedef struct zxslip_apkt_wifi_status{
	int8_t		exit_code;
	uint8_t		status;
	uint8_t		auth;
	uint8_t		mode;
	uint8_t		name[0x40];
	uint8_t		pass[0x40];
}zxslip_apkt_wifi_status;

/**
* @brief Тело ответа на команду zxslip_cmd_socket
*/
typedef struct zxslip_apkt_socket{
	int8_t		exit_code;
}zxslip_apkt_socket;

/**
* @brief Тело ответа на команду zxslip_cmd_close
*/
typedef struct zxslip_apkt_close{
	int8_t		exit_code;
}zxslip_apkt_close;

/**
* @brief Тело ответа на команду zxslip_cmd_fcntl
*/
typedef struct zxslip_apkt_fcntl{
	int8_t		exit_code;
	uint8_t		fd_sock;
	uint8_t		cmd_fcntl;
	uint8_t		data[zxslip_max_datalen];
}zxslip_apkt_fcntl;

/**
* @brief Тело ответа на команду zxslip_cmd_connect
*/
typedef struct zxslip_apkt_connect{
	int8_t		exit_code;
}zxslip_apkt_connect;

/**
* @brief Тело ответа на команду zxslip_cmd_recv
*/
typedef struct zxslip_apkt_recv{
	int8_t		exit_code;
	uint8_t		fd_sock;
	uint8_t		flags;
	uint16_t	size;
	uint8_t		data[zxslip_max_datalen];
}zxslip_apkt_recv;

/**
* @brief Тело ответа на команду zxslip_cmd_send
*/
typedef struct zxslip_apkt_send{
	int8_t		exit_code;
}zxslip_apkt_send;


#endif /* __ZXSLIP_H__ */
