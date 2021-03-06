#include "zxslip.h"
#include <string.h>

static uint8_t sendpkt_id=0xAB;

// Функция копирования с возвратом на конец строки
static uint8_t* stpncpy_r(uint8_t *dest, uint8_t *src, uint16_t n) {
	while ( (*src) && (n--) ) {
		*(src++) = *(dest++);
	}
	return src;
}

uint8_t zxslip_getId() {
	return sendpkt_id;
}

void zxslip_setId ( uint8_t id ) {
	sendpkt_id = id;
}

uint8_t* zxslip_addheader ( uint8_t* buf, uint8_t cmd, uint8_t id ) {
	* ( buf++ ) = 0;
	* ( buf++ ) = cmd;
	* ( buf++ ) = id;
	return buf;
}

/** --------------------- Функции обратного вызова запросов --------------------- */
void (*zxslip_q_gettxtinfo_cb)(zxslip_pkt_header* h);
void  (*zxslip_q_esp_poll_cb)(zxslip_pkt_header* h);

void (*zxslip_q_wifi_config_cb)(zxslip_pkt_header* h, zxslip_qpkt_wifi_config* p);
void (*zxslip_q_wifi_status_cb)(zxslip_pkt_header* h);

void (*zxslip_q_socket_cb)(zxslip_pkt_header* h, zxslip_qpkt_socket* p);
void (*zxslip_q_close_cb)(zxslip_pkt_header* h, zxslip_qpkt_close* p);
void (*zxslip_q_fcntl_cb)(zxslip_pkt_header* h, zxslip_qpkt_fcntl* p);

void (*zxslip_q_connect_cb)(zxslip_pkt_header* h, zxslip_qpkt_connect* p);
void (*zxslip_q_recv_cb)(zxslip_pkt_header* h, zxslip_qpkt_recv* p);
void (*zxslip_q_send_cb)(zxslip_pkt_header* h, zxslip_qpkt_send* p);
/*
uint8_t* zxslip_cr_bind
uint8_t* zxslip_cr_listen
uint8_t* zxslip_cr_accept
*/

/** --------------------- Функции обратного вызова ответов --------------------- */
void (*zxslip_a_gettxtinfo_cb)(zxslip_pkt_header* h, zxslip_apkt_gettxtinfo* p);
void (*zxslip_a_esp_poll_cb)(zxslip_pkt_header* h, zxslip_apkt_esp_poll* p);

void (*zxslip_a_wifi_config_cb)(zxslip_pkt_header* h, zxslip_apkt_wifi_config* p);
void (*zxslip_a_wifi_status_cb)(zxslip_pkt_header* h, zxslip_apkt_wifi_status* p);

void (*zxslip_a_socket_cb)(zxslip_pkt_header* h, zxslip_apkt_socket* p);
void (*zxslip_a_close_cb)(zxslip_pkt_header* h, zxslip_apkt_close* p);
void (*zxslip_a_fcntl_cb)(zxslip_pkt_header* h, zxslip_apkt_fcntl* p);

void (*zxslip_a_connect_cb)(zxslip_pkt_header* h, zxslip_apkt_connect* p);
void (*zxslip_a_recv_cb)(zxslip_pkt_header* h, zxslip_apkt_recv* p);
void (*zxslip_a_send_cb)(zxslip_pkt_header* h, zxslip_apkt_send* p);

/*
uint8_t* zxslip_cra_bind
uint8_t* zxslip_cra_listen
uint8_t* zxslip_cra_accept
*/

/** --------------------- Функции создания команд --------------------- */

uint8_t*  zxslip_crq_gettxtinfo(uint8_t* buf) {
	return zxslip_addheader ( buf, zxslip_cmd_gettxtinfo, zxslip_getId() );
}

uint8_t*  zxslip_crq_esp_poll(uint8_t* buf) {
	return zxslip_addheader ( buf, zxslip_cmd_esp_poll, zxslip_getId() );
}

