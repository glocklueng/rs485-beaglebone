/*
 * timers.c
 *
 * Author: marko
 *
 */

#include "system.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <sys/siginfo.h>
#include <sys/neutrino.h>

// messages
#define MT_WAIT_DATA	2		// message from client
#define MT_SEND_DATA	3		// message to client

// pulses
#define CODE_TIMER		1		// pulse from timer

// message reply definitions
#define MT_OK					// message to client
#define MT_TIMEDOUT				// message to client

// message structure
typedef struct {
	int messageType;			// contatns both message to and from client
	int messageData;			// optional data, depends od message
} ClientMessageT;

typedef union {
	ClientMessageT msg; 		// a massage can be either from a client, or
	struct _pulse pulse;		// a pulse
} MessageT;

#define MAX_CLIENT	16			// maximum number of simultaneous clients

struct {
	int in_use;					// is this client entry in use?
	int rcvid;					// receive ID of client
	int timeout;				// timeout left for client
} clients[MAX_CLIENT];			// client table

int chid;						// channel ID
int debug = 1;					// 1 == enabled, 0 == disabled
char *progname = "Thread B";

static void setup_timer(int tv_sec_val,
						int tv_nsec_val,
						int tv_sec_intvl,
						int tv_nsec_intvl,
						struct itimerspec *my_timer);
static void got_msg(int rcvid, ClientMessageT *msg);
static void got_pulse(void);
static void init_pulse_and_timer(void);


static void init_pulse_and_timer(void) {
	timer_t timerid;			// timer ID for timer
	struct sigevent event;		// event to deliver
	struct itimerspec timer;	// timer data structure
	int	coid;					// connection back to myself
	int ctrid;

	// create a connection back to myself
	coid = ConnectAttach(0, 0, chid, 0, 0);
	if(coid == -1) {
		fprintf(stderr, "%s: couldn't create connection!\n", progname);
		perror(NULL);
		exit(EXIT_FAILURE);
	}

	// set up the kind of event that we want to deliver - a pulse
	SIGEV_PULSE_INIT(&event, coid, SIGEV_PULSE_PRIO_INHERIT, CODE_TIMER, 0);

	// create the timer, binding it to the event
	ctrid = timer_create(CLOCK_REALTIME, &event, &timerid);
	if(ctrid == -1) {
		fprintf(stderr, "%s: couldn't create a timer, errno %d\n", progname, errno);
		perror(NULL);
		exit(EXIT_FAILURE);
	}
	setup_timer(1, 0, 1, 0, &timer);
	timer_settime(timerid, 0, &timer, NULL);
}

static void got_pulse(void) {


}

static void got_msg(int rcvid, ClientMessageT *msg) {


}

static void setup_timer(int tv_sec_val,
						int tv_nsec_val,
						int tv_sec_intvl,
						int tv_nsec_intvl,
						struct itimerspec *my_timer) {
	my_timer->it_value.tv_sec = tv_sec_val;
	my_timer->it_value.tv_nsec = tv_nsec_val;
	my_timer->it_interval.tv_sec = tv_sec_intvl;
	my_timer->it_interval.tv_nsec = tv_nsec_intvl;
}

void send_timed_msg(char *sendmsg) {
	int rcvid;
	MessageT msg;

	chid = ChannelCreate(0);
	if( chid  == -1 ) {
		fprintf(stderr, "%s: couldn't create channel!\n", progname);
		perror(NULL);
		exit(EXIT_FAILURE);
	}
	init_pulse_and_timer();

	// constant checking for a pulse or message
	while(1) {
		rcvid = MsgReceive(chid, &msg, sizeof(msg), NULL);
		if(rcvid == 0) {
			got_pulse(); 				// received a pulse
		} else {
			got_msg(rcvid, &msg.msg);	// received a message
		}
	}
}




















