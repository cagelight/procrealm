#include "inp.h"

pthread_t inp_thrd;
thrd_status_ta inp_thrd_status;
char inp_thrd_err [THRD_ERRLEN];

#define KEYSTATE_INIT(keychar) atomic_init(&inp_key_##keychar, (inp_keystate_t)INP_KEYINACTIVE)

//================================================================

static inline void inp_keyupd(inp_keystuff_t * ks, timekeeper_t * sw) {
	switch(glfwGetKey(com_win, ks->glfwkey)) {
	case GLFW_PRESS:
		switch(ks->state) {
		case INP_KEYINACTIVE:
			ks->state = (inp_keystate_t)INP_KEYPRESSED; ks->time = 0;
			break;
		case INP_KEYPRESSED:
			ks->state = (inp_keystate_t)INP_KEYHELD; if (sw) {ks->time += sw->watch.impulse;}
			break;
		case INP_KEYRELEASED:
			ks->state = (inp_keystate_t)INP_KEYPRESSED; ks->time = 0;
			break;
		case INP_KEYHELD:
			if (sw) {ks->time += sw->watch.impulse;}
			break; 
		default:
			com_print("keystate has unexpected value!");
		}
		break;
	case GLFW_RELEASE:
		switch(ks->state) {
		case INP_KEYINACTIVE:
			if (sw) {ks->time += sw->watch.impulse;}
			break;
		case INP_KEYPRESSED:
			ks->state = (inp_keystate_t)INP_KEYRELEASED; ks->time = 0;
			break;
		case INP_KEYRELEASED:
			ks->state = (inp_keystate_t)INP_KEYINACTIVE; if (sw) {ks->time += sw->watch.impulse;}
			break;
		case INP_KEYHELD:
			ks->state = (inp_keystate_t)INP_KEYRELEASED; ks->time = 0;
			break;
		default:
			com_print("keystate has unexpected value!");
		}
		break;
	default:
		com_print("glfwGetKey returned unexpected value!");
		break;
	}
}

void inp_keyset_init(inp_keyset_t * kst) {
	#define XKEYSET_INIT
	#include "inp_xkeyset.h"
	#undef XKEYSET_INIT
}

void inp_keyset_upd(inp_keyset_t * kst, timekeeper_t * tk) {
	pthread_mutex_lock(&com_win_mut);
	#define XKEYSET_UPD
	#include "inp_xkeyset.h"
	#undef XKEYSET_UPD
	pthread_mutex_unlock(&com_win_mut);
}

//================================================================

static void inp_frame() {
	glfwPollEvents();
}

//================================================================

static inp_keyset_t inp_keyset;

static void * inp_run(void * arg) {
	assert(arg == NULL);
	
	timekeeper_t inp_slp;
	timekeeper_init(&inp_slp);
	timekeeper_set_target_FPS(&inp_slp, 250);
	
	inp_keyset_init(&inp_keyset);
	
	int glfwsc;
	do {
		pthread_mutex_lock(&com_win_mut);
		inp_frame();
		pthread_mutex_unlock(&com_win_mut);
		inp_keyset_upd(&inp_keyset, NULL);
		
		if (inp_keyset.esc.state & (INP_KEYHELD | INP_KEYPRESSED)) {
			pthread_mutex_lock(&com_win_mut);
			glfwSetWindowShouldClose(com_win, 1);
			pthread_mutex_unlock(&com_win_mut);
		}
		
		timekeeper_sleep_for_target(&inp_slp);
		
		pthread_mutex_lock(&com_win_mut);
		glfwsc = glfwWindowShouldClose(com_win);
		pthread_mutex_unlock(&com_win_mut);
	} while (!glfwsc);
	INPEXIT;
}

int inp_init() {	
	//glfwSetInputMode(com_win, GLFW_STICKY_KEYS, 1);
	//glfwSetInputMode(com_win, GLFW_STICKY_MOUSE_BUTTONS, 1);
	atomic_init(&inp_thrd_status, (int)THRD_STATUS_RUNNING);
	pthread_create(&inp_thrd, &com_thrd_attr, inp_run, NULL);
	return 0;
}
