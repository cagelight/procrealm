#include "rnd.h"

pthread_t rnd_thrd;
com_thrd_status_ta rnd_thrd_status;
char rnd_thrd_err [COM_THRD_ERRLEN];

//================================================================

static void rnd_frame() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//================================================================

static void rnd_glinit() {
	glClearColor(1, 0, 0, 1);
}

//================================================================

static void * rnd_run(void * arg) {
	assert(arg == NULL);
	pthread_mutex_lock(&com_win_mut);
	glfwMakeContextCurrent(com_win);
	pthread_mutex_unlock(&com_win_mut);
	
	target_sleeper_t rnd_slp;
	target_sleeper_init(&rnd_slp);
	target_sleeper_set_target_FPS(&rnd_slp, 60);
	
	rnd_glinit();
	
	int glfwsc;
	do {
		rnd_frame();
		
		pthread_mutex_lock(&com_win_mut);
		glfwSwapBuffers(com_win);
		pthread_mutex_unlock(&com_win_mut);
		
		target_sleeper_sleep_for_target(&rnd_slp);
		
		pthread_mutex_lock(&com_win_mut);
		glfwsc = glfwWindowShouldClose(com_win);
		pthread_mutex_unlock(&com_win_mut);
	} while (!glfwsc);
	RNDEXIT;
}

//================================================================

int rnd_init() {
	if (glewInit() != GL_TRUE) {
		com_print("GLEW failed to initialize");
		return 1;
	}
	atomic_init(&rnd_thrd_status, COM_THRD_STATUS_RUNNING);
	pthread_create(&rnd_thrd, &com_thrd_attr, rnd_run, NULL);
	return 0;
}