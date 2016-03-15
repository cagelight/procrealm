#ifndef COM_H
#define COM_H

#include "util.h"

#include <assert.h>
#include <math.h>
#include <pthread.h>
#include <stdatomic.h>
#include <string.h>

typedef struct timespec timespec_t;

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

enum thrd_status_e {
	THRD_STATUS_RUNNING,
	THRD_STATUS_EXIT_NORMAL,
	THRD_STATUS_EXIT_ERROR,
};
typedef uint_fast8_t thrd_status_t;
typedef atomic_uint_fast8_t thrd_status_ta;

#define THRD_ERRLEN 1024

//================================================================
//----------------------------------------------------------------
//============================= CORE =============================
// com_core.c

enum com_signal_e {
	COM_SIGNAL_RUN,
	COM_SIGNAL_EXIT
};
typedef uint_fast8_t com_signal_t;
typedef atomic_uint_fast8_t com_signal_ta;
#define COMSIG ((com_signal_t)atomic_load(&com_signal))
extern com_signal_ta com_signal;

extern pthread_attr_t com_thrd_attr;

extern GLFWwindow * com_win;
extern pthread_mutex_t com_win_mut;

void com_init();
void com_run();

//================================================================
//----------------------------------------------------------------
//============================= UTIL =============================
// com_util.c

void com_util_init();
void com_print(char const *);
#define com_printf(str, ...) com_print(vas(str, ##__VA_ARGS__))
#define com_printf_error(str, ...) com_print(vas("ERROR (%s): %s", __func__, vas(str, ##__VA_ARGS__)))

//================================================================
//----------------------------------------------------------------
//================================================================


#endif
