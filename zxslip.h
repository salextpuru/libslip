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
* @brief Состояние сокета
*/
typedef struct SockStatus{
	uint8_t	fd_sock;
	uint8_t	error;
	uint8_t	status;
}SockStatus;

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

/** --------------------- Функции парсинга --------------------- */

/**
* @brief Заголовок пакета
*/
typedef struct zxslip_pkt_header{
	uint8_t reserv0;
	uint8_t cmd_code;
	uint8_t id;
}zxslip_pkt_header;

/** --------------------- Функции парсинга запросов --------------------- */
typedef struct zxslip_qpkt_wifi_config{
	uint8_t		auth;
	uint8_t		mode;
	uint8_t		name[0x41];
	uint8_t		pass[0x41];
}zxslip_qpkt_wifi_config;

typedef struct zxslip_qpkt_socket{
	uint8_t		domain;
	uint8_t		type;
	uint8_t		protocol;
}zxslip_qpkt_socket;

typedef struct zxslip_qpkt_close{
	uint8_t		fd_sock;
}zxslip_qpkt_close;

typedef struct zxslip_qpkt_fcntl{
	uint8_t		fd_sock;
	uint8_t		cmd_fcntl;
	uint16_t	size;
	uint8_t*	data;
}zxslip_qpkt_fcntl;

typedef struct zxslip_qpkt_connect{
	uint8_t		fd_sock;
	uint16_t	adrsize;
	uint8_t*	adr;
}zxslip_qpkt_connect;

typedef struct zxslip_qpkt_recv{
	uint8_t		fd_sock;
	uint8_t		flags;
	uint16_t	size;
}zxslip_qpkt_recv;

typedef struct zxslip_qpkt_send{
	uint8_t		fd_sock;
	uint8_t		flags;
	uint16_t	size;
	uint8_t*	data;
}zxslip_qpkt_send;

/**
* @brief Общая функция парсинга всех пакетов-запросов
* 
* @param buf - буфер с данными
* @param size - размер данных
*/
void zxslip_query_parse(uint8_t* buf, uint16_t size);

/** --------------------- Функции парсинга ответов --------------------- */
/**
* @brief Тело ответа на команду zxslip_cmd_gettxtinfo
*/
typedef struct zxslip_apkt_gettxtinfo{
	uint16_t	size;
	uint8_t*	text;
}zxslip_apkt_gettxtinfo;

/**
* @brief Тело ответа на команду zxslip_cmd_esp_poll
*/
typedef struct zxslip_apkt_esp_poll{
	uint8_t		wifi_status;
	uint8_t		nsock;
	SockStatus*	sock_status;
}zxslip_apkt_esp_poll;

/**
* @brief Тело ответа на команду zxslip_cmd_wifi_config
*/
typedef struct zxslip_apkt_wifi_config{
	int8_t		exit_code;
	uint8_t		wifi_status;
}zxslip_apkt_wifi_config;

/**
* @brief Тело ответа на команду zxslip_cmd_wifi_status
*/
typedef struct zxslip_apkt_wifi_status{
	int8_t		exit_code;
	uint8_t		wifi_status;
	uint8_t		auth;
	uint8_t		mode;
	uint8_t		name[0x41];
	uint8_t		pass[0x41];
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
	uint16_t	size;
	uint8_t*	data;
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
	uint8_t*	data;
}zxslip_apkt_recv;

/**
* @brief Тело ответа на команду zxslip_cmd_send
*/
typedef struct zxslip_apkt_send{
	int8_t		exit_code;
	uint8_t		fd_sock;
	uint8_t		flags;
	uint16_t	size;
}zxslip_apkt_send;

/**
* @brief Общая функция парсинга всех пакетов-ответов
* 
* @param buf - буфер с данными
* @param size - размер данных
*/
void zxslip_answer_parse(uint8_t* buf, uint16_t size);

/** --------------------- Функции создания команд --------------------- */

uint8_t*  zxslip_crq_gettxtinfo(uint8_t* buf);
uint8_t*  zxslip_crq_esp_poll(uint8_t* buf);

uint8_t* zxslip_crq_wifi_config(uint8_t* buf, zxslip_qpkt_wifi_config* p);
uint8_t* zxslip_crq_wifi_status(uint8_t* buf);

uint8_t* zxslip_crq_socket(uint8_t* buf, zxslip_qpkt_socket* p);
uint8_t* zxslip_crq_close(uint8_t* buf, zxslip_qpkt_close* p);
uint8_t* zxslip_crq_fcntl(uint8_t* buf, zxslip_qpkt_fcntl* p);

uint8_t* zxslip_crq_connect(uint8_t* buf, zxslip_qpkt_connect* p);
uint8_t* zxslip_crq_recv(uint8_t* buf, zxslip_qpkt_recv* p);
uint8_t* zxslip_crq_send(uint8_t* buf, zxslip_qpkt_send* p);
/*
uint8_t* zxslip_cr_bind
uint8_t* zxslip_cr_listen
uint8_t* zxslip_cr_accept
*/

/** --------------------- Функции создания ответов --------------------- */
uint8_t* zxslip_cra_gettxtinfo(uint8_t* buf, zxslip_apkt_gettxtinfo* p);
uint8_t*zxslip_cra_esp_poll(uint8_t* buf, zxslip_apkt_esp_poll* p);

uint8_t* zxslip_cra_wifi_config(uint8_t* buf, zxslip_apkt_wifi_config* p);
uint8_t* zxslip_cra_wifi_status(uint8_t* buf, zxslip_apkt_wifi_status* p);

uint8_t* zxslip_cra_socket(uint8_t* buf, zxslip_apkt_socket* p);
uint8_t* zxslip_cra_close(uint8_t* buf, zxslip_apkt_close* p);
uint8_t* zxslip_cra_fcntl(uint8_t* buf, zxslip_apkt_fcntl* p);

uint8_t* zxslip_cra_connect(uint8_t* buf, zxslip_apkt_connect* p);
uint8_t* zxslip_cra_recv(uint8_t* buf, zxslip_apkt_recv* p);
uint8_t* zxslip_cra_send(uint8_t* buf, zxslip_apkt_send* p);

/*
uint8_t* zxslip_cra_bind
uint8_t* zxslip_cra_listen
uint8_t* zxslip_cra_accept
*/

#endif /* __ZXSLIP_H__ */