uint8_t* zxslip_crq_wifi_config(uint8_t* buf, zxslip_qpkt_wifi_config* p) {
	buf = zxslip_addheader ( buf, zxslip_cmd_wifi_config, zxslip_getId() );
	* ( buf++ ) = p->auth;
	* ( buf++ ) = p->mode;
	return stpncpy_r( stpncpy_r( buf, p->name, 0x41 )+1, p->pass, 0x41 )+1;
}

uint8_t* zxslip_crq_wifi_status(uint8_t* buf) {
	return zxslip_addheader ( buf, zxslip_cmd_wifi_status, zxslip_getId() );
}

uint8_t* zxslip_crq_socket(uint8_t* buf, zxslip_qpkt_socket* p) {
	buf = zxslip_addheader ( buf, zxslip_cmd_socket, zxslip_getId() );
	* ( buf++ ) = p->domain;
	* ( buf++ ) = p->type;
	* ( buf++ ) = p->protocol;
	return buf;
}

uint8_t* zxslip_crq_close(uint8_t* buf, zxslip_qpkt_close* p) {
	buf = zxslip_addheader ( buf, zxslip_cmd_close, zxslip_getId() );
	* ( buf++ ) = p->fd_sock;
	return buf;
}

uint8_t* zxslip_crq_fcntl(uint8_t* buf, zxslip_qpkt_fcntl* p) {
	buf = zxslip_addheader ( buf, zxslip_cmd_fcntl, zxslip_getId() );
	* ( buf++ ) = p->fd_sock;
	* ( buf++ ) = p->cmd_fcntl;
	* ( buf++ ) = p->size & 0xFF;
	* ( buf++ ) = (p->size>>8) & 0xFF;
	//
	if ( ( !p->data ) || ( !p->size ) ) {
		return buf;
	}

	memcpy ( buf,p->data,p->size );
	buf+=p->size;
	return buf;
}

uint8_t* zxslip_crq_connect(uint8_t* buf, zxslip_qpkt_connect* p) {
	buf = zxslip_addheader ( buf, zxslip_cmd_fcntl, zxslip_getId() );
	* ( buf++ ) = p->fd_sock;
	* ( buf++ ) = p->adrsize & 0xFF;
	* ( buf++ ) = (p->adrsize>>8) & 0xFF;
	memcpy ( buf, p->adr,p->adrsize );
	buf+=p->adrsize;
	return buf;
}

uint8_t* zxslip_crq_recv(uint8_t* buf, zxslip_qpkt_recv* p) {
	buf = zxslip_addheader ( buf, zxslip_cmd_recv, zxslip_getId() );
	* ( buf++ ) = p->fd_sock;
	* ( buf++ ) = p->flags;
	* ( buf++ ) = p->size & 0xff;
	* ( buf++ ) = ( p->size>>8 ) & 0xff;
	return buf;
}

uint8_t* zxslip_crq_send(uint8_t* buf, zxslip_qpkt_send* p) {
	buf = zxslip_addheader ( buf, zxslip_cmd_send, zxslip_getId() );
	* ( buf++ ) = p->fd_sock;
	* ( buf++ ) = p->flags;
	* ( buf++ ) = p->size & 0xff;
	* ( buf++ ) = ( p->size >> 8 ) & 0xff;
	memcpy ( buf, p->data, p->size );
	buf+=p->size;
	return buf;
}


/** --------------------- Функции создания ответов --------------------- */
uint8_t* zxslip_cra_gettxtinfo(uint8_t* buf, zxslip_apkt_gettxtinfo* p) {
	buf = zxslip_addheader ( buf, zxslip_cmd_gettxtinfo, zxslip_getId() );
	* ( buf++ ) = p->size & 0xFF;
	* ( buf++ ) = (p->size>>8) & 0xFF;
	//
	if ( !p->size ) {
		return buf;
	}

	memcpy ( buf,p->text,p->size );
	buf+=p->size;
	return buf;
}

