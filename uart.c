/*
 * uart.c
 *
 * Author: marko
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "system.h"

static char ser_rx_buffer[RX_BUFFER_SIZE] = {'\0'};
static size_t buff_size_t = RX_BUFFER_SIZE;

static void clear_rx_buffer(void) {
	int i;
	for(i = 0; i < RX_BUFFER_SIZE; i++){
		ser_rx_buffer[i] = 0x00;
	}
}

void serial_clean_up(void) {
	// code
	// maybe close some files, de-init drivers, clear buffers etc.
}

int serial_init(void) {
	int sysres = 0;

	if( access(DEV_SER, F_OK) != -1 ){
		fprintf(stdout, "%s already exists\ncontinuing...\n", DEV_SER);
	} else {
		fprintf(stdout, "Starting UART 1\n");
		sysres = system(UART_1_INIT_CMD);
		if(sysres == -1) {
			fprintf(stdout, "Unable to start UART 1 (%d)\n", sysres);
		} else {
			fprintf(stdout, "UART 1 started on %s [115200/8/N/1] (%d)\n", DEV_SER, sysres);
		}
	}
	return sysres;
}

void serial_write(char *msg) {
	FILE *port;
	
	port = fopen(DEV_SER, "w");
	if(port == NULL){
		fprintf(stderr, "Failed to open %s.\r\n", DEV_SER);
	} else {
		fprintf(port, msg);
		fflush(port);
	}
	fclose(port);
}

int serial_read(void) {
	int port, bytes_read = 0;
	port = open(DEV_SER, O_RDONLY);

	if(port == NULL){
		fprintf(stderr, "Unable to open serial port %s.\r\n", DEV_SER);
		return -1;
	} else {
		while(bytes_read < RX_BUFFER_SIZE){
			bytes_read = read(port, ser_rx_buffer, buff_size_t);
		}
	}
	fprintf(stdout, "RX buffer: %s (%d)\r\n", ser_rx_buffer, bytes_read);
	close(port);
	clear_rx_buffer();
	return bytes_read;
}
