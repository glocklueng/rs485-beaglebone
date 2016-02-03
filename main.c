/*
 * main.c
 *
 * Author: marko
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <sys/neutrino.h>
#include <pthread.h>
#include "system.h"

int main(int argc, char *argv[]) {
	pthread_t tid;

	printf("Main entry point\n");
	//pthread_create(&tid, NULL, serial_thread_A, NULL);
	pthread_create(&tid, NULL, serial_thread_B, NULL);
	printf("Thread id %d\n", tid);
	pthread_join(tid, NULL);
	printf("Main exit\n");
	return EXIT_SUCCESS;
}