uint8_t*zxslip_cra_esp_poll(uint8_t* buf, zxslip_apkt_esp_poll* p) {
	uint16_t sock_status_size = p->nsock * sizeof(SockStatus);
	buf = zxslip_addheader ( buf, zxslip_cmd_esp_poll, zxslip_getId() );
	memcpy ( buf,p, sizeof(zxslip_apkt_esp_poll) );
	buf+=sizeof(zxslip_apkt_esp_poll);
	return buf;
}

uint8_t* zxslip_cra_wifi_config(uint8_t* buf, zxslip_apkt_wifi_config* p) {
	buf = zxslip_addheader ( buf, zxslip_cmd_wifi_config, zxslip_getId() );
	* ( buf++ ) = p->exit_code;
	* ( buf++ ) = p->wifi_status;
	return buf;
}

uint8_t* zxslip_cra_wifi_status(uint8_t* buf, zxslip_apkt_wifi_status* p) {
	buf = zxslip_addheader ( buf, zxslip_cmd_wifi_status, zxslip_getId() );
	* ( buf++ ) = p->exit_code;
	* ( buf++ ) = p->wifi_status;
	* ( buf++ ) = p->auth;
	* ( buf++ ) = p->mode;
	
	return stpncpy_r( stpncpy_r( buf, p->name, 0x41 )+1, p->pass, 0x41 )+1;
}

uint8_t* zxslip_cra_socket(uint8_t* buf, zxslip_apkt_socket* p) {
	buf = zxslip_addheader ( buf, zxslip_cmd_socket, zxslip_getId() );
	* ( buf++ ) = p->exit_code;
	return buf;
}

uint8_t* zxslip_cra_close(uint8_t* buf, zxslip_apkt_close* p) {
	buf = zxslip_addheader ( buf, zxslip_cmd_close, zxslip_getId() );
	* ( buf++ ) = p->exit_code;
	return buf;
}

uint8_t* zxslip_cra_fcntl(uint8_t* buf, zxslip_apkt_fcntl* p) {
	buf = zxslip_addheader ( buf, zxslip_cmd_fcntl, zxslip_getId() );
	* ( buf++ ) = p->exit_code;
	* ( buf++ ) = p->fd_sock;
	* ( buf++ ) = p->cmd_fcntl;
	* ( buf++ ) = p->size & 0xFF;
	* ( buf++ ) = (p->size>>8) & 0xFF;
	//
	if ( ( !p->data ) || ( !p->size ) ) {
		return buf;
	}

	memcpy ( buf,p->data,p->size );
	buf+=p->size;
	return buf;
}

uint8_t* zxslip_cra_connect(uint8_t* buf, zxslip_apkt_connect* p) {
	buf = zxslip_addheader ( buf, zxslip_cmd_connect, zxslip_getId() );
	* ( buf++ ) = p->exit_code;
	return buf;
}

uint8_t* zxslip_cra_recv(uint8_t* buf, zxslip_apkt_recv* p) {
	buf = zxslip_addheader ( buf, zxslip_cmd_recv, zxslip_getId() );
	* ( buf++ ) = p->exit_code;
	* ( buf++ ) = p->fd_sock;
	* ( buf++ ) = p->flags;
	
	* ( buf++ ) = p->size & 0xFF;
	* ( buf++ ) = (p->size>>8) & 0xFF;
	//
	if ( ( !p->data ) || ( !p->size ) ) {
		return buf;
	}
	
	memcpy ( buf,p->data,p->size );
	buf+=p->size;
	return buf;
}

uint8_t* zxslip_cra_send(uint8_t* buf, zxslip_apkt_send* p) {
	buf = zxslip_addheader ( buf, zxslip_cmd_send, zxslip_getId() );
	* ( buf++ ) = p->exit_code;
	* ( buf++ ) = p->fd_sock;
	* ( buf++ ) = p->flags;
	
	* ( buf++ ) = p->size & 0xFF;
	* ( buf++ ) = (p->size>>8) & 0xFF;
	
	return buf;
}

/** --------------------- Функции парсинга запросов --------------------- */

