/*
 * An simple example for how to use libevent
 *
 * # gcc -levent hello_world.c -o hello_world
 *
 */
#include <sys/signal.h>
#include <event.h>
#include <stdlib.h>

// signal event callback
void signal_cb(int fd, short event, void *argc)
{
	struct event_base *base = (struct event_base *)argc;
	struct timeval delay = {2, 0};

	// just for display, it's bad choice to call printf in signal handler
	printf("Caught an interrupt signal, exiting cleanly in 2 seconds.\n");

	// exit the event handle loop
	event_base_loopexit(base, &delay);
}

// timeout event callback
void timeout_cb(int fd, short event, void *argc)
{
	printf("timeout\n");
}

int main()
{
	struct event *signal_event;
	struct event *timeout_event;
	struct timeval tv = {1, 0};
	struct event_base *base = event_init();

	if (!base) {
		printf("event_init failed\n");
		exit(-1);
	}

	// alloc and register a signal event
	signal_event = evsignal_new(base, SIGINT, signal_cb, base);
	if (!signal_event) {
		printf("evsignal_new failed\n");
		goto err1;

	}
	event_add(signal_event, NULL);

	// alloc and register a timeout event
	timeout_event = evtimer_new(base, timeout_cb, NULL);
	if (!timeout_event) {
		printf("evtimer_new failed\n");
		goto err2;
	}
	event_add(timeout_event, NULL);

	// Now, wait and handle loop
	event_base_dispatch(base);

err2:
	event_free(timeout_event);
err1:
	event_free(signal_event);
	event_base_free(base);
}
