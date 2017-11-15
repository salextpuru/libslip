#include "slip.h"

enum {
	// Wait for END (0xC0)
	stayWEND = 0,
	// Wait for BEGIN  (not 0xC0)
	stayWBEGIN = 1,
	// DB recieved
	stay0xDB = 2,
	// recieve byte
	stayRecv = 3
};

void slip_recv_init ( slip_recv_struct* rs, slip_data_cb cb, slip_rec_be b, slip_rec_be e ) {
	rs->status = stayWEND;
	rs->callback = cb;
	rs->bcb = b;
	rs->ecb = e;
}

uint8_t slip_recv_byte ( slip_recv_struct* rs, uint8_t c ) {
	// End or begin
	switch ( rs->status ) {
	case stayWEND: {
		if ( c == 0xC0 ) {
			rs->status = stayWBEGIN;
		}
		return 0;
	}
	case stayWBEGIN: {
		if ( c == 0xC0 ) {
			return 0;
		}
		// Callback on begin packet
		if ( rs->bcb ) {
			rs->bcb();
		}
		//
		rs->status = stayRecv;
		break;
	}
	default:
		;
	}
	// Byte must be received
	switch ( rs->status ) {
	case stay0xDB: {
		switch ( c ) {
		case 0xDC: {
			c = 0xC0;
			rs->status = stayRecv;
			break;
		}
		case 0xDD: {
			c = 0xDB;
			rs->status = stayRecv;
			break;
		}
		default: {
			// Error!
			rs->status = stayWEND;
			return 1;
		}
		}
		break;
	}
	case stayRecv: {
		switch ( c ) {
		case 0xDB: {
			rs->status = stay0xDB;
			return 0;
		}
		case 0xC0: {
			// Callback on end packet
			if ( rs->ecb ) {
				rs->ecb();
			}
			rs->status = stayWBEGIN;
			return 0;
		}
		default:
			;
		}
		break;
	}
	default:
		;
	}
	// Callback with decode
	if ( rs->callback ) {
		rs->callback ( c );
	}
}

uint8_t slip_recv_buf(uint8_t* buf, uint16_t size, slip_recv_struct* rs){
	while(size--){
		slip_recv_byte(rs, *(buf++));
	}
}

void slip_send_byte(uint8_t c, slip_data_cb cb){
	if(!cb) return;
	//
	switch(c){
		case 0xC0:{
			cb(0xDB);
			cb(0xDC);
			break;
		}
		case 0xDB:{
			cb(0xDB);
			cb(0xDD);
			break;
		}
		default:{
			cb(c);
		}
	}
}

void slip_send_startend(slip_data_cb cb){
	if(cb){
		cb(0xC0);
	}
}

void slip_send_buf(uint8_t* buf, uint16_t size, slip_data_cb cb){
	slip_send_startend(cb);
	while(size--){
		slip_send_byte(*(buf++),cb);
	}
	slip_send_startend(cb);
}