void zxslip_query_parse ( uint8_t* buf, uint16_t size ) {
	zxslip_pkt_header* h= ( zxslip_pkt_header* ) buf;
	
	// Ошибка косяковна
	if ( size < sizeof ( zxslip_pkt_header ) ) {
		return;
	}

	// Заголовок (он передаётся в функцию обратного вызова всегда)
	buf+=sizeof(zxslip_pkt_header);
	size-=sizeof(zxslip_pkt_header);

	switch ( h->cmd_code ) {

	case zxslip_cmd_gettxtinfo: {
		// Нет параметров запроса
		if( zxslip_q_gettxtinfo_cb ){zxslip_q_gettxtinfo_cb(h);}
		break;
	}
	case zxslip_cmd_esp_poll: {
		// Нет параметров запроса
		if( zxslip_q_esp_poll_cb ){zxslip_q_esp_poll_cb(h);}
		break;
	}

	case zxslip_cmd_wifi_config: {
		zxslip_qpkt_wifi_config p;
		memset(&p, 0, sizeof(p));
		p.auth = *(buf+0);
		p.mode = *(buf+1);
		stpncpy_r(p.pass, stpncpy_r(p.name, buf+2, 0x41)+1, 0x41);
		// Вызов обработчика
		if( zxslip_q_wifi_config_cb ){zxslip_q_wifi_config_cb(h,&p);}
		//
		break;
	}

	case zxslip_cmd_wifi_status: {
		// Нет параметров запроса
		if( zxslip_q_wifi_status_cb ){zxslip_q_wifi_status_cb(h);}
		break;
	}

	case zxslip_cmd_socket: {
		zxslip_qpkt_socket* p= (zxslip_qpkt_socket*)(buf);
		// Вызов обработчика
		if( zxslip_q_socket_cb ){zxslip_q_socket_cb(h,p);}
		//
		break;
	}
	case zxslip_cmd_close: {
		zxslip_qpkt_close* p= (zxslip_qpkt_close*)(buf);
		// Вызов обработчика
		if( zxslip_q_close_cb ){zxslip_q_close_cb(h,p);}
		//
		break;
	}
	case zxslip_cmd_fcntl: {
		zxslip_qpkt_fcntl p;//= (zxslip_qpkt_fcntl*)(buf);
		p.fd_sock = *(buf+0);
		p.cmd_fcntl = *(buf+1);
		p.size = (*(buf+3) << 8) + *(buf+2);
		p.data=buf+4;
		// Вызов обработчика
		if( zxslip_q_fcntl_cb ){zxslip_q_fcntl_cb(h,&p);}
		//
		break;
	}

	case zxslip_cmd_connect: {
		zxslip_qpkt_connect p;
		p.fd_sock= *(buf+0);
		p.adrsize = size-1;
		p.adr=buf+3;
		// Вызов обработчика
		if( zxslip_q_connect_cb ){zxslip_q_connect_cb(h,&p);}
		//
		break;
	}
	case zxslip_cmd_recv: {
		zxslip_qpkt_recv* p= (zxslip_qpkt_recv*)(buf);
		// Вызов обработчика
		if( zxslip_q_recv_cb ){zxslip_q_recv_cb(h,p);}
		//
		break;
	}
	case zxslip_cmd_send: {
		zxslip_qpkt_send p;//= (zxslip_qpkt_recv*)(buf);
		p.fd_sock = *(buf+0);
		p.flags = *(buf+1);
		p.size = size-4;
		p.data = buf+4;
		// Вызов обработчика
		if( zxslip_q_send_cb ){zxslip_q_send_cb(h,&p);}
		//
		break;
	}

	case zxslip_cmd_bind: {
		// Пока не сделано
		break;
	}
	case zxslip_cmd_listen: {
		// Пока не сделано
		break;
	}
	case zxslip_cmd_accept: {
		// Пока не сделано
		break;
	}
	default: {
		// Unknown command!
	}

	}
}

