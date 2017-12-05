#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>

#include "slip.h"
#include "slipcrc8.h"

uint8_t sendbuf[0x100];
uint8_t slipbuf[0x200];
uint8_t recbuf[0x100];

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

void fill_sendbuf(){
	int fd=open("/dev/urandom",O_RDONLY);
	read(fd, sendbuf, sizeof(sendbuf) );
	close(fd);
}

// SLIP buffer!
uint16_t slipbufc;

uint8_t toSLIPcb(uint8_t c){
	slipbuf[slipbufc++] =c;
}

// SLIP rec
slip_recv_struct	sr;

uint8_t slbegin(){
	printf("SLBEGIN detected!\n");
}
uint8_t slend(){
	printf("SLEND detected!\n");
}
uint16_t recbufc;
uint8_t slrecv(uint8_t c){
	recbuf[recbufc++]=c;
}

int main ( int argc, char **argv ) {
	printf("- test-slip -\n");
	//
	fill_sendbuf();
	sendbuf[sizeof(sendbuf)-1] = slipcrc8buf(sendbuf, sizeof(sendbuf)-1);
	dump(sendbuf, sizeof(sendbuf));
	//
	slipbufc=0;
	slip_send_buf( sendbuf, sizeof(sendbuf), toSLIPcb );	
	dump(slipbuf, slipbufc);
	//
	recbufc=0;
	slip_recv_init(&sr, slrecv, slbegin, slend );
	slip_recv_buf(slipbuf, slipbufc, &sr);
	dump(recbuf, recbufc);
	printf("CRC8RECV=%.2X\n",slipcrc8buf(recbuf, recbufc-1));
	printf("CRC8RECVBUF=%.2X\n",recbuf[recbufc-1]);
	// diff
	int i;
	for( i=0; i<recbufc; i++ ){
		if( sendbuf[i] != recbuf[i] ){
			printf("Diff pos %.4X sendbuf %.2X rec %.2X\n",i,sendbuf[i], recbuf[i]);
		}
	}
	//
	return 0;
}

