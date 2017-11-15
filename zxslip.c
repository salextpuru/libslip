#include "zxslip.h"
#include <string.h>

static uint8_t sendpkt_id=0xAB;

uint8_t zxslip_getId(){
	return sendpkt_id;
}

void zxslip_setId(uint8_t id){
	sendpkt_id = id;
}

uint8_t* zxslip_addheader(uint8_t* buf, uint8_t cmd, uint8_t id){
	*(buf++) = 0;
	*(buf++) = cmd;
	*(buf++) = id;
	return buf;
}

uint8_t* zxslip_cr_wifi_config(uint8_t* buf, uint8_t mode, const char* name, const char* pass, uint8_t auth){
	buf = zxslip_addheader(buf, zxslip_cmd_wifi_config, zxslip_getId());
	*(buf++) = mode;
	*(buf++) = auth;
	while(*(buf++) =*(name++)){}
	while(*(buf++) =*(pass++)){}
	return buf;
}

uint8_t* zxslip_cr_socket(uint8_t* buf, int8_t domain, int8_t type, int8_t protocol){
	buf = zxslip_addheader(buf, zxslip_cmd_socket, zxslip_getId());
	*(buf++) = domain;
	*(buf++) = type;
	*(buf++) = protocol;
	return buf;
}

uint8_t* zxslip_cr_close(uint8_t* buf, int8_t fd){
	buf = zxslip_addheader(buf, zxslip_cmd_close, zxslip_getId());
	*(buf++) = fd;
	return buf;
}

uint8_t* zxslip_cr_fcntl(uint8_t* buf, int fd, int cmd, uint8_t* data, uint16_t size){
	buf = zxslip_addheader(buf, zxslip_cmd_fcntl, zxslip_getId());
	*(buf++) = fd;
	*(buf++) = cmd;
	//
	if( (!data) || (!size)){
		return buf;
	}
	
	memcpy(buf,data,size);
	buf+=size;
	return buf;
}

uint8_t* zxslip_cr_connect(uint8_t* buf, int8_t fd, uint8_t *addr, int16_t addrlen){
	buf = zxslip_addheader(buf, zxslip_cmd_connect, zxslip_getId());
	*(buf++) = fd;
	memcpy(buf,addr,addrlen);
	buf+=addrlen;
	return buf;
}

uint8_t* zxslip_cr_recv(uint8_t* buf, int8_t fd, uint16_t len, int8_t flags){
	buf = zxslip_addheader(buf, zxslip_cmd_recv, zxslip_getId());
	*(buf++) = fd;
	*(buf++) = flags;
	*(buf++) = len&0xff;
	*(buf++) = (len>>8)&0xff;
	return buf;
}

uint8_t* zxslip_cr_send(uint8_t* buf, int8_t fd, uint8_t* data, uint16_t len, int8_t flags){
	buf = zxslip_addheader(buf, zxslip_cmd_send, zxslip_getId());
	*(buf++) = fd;
	*(buf++) = flags;
	*(buf++) = len&0xff;
	*(buf++) = (len>>8)&0xff;
	memcpy(buf,data,len);
	buf+=len;
	return buf;
}
