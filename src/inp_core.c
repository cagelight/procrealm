#include "inp.h"

pthread_t inp_thrd;
com_thrd_status_ta inp_thrd_status;
char inp_thrd_err [COM_THRD_ERRLEN];

static void * inp_run(void * arg) {
	assert(arg == NULL);
	
	target_sleeper_t inp_slp;
	target_sleeper_init(&inp_slp);
	target_sleeper_set_target_FPS(&inp_slp, 250);
	
	int glfwsc;
	do {
		target_sleeper_sleep_for_target(&inp_slp);
		
		pthread_mutex_lock(&com_win_mut);
		glfwsc = glfwWindowShouldClose(com_win);
		pthread_mutex_unlock(&com_win_mut);
	} while (!glfwsc);
	INPEXIT;
}

int inp_init() {
	atomic_init(&inp_thrd_status, COM_THRD_STATUS_RUNNING);
	pthread_create(&inp_thrd, &com_thrd_attr, inp_run, NULL);
	return 0;
}