#ifndef __SLIP_H__
#define __SLIP_H__

#include <stdint.h>

// unSLIP callback recv and send byte
typedef uint8_t(*slip_data_cb )(uint8_t);
// unSLIP callback begin and end
typedef uint8_t(*slip_rec_be)();

// Recv struct
typedef struct{
	// current status
	uint8_t		status;
	
	// callback on recv char
	slip_data_cb	callback;
	
	// begin callback
	slip_rec_be	bcb;
	
	// end callback
	slip_rec_be	ecb;
	
} slip_recv_struct;

// Init SLIP protocol (recv)
void slip_recv_init(slip_recv_struct* rs, slip_data_cb cb, slip_rec_be b, slip_rec_be e );

// Recv one SLIP-byte
uint8_t slip_recv_byte(slip_recv_struct* rs, uint8_t c);

// Recieve buffer
uint8_t slip_recv_buf(uint8_t* buf, uint16_t size, slip_recv_struct* rs);

// Send one byte
void slip_send_byte(uint8_t c, slip_data_cb cb);

// Start/End condition
void slip_send_startend(slip_data_cb cb);

// Send buffer with translate
void slip_send_buf(uint8_t* buf, uint16_t size, slip_data_cb cb);

#endif /* __SLIP_H__ */
