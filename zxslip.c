#include "zxslip.h"
#include <string.h>

static uint8_t sendpkt_id=0xAB;

// Функция копирования с возвратом на конец строки
static uint8_t* stpncpy_r(uint8_t *dest, uint8_t *src, uint16_t n){
	while( (*src) && (n--) ){
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

uint8_t* zxslip_cr_wifi_config ( uint8_t* buf, uint8_t mode, const char* name, const char* pass, uint8_t auth ) {
	buf = zxslip_addheader ( buf, zxslip_cmd_wifi_config, zxslip_getId() );
	* ( buf++ ) = mode;
	* ( buf++ ) = auth;
	while ( * ( buf++ ) =* ( name++ ) ) {}
	while ( * ( buf++ ) =* ( pass++ ) ) {}
	return buf;
}

uint8_t* zxslip_cr_socket ( uint8_t* buf, int8_t domain, int8_t type, int8_t protocol ) {
	buf = zxslip_addheader ( buf, zxslip_cmd_socket, zxslip_getId() );
	* ( buf++ ) = domain;
	* ( buf++ ) = type;
	* ( buf++ ) = protocol;
	return buf;
}

uint8_t* zxslip_cr_close ( uint8_t* buf, int8_t fd ) {
	buf = zxslip_addheader ( buf, zxslip_cmd_close, zxslip_getId() );
	* ( buf++ ) = fd;
	return buf;
}

uint8_t* zxslip_cr_fcntl ( uint8_t* buf, int fd, int cmd, uint8_t* data, uint16_t size ) {
	buf = zxslip_addheader ( buf, zxslip_cmd_fcntl, zxslip_getId() );
	* ( buf++ ) = fd;
	* ( buf++ ) = cmd;
	//
	if ( ( !data ) || ( !size ) ) {
		return buf;
	}

	memcpy ( buf,data,size );
	buf+=size;
	return buf;
}

uint8_t* zxslip_cr_connect ( uint8_t* buf, int8_t fd, uint8_t *addr, int16_t addrlen ) {
	buf = zxslip_addheader ( buf, zxslip_cmd_connect, zxslip_getId() );
	* ( buf++ ) = fd;
	memcpy ( buf,addr,addrlen );
	buf+=addrlen;
	return buf;
}

uint8_t* zxslip_cr_recv ( uint8_t* buf, int8_t fd, uint16_t len, int8_t flags ) {
	buf = zxslip_addheader ( buf, zxslip_cmd_recv, zxslip_getId() );
	* ( buf++ ) = fd;
	* ( buf++ ) = flags;
	* ( buf++ ) = len&0xff;
	* ( buf++ ) = ( len>>8 ) &0xff;
	return buf;
}

uint8_t* zxslip_cr_send ( uint8_t* buf, int8_t fd, uint8_t* data, uint16_t len, int8_t flags ) {
	buf = zxslip_addheader ( buf, zxslip_cmd_send, zxslip_getId() );
	* ( buf++ ) = fd;
	* ( buf++ ) = flags;
	* ( buf++ ) = len&0xff;
	* ( buf++ ) = ( len>>8 ) &0xff;
	memcpy ( buf,data,len );
	buf+=len;
	return buf;
}


/** --------------------- Функции парсинга запросов --------------------- */

void zxslip_query_parse ( uint8_t* buf, uint16_t size ) {
	// Ошибка косяковна
	if ( size < sizeof ( zxslip_pkt_header ) ) {
		return;
	}

	// Заголовок (он передаётся в функцию обратного вызова всегда)
	zxslip_pkt_header* h= ( zxslip_pkt_header* ) buf;
	buf+=sizeof(zxslip_pkt_header);
	size-=sizeof(zxslip_pkt_header);

	switch ( h->cmd_code ) {

	case zxslip_cmd_gettxtinfo: {
		// Нет параметров запроса
		break;
	}
	case zxslip_cmd_esp_poll: {
		// Нет параметров запроса
		break;
	}

	case zxslip_cmd_wifi_config: {
		zxslip_qpkt_wifi_config p;
		memset(&p, 0, sizeof(p));
		p.auth = *(buf+0);
		p.mode = *(buf+1);
		stpncpy_r(p.pass, stpncpy_r(p.name, buf+2, 0x41)+1, 0x41);
		// Вызов обработчика
		
		//
		break;
	}

	case zxslip_cmd_wifi_status: {
		// Нет параметров запроса
		break;
	}

	case zxslip_cmd_socket: {
		zxslip_qpkt_socket* p= (zxslip_qpkt_socket*)(buf);
		// Вызов обработчика
		
		//
		break;
	}
	case zxslip_cmd_close: {
		zxslip_qpkt_close* p= (zxslip_qpkt_close*)(buf);
		// Вызов обработчика
		
		//
		break;
	}
	case zxslip_cmd_fcntl: {
		zxslip_qpkt_fcntl* p= (zxslip_qpkt_fcntl*)(buf);
		p->data=buf+2;
		// Вызов обработчика
		
		//
		break;
	}

	case zxslip_cmd_connect: {
		zxslip_qpkt_connect p;
		p.fd_sock= *(buf+0);
		p.adrsize = size-1;
		p.adr=buf+3;
		// Вызов обработчика
		
		//
		break;
	}
	case zxslip_cmd_recv: {
		zxslip_qpkt_recv* p= (zxslip_qpkt_recv*)(buf);
		// Вызов обработчика
		
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
	default:{
		// Unknown command!
	}
	
	}
}

/** --------------------- Функции парсинга ответов --------------------- */
void zxslip_answer_parse(uint8_t* buf, uint16_t size){
	// Ошибка косяковна
	if ( size < sizeof ( zxslip_pkt_header ) ) {
		return;
	}

	// Заголовок (он передаётся в функцию обратного вызова всегда)
	zxslip_pkt_header* h= ( zxslip_pkt_header* ) buf;
	buf+=sizeof(zxslip_pkt_header);
	size-=sizeof(zxslip_pkt_header);

	switch ( h->cmd_code ) {

	case zxslip_cmd_gettxtinfo: {
		zxslip_apkt_gettxtinfo p;
		p.size = size;
		p.text=buf;
		// Вызов обработчика 
		
		//
		break;
	}
	case zxslip_cmd_esp_poll: {
		zxslip_apkt_esp_poll* p=(zxslip_apkt_esp_poll*)buf;
		// Вызов обработчика 
		
		//
		break;
	}

	case zxslip_cmd_wifi_config: {
		zxslip_apkt_wifi_config* p=(zxslip_apkt_wifi_config*)buf;
		// Вызов обработчика 
		
		//
		break;
	}

	case zxslip_cmd_wifi_status: {
		zxslip_apkt_wifi_status p;
		memset(&p,0,sizeof(p));
		p.exit_code = *(buf+0);
		p.status = *(buf+1);
		p.auth = *(buf+2);
		p.mode = *(buf+3);
		stpncpy_r(p.pass, stpncpy_r(p.name, buf+4, 41)+1, 41);
		// Вызов обработчика 
		
		//
		break;
	}

	case zxslip_cmd_socket: {
		zxslip_apkt_socket* p=(zxslip_apkt_socket*)buf;
		// Вызов обработчика 
		
		//
		break;
	}
	case zxslip_cmd_close: {
		zxslip_apkt_close* p=(zxslip_apkt_close*)buf;
		// Вызов обработчика 
		
		//
		break;
	}
	case zxslip_cmd_fcntl: {
		zxslip_apkt_fcntl p;
		p.exit_code = *(buf+0);
		p.fd_sock = *(buf+1);
		p.cmd_fcntl = *(buf+2);
		p.data = buf+3;
		// Вызов обработчика 
		
		//
		break;
	}

	case zxslip_cmd_connect: {
		zxslip_apkt_connect* p=(zxslip_apkt_connect*)buf;
		// Вызов обработчика 
		
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
		
		//
		break;
	}
	case zxslip_cmd_send: {
		zxslip_apkt_send* p=(zxslip_apkt_send*)buf;
		// Вызов обработчика 
		
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
	default:{
		// Unknown command!
	}
	
	}
}
