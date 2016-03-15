#include "com.h"
#include "inp.h"
#include "rnd.h"

#include "util.h"

com_signal_ta com_signal;
pthread_attr_t com_thrd_attr;

GLFWwindow * com_win;
pthread_mutex_t com_win_mut;

void com_init() {
	com_util_init();
	
	if (glfwInit() != GL_TRUE) {
		com_print("glfw failed to init");
		exit(1);
	}
	
	com_win = glfwCreateWindow(800, 600, "Test", NULL, NULL);
	if (!com_win) {
		com_print("glfw failed to create window");
		exit(1);
	}
	
	pthread_mutex_init(&com_win_mut, NULL);
	pthread_attr_setdetachstate(&com_thrd_attr, PTHREAD_CREATE_DETACHED);
	
	atomic_init(&com_signal, COM_SIGNAL_RUN);
	
	rnd_init();
	inp_init();
}

void com_run() {
	enum thrd_bits_e {
		THRD_BIT_INP = 1 << 0,
		THRD_BIT_RND = 1 << 1,
	};
	
	int thrd_stati = THRD_BIT_INP | THRD_BIT_RND;
	timekeeper_t tsp;
	timekeeper_init(&tsp);
	timekeeper_set_target_FPS(&tsp, 10);
	
	while(thrd_stati) {
		switch(COMSIG) {
		case COM_SIGNAL_RUN:
			break;
		case COM_SIGNAL_EXIT:
			pthread_mutex_lock(&com_win_mut);
			glfwSetWindowShouldClose(com_win, GL_TRUE);
			pthread_mutex_unlock(&com_win_mut);
			break;
		}
		if (thrd_stati & THRD_BIT_INP) switch(INPSTAT) {
		case THRD_STATUS_RUNNING:
			break;
		case THRD_STATUS_EXIT_NORMAL:
		case THRD_STATUS_EXIT_ERROR:
			atomic_store(&com_signal, COM_SIGNAL_EXIT);
			thrd_stati &= ~THRD_BIT_INP;
			break;
		}
		
		if (thrd_stati & THRD_BIT_RND) switch(RNDSTAT) {
		case THRD_STATUS_RUNNING:
			break;
		case THRD_STATUS_EXIT_NORMAL:
		case THRD_STATUS_EXIT_ERROR:
			atomic_store(&com_signal, COM_SIGNAL_EXIT);
			thrd_stati &= ~THRD_BIT_RND;
			break;
		}
		timekeeper_sleep_for_target(&tsp);
	}
	
	if (INPSTAT == THRD_STATUS_EXIT_ERROR) {
		com_printf("input thread exited with error: %s\n", inp_thrd_err);
	}
	if (RNDSTAT == THRD_STATUS_EXIT_ERROR) {
		com_printf("render thread exited with error: %s\n", rnd_thrd_err);
	}
}
