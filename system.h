/*
 * system.h
 *
 * Author: marko
 *
 */

#ifndef MY_SYSTEM_H_
#define MY_SYSTEM_H_

#define TRUE 				1
#define FALSE 				!TRUE
#define DEV_SER 			"/dev/ser2"
#define RX_BUFFER_SIZE		100
#define SECONDS(t)			t
#define MINUTES(t)			t*60
#define UART_1_INIT_CMD		"devc-seromap -u2 -F -b115200 -c48000000/16 0x48022000^2,74"
#define USB_TTL_INIT_CMD	"devc-usb"

// Serial UART functions
int serial_init(void);
int serial_read();
void serial_write(char* msg);
void serial_clean_up(void);

// Threading functions
void *serial_thread_A(void *v);
void *serial_thread_B(void *v);

// Timer functions
void send_timed_msg(char *msg);

#endif /* MY_SYSTEM_H_ */