/** --------------------- Функции парсинга ответов --------------------- */
void zxslip_answer_parse(uint8_t* buf, uint16_t size) {
	zxslip_pkt_header* h= ( zxslip_pkt_header* ) buf;
	
	// Ошибка косяковна
	if ( size < sizeof ( zxslip_pkt_header ) ) {
		return;
	}

	// Заголовок (он передаётся в функцию обратного вызова всегда)
	buf+=sizeof(zxslip_pkt_header);
	size-=sizeof(zxslip_pkt_header);

	switch ( h->cmd_code ) {

	case zxslip_cmd_gettxtinfo: {
		zxslip_apkt_gettxtinfo p;
		p.size = size;
		p.text=buf;
		// Вызов обработчика
		if( zxslip_a_gettxtinfo_cb ){zxslip_a_gettxtinfo_cb(h,&p);}
		//
		break;
	}
	case zxslip_cmd_esp_poll: {
		zxslip_apkt_esp_poll* p=(zxslip_apkt_esp_poll*)buf;
		// Вызов обработчика
		if( zxslip_a_esp_poll_cb ){zxslip_a_esp_poll_cb(h,p);}
		//
		break;
	}

	case zxslip_cmd_wifi_config: {
		zxslip_apkt_wifi_config* p=(zxslip_apkt_wifi_config*)buf;
		// Вызов обработчика
		if( zxslip_a_wifi_config_cb ){zxslip_a_wifi_config_cb(h,p);}
		//
		break;
	}

	case zxslip_cmd_wifi_status: {
		zxslip_apkt_wifi_status p;
		memset(&p,0,sizeof(p));
		p.exit_code = *(buf+0);
		p.wifi_status = *(buf+1);
		p.auth = *(buf+2);
		p.mode = *(buf+3);
		stpncpy_r(p.pass, stpncpy_r(p.name, buf+4, 41)+1, 41);
		// Вызов обработчика
		if( zxslip_a_wifi_status_cb){zxslip_a_wifi_status_cb(h,&p);}
		//
		break;
	}

	case zxslip_cmd_socket: {
		zxslip_apkt_socket* p=(zxslip_apkt_socket*)buf;
		// Вызов обработчика
		if( zxslip_a_socket_cb ){zxslip_a_socket_cb(h,p);}
		//
		break;
	}
	case zxslip_cmd_close: {
		zxslip_apkt_close* p=(zxslip_apkt_close*)buf;
		// Вызов обработчика
		if( zxslip_a_close_cb ){zxslip_a_close_cb(h,p);}
		//
		break;
	}
	case zxslip_cmd_fcntl: {
		zxslip_apkt_fcntl p;
		p.exit_code = *(buf+0);
		p.fd_sock = *(buf+1);
		p.cmd_fcntl = *(buf+2);
		p.data = buf+5;
		// Вызов обработчика
		if( zxslip_a_fcntl_cb ){zxslip_a_fcntl_cb(h,&p);}
		//
		break;
	}

	case zxslip_cmd_connect: {
		zxslip_apkt_connect* p=(zxslip_apkt_connect*)buf;
		// Вызов обработчика
		if( zxslip_a_connect_cb ){zxslip_a_connect_cb(h,p);}
		//
		break;
	}
	case zxslip_cmd_recv: {
		zxslip_apkt_recv p;
		p.exit_code = *(buf+0);
		p.fd_sock = *(buf+1);
		p.flags = *(buf+2);
		p.size = size-5;
		p.data = buf+5;
		// Вызов обработчика
		if( zxslip_a_recv_cb ){zxslip_a_recv_cb(h,&p);}
		//
		break;
	}
	case zxslip_cmd_send: {
		zxslip_apkt_send* p=(zxslip_apkt_send*)buf;
		// Вызов обработчика
		if( zxslip_a_send_cb){zxslip_a_send_cb(h,p);}
		//
		break;
	}

	case zxslip_cmd_bind: {
		// Пока не сделано
		break;
	}
	case zxslip_cmd_listen: {
		// Пока не сделано
		break;
	}
	case zxslip_cmd_accept: {
		// Пока не сделано
		break;
	}
	default: {
		// Unknown command!
	}

	}
}
