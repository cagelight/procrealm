#include "com.h"

#include <signal.h>

static void catch_interrupt (int sig) {
	assert(sig == SIGINT);
	if (atomic_load(&com_signal) == COM_SIGNAL_EXIT) abort();
	atomic_store(&com_signal, COM_SIGNAL_EXIT);
	com_print("interrupt signal detected, exit flag set");
}

int main() {
	signal(SIGINT, catch_interrupt);
	int status = com_init();
	if (status) {
		com_printf("initialization failed: code %i\n", status);
		return status;
	}
	return com_run();
}
