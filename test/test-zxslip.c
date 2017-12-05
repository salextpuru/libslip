#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>

#include "zxslip.h"

void dump(uint8_t* buf, uint16_t size ){
	int line=0;
	printf("\n");
	while( size-- ){
		printf("0x%.2X ", *(buf++) );
		if( (++line) >= 0x10 ){
			line=0;
			printf("\n");
		}
	}
	printf("\n");
}

void dumpp(uint8_t* buf, uint8_t* ebuf){
	int line=0;
	printf("\n");
	while( buf!=ebuf ){
		printf("0x%.2X ", *(buf++) );
		if( (++line) >= 0x10 ){
			line=0;
			printf("\n");
		}
	}
	printf("\n");
}

/**---- Буфера запросов и ответов ---- */

static uint8_t qbuf[0x400];
static uint8_t abuf[0x400];

/**---- Тесты ---- */

static int test_gettxtinfo(){
	printf("---- gettxtinfo ----\n");
	
	// query have no data
	zxslip_apkt_gettxtinfo answer_s;
	zxslip_apkt_gettxtinfo answer_r;
	
	printf("---- query (no query data)----\n");
	uint8_t* ebuf=zxslip_crq_gettxtinfo(qbuf);
	dumpp(qbuf,ebuf);
	
	printf("---- answer (no query data)----\n");
	printf("---- Send:\n");
	answer_s.text="Test of debug gettxtinfo";
	answer_s.size=strlen(answer_s.text)+1;
	ebuf=zxslip_cra_gettxtinfo(qbuf,&answer_s);
	printf("answer.text=%s\nanswer.size=0x%X\n",answer_s.text,answer_s.size);
	dumpp(qbuf,ebuf);
	printf("---- Recv:\n");
	
	
	
	return 0;
}

/**--------------------------------------------------------------- */

int main ( int argc, char **argv ) {
	test_gettxtinfo();
	
	/*
	ebuf=zxslip_cr_gettxtinfo(cmdbuf);
	printf("\nzxslip_cr_gettxtinfo:");dumpp(cmdbuf,ebuf);
	
	ebuf=zxslip_cr_esp_poll(cmdbuf);
	printf("\nzxslip_cr_esp_poll:");dumpp(cmdbuf,ebuf);
	//
	ebuf=zxslip_cr_wifi_config(cmdbuf,0x55,"0WiFiName0","1WifiPass1",0xAA);
	printf("\nzxslip_cr_wifi_config:");dumpp(cmdbuf,ebuf);
	
	ebuf=zxslip_cr_wifi_status(cmdbuf);
	printf("\nzxslip_cr_wifi_status:");dumpp(cmdbuf,ebuf);
	//
	ebuf=zxslip_cr_socket(cmdbuf,0x01,0x02,0x03);
	printf("\nzxslip_cr_socket:");dumpp(cmdbuf,ebuf);
	
	ebuf=zxslip_cr_close(cmdbuf,0x04);
	printf("\nzxslip_cr_close:");dumpp(cmdbuf,ebuf);
	
	ebuf=zxslip_cr_fcntl(cmdbuf,0x04, 0xA5, 0, 0);
	printf("\nzxslip_cr_fcntl (null):");dumpp(cmdbuf,ebuf);
	
	ebuf=zxslip_cr_fcntl(cmdbuf,0x04, 0xA5, "12345", 5);
	printf("\nzxslip_cr_fcntl (par):");dumpp(cmdbuf,ebuf);
	//
	ebuf=zxslip_cr_connect(cmdbuf,0x04,"\xC0\xA8\x01\x02\x20\x40",6);
	printf("\nzxslip_cr_connect:");dumpp(cmdbuf,ebuf);
	
	ebuf=zxslip_cr_recv(cmdbuf,0x04,0xABCD,0xFF);
	printf("\nzxslip_cr_recv:");dumpp(cmdbuf,ebuf);
	
	ebuf=zxslip_cr_send(cmdbuf,0x04, "0123456", 7, 0xFF);
	printf("\nzxslip_cr_send:");dumpp(cmdbuf,ebuf);
	*/
	
	return 0;
}

