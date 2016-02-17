#include "com.h"
#include "inp.h"
#include "rnd.h"

com_signal_ta com_signal;
pthread_attr_t com_thrd_attr;

GLFWwindow * com_win;
pthread_mutex_t com_win_mut;

int com_init() {
	com_util_init();
	
	if (glfwInit() != GL_TRUE) {
		com_print("glfw failed to init");
		return 1;
	}
	
	com_win = glfwCreateWindow(800, 600, "Test", NULL, NULL);
	if (!com_win) {
		com_print("glfw failed to create window");
		return 1;
	}
	
	pthread_mutex_init(&com_win_mut, NULL);
	pthread_attr_setdetachstate(&com_thrd_attr, PTHREAD_CREATE_DETACHED);
	
	atomic_init(&com_signal, COM_SIGNAL_RUN);
	int status = 0;
	
	status = rnd_init();
	if (status) return status;
	status = inp_init();
	if (status) return status;
	
	return 0;
}

int com_run() {
	enum thrd_bits_e {
		THRD_BIT_INP = 1 << 0,
		THRD_BIT_RND = 1 << 1,
	};
	
	int thrd_stati = THRD_BIT_INP | THRD_BIT_RND;
	target_sleeper_t tsp;
	target_sleeper_init(&tsp);
	target_sleeper_set_target_FPS(&tsp, 10);
	
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
		case COM_THRD_STATUS_RUNNING:
			break;
		case COM_THRD_STATUS_EXIT_NORMAL:
		case COM_THRD_STATUS_EXIT_ERROR:
			atomic_store(&com_signal, COM_SIGNAL_EXIT);
			thrd_stati &= ~THRD_BIT_INP;
			break;
		}
		
		if (thrd_stati & THRD_BIT_RND) switch(RNDSTAT) {
		case COM_THRD_STATUS_RUNNING:
			break;
		case COM_THRD_STATUS_EXIT_NORMAL:
		case COM_THRD_STATUS_EXIT_ERROR:
			atomic_store(&com_signal, COM_SIGNAL_EXIT);
			thrd_stati &= ~THRD_BIT_RND;
			break;
		}
		target_sleeper_sleep_for_target(&tsp);
	}
	
	int eflag = 0;
	if (INPSTAT == COM_THRD_STATUS_EXIT_ERROR) {
		eflag |= THRD_BIT_INP;
		com_printf("input thread exited with error: %s\n", inp_thrd_err);
	}
	if (RNDSTAT == COM_THRD_STATUS_EXIT_ERROR) {
		eflag |= THRD_BIT_RND;
		com_printf("render thread exited with error: %s\n", rnd_thrd_err);
	}
	return eflag;
}