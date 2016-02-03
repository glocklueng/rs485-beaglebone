/*
 * threading.c
 *
 * Author: marko
 *
 */

#include "system.h"
#include <stdio.h>

void *serial_thread_A(void *v) {
	char c[5];
	c[0] = 0;

	printf("Serial thread A started\n");
	serial_init();
	while(c[0] != 'n' && c[0] != 'N') {
		fprintf(stdout, "Send message? [Y/n]: ");
		fgets(c, 5, stdin);

		if(c[0] == 'Y' || c[0] == 'y'){
			serial_write("(A) hello!\n\r");
			c[0] = 0;
		} else if(c[0] == 'N' || c[0] == 'n') {
			serial_clean_up();
		}
	}
	printf("Serial thread A ended\n");
	return (0);
}

void *serial_thread_B(void *v) {
	int i = 0;
	printf("Serial thread B started\n");
	serial_init();
	send_timed_msg("(B) hello\n\r");
	printf("Serial thread B ended\n");
	return (0);
}
