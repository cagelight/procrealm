#include "rnd.h"

pthread_t rnd_thrd;
thrd_status_ta rnd_thrd_status;
char rnd_thrd_err [THRD_ERRLEN];

//================================================================

static void rnd_frame() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glUseProgram(shprog_basic);
	glBindVertexArray(cube_up_vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

//================================================================

static void rnd_glinit() {
	if (glewInit() != GLEW_OK) {
		RNDEXER("GLEW failed to initialize");
	}
	if (!GLEW_VERSION_4_5) {
		com_print("\n\tYour system does not seem to be fully OpenGL 4.5 compliant. "
			  "\n\tThis program was developed and tested on an OpenGL 4.5 compliant system, and relies on some OpenGL 4.5 functions. "
			  "\n\tYou may experience crashing or total non-function if your driver does not support these functions.");
	}
	glClearColor(1, 0, 0, 1);
}

//================================================================

static int current_width, current_height;
static int target_width, target_height;

static void * rnd_run(void * arg) {
	assert(arg == NULL);
	pthread_mutex_lock(&com_win_mut);
	glfwSwapInterval(1);
	glfwMakeContextCurrent(com_win);
	pthread_mutex_unlock(&com_win_mut);
	
	rnd_glinit();
	rnd_util_init();
	rnd_shader_init();
	
	timekeeper_t rndtk;
	timekeeper_init(&rndtk);
	timekeeper_set_target_FPS(&rndtk, 60);
	
	int glfwsc;
	do {
		rnd_frame();
		
		pthread_mutex_lock(&com_win_mut);
		glfwSwapBuffers(com_win);
		glfwGetFramebufferSize(com_win, &target_width, &target_height);
		pthread_mutex_unlock(&com_win_mut);
		
		if (target_width != current_width || target_height != current_height) {
			glViewport(0, 0, target_width, target_height);
			current_width = target_width;
			current_height = target_height;
		}
		
		timekeeper_sleep_for_target(&rndtk);
		
		pthread_mutex_lock(&com_win_mut);
		glfwsc = glfwWindowShouldClose(com_win);
		pthread_mutex_unlock(&com_win_mut);
	} while (!glfwsc);
	RNDEXIT;
}

//================================================================

void rnd_init() {
	atomic_init(&rnd_thrd_status, THRD_STATUS_RUNNING);
	pthread_create(&rnd_thrd, &com_thrd_attr, rnd_run, NULL);
}

