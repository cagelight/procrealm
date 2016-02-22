#include "inp.h"

pthread_t inp_thrd;
com_thrd_status_ta inp_thrd_status;
char inp_thrd_err [COM_THRD_ERRLEN];

#define KEYSTATE_INIT(keychar) atomic_init(&inp_key_##keychar, (inp_keystate_t)INP_KEYINACTIVE)

//================================================================

static inline void inp_keyupd(inp_keystate_t * astate, int glfwkey) {
	switch(glfwGetKey(com_win, glfwkey)) {
	case GLFW_PRESS:
		switch(*astate) {
		case INP_KEYINACTIVE:
			*astate = (inp_keystate_t)INP_KEYPRESSED;
			break;
		case INP_KEYPRESSED:
			*astate = (inp_keystate_t)INP_KEYHELD;
			break;
		case INP_KEYRELEASED:
			*astate = (inp_keystate_t)INP_KEYPRESSED;
			break;
		case INP_KEYHELD:
			break;
		default:
			com_print("keystate has unexpected value!");
		}
		break;
	case GLFW_RELEASE:
		switch(*astate) {
		case INP_KEYINACTIVE:
			break;
		case INP_KEYPRESSED:
			*astate = (inp_keystate_t)INP_KEYRELEASED;
			break;
		case INP_KEYRELEASED:
			*astate = (inp_keystate_t)INP_KEYINACTIVE;
			break;
		case INP_KEYHELD:
			*astate = (inp_keystate_t)INP_KEYRELEASED;
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

void inp_keyset_upd(inp_keyset_t * kst) {
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
	
	target_sleeper_t inp_slp;
	target_sleeper_init(&inp_slp);
	target_sleeper_set_target_FPS(&inp_slp, 250);
	
	inp_keyset_init(&inp_keyset);
	
	int glfwsc;
	do {
		pthread_mutex_lock(&com_win_mut);
		inp_frame();
		pthread_mutex_unlock(&com_win_mut);
		inp_keyset_upd(&inp_keyset);
		
		if (KEYPORH(&inp_keyset, esc)) {
			pthread_mutex_lock(&com_win_mut);
			glfwSetWindowShouldClose(com_win, 1);
			pthread_mutex_unlock(&com_win_mut);
		}
		
		target_sleeper_sleep_for_target(&inp_slp);
		
		pthread_mutex_lock(&com_win_mut);
		glfwsc = glfwWindowShouldClose(com_win);
		pthread_mutex_unlock(&com_win_mut);
	} while (!glfwsc);
	INPEXIT;
}

int inp_init() {	
	//glfwSetInputMode(com_win, GLFW_STICKY_KEYS, 1);
	//glfwSetInputMode(com_win, GLFW_STICKY_MOUSE_BUTTONS, 1);
	atomic_init(&inp_thrd_status, COM_THRD_STATUS_RUNNING);
	pthread_create(&inp_thrd, &com_thrd_attr, inp_run, NULL);
	return 0;
}