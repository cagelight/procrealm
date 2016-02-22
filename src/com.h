#ifndef COM_H
#define COM_H

#include <assert.h>
#include <math.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#define thread_local _Thread_local

enum com_thrd_status_e {
	COM_THRD_STATUS_RUNNING,
	COM_THRD_STATUS_EXIT_NORMAL,
	COM_THRD_STATUS_EXIT_ERROR,
};
typedef uint_fast8_t com_thrd_status_t;
typedef atomic_uint_fast8_t com_thrd_status_ta;

#define COM_THRD_ERRLEN 1024

//================================================================
//----------------------------------------------------------------
//============================= CORE =============================
// com_core.c

enum com_signal_e {
	COM_SIGNAL_RUN,
	COM_SIGNAL_EXIT
};
typedef atomic_uint_fast8_t com_signal_ta;
typedef uint_fast8_t com_signal_t;
#define COMSIG ((com_signal_t)atomic_load(&com_signal))
extern com_signal_ta com_signal;

extern pthread_attr_t com_thrd_attr;

extern GLFWwindow * com_win;
extern pthread_mutex_t com_win_mut;

int com_init();
int com_run();

//================================================================
//----------------------------------------------------------------
//============================= UTIL =============================
// com_util.c

void com_util_init();
char * vas(char const * fmt, ...);

void com_print(char const *);
#define com_printf(str, ...) com_print(vas(str, ##__VA_ARGS__))

typedef struct target_sleeper_s {
	clockid_t clk_id;
	struct timespec target;
	struct timespec frame;
} target_sleeper_t;

void target_sleeper_init(target_sleeper_t *);
void target_sleeper_sleep_for_target(target_sleeper_t *);
void target_sleeper_set_target_FPS(target_sleeper_t *, double);

//================================================================
//----------------------------------------------------------------
//================================================================


#endif