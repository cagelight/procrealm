#include "com.h"

#include <stdarg.h>
#include <stdio.h>

//================================================================


static pthread_mutex_t prntmut;
void com_print(char const * str) {
	pthread_mutex_lock(&prntmut);
	printf(str);
	if (str[strlen(str)-1] != '\n') printf("\n");
	pthread_mutex_unlock(&prntmut);
}

void com_util_init() {
	pthread_mutex_init(&prntmut, NULL);
}

//========================== VAS =================================
//	Thread safe function for quick string formatting.
//	Allows nesting to an extent.

#define VAS_BUFLEN 1024 //Should be a size large enough for "general use"
#define VAS_BUFNUM 4 //Corresponds to maximum nesting depth

thread_local char vbufs [VAS_BUFLEN][VAS_BUFNUM];
thread_local int vbufn = 0;

char * vas(char const * fmt, ...) {
	va_list va;
	va_start(va, fmt);
	char * vbuf = vbufs[vbufn++];
	vsnprintf(vbuf, VAS_BUFLEN, fmt, va);
	va_end(va);
	if (vbufn == VAS_BUFNUM) vbufn = 0;
	return vbuf;
}

//======================== TARGET SLEEPER ========================

void target_sleeper_init(target_sleeper_t * tst) {
	memset(tst, 0, sizeof(target_sleeper_t));
	tst->clk_id = CLOCK_MONOTONIC;
	clock_gettime(tst->clk_id, &tst->frame);
}
void target_sleeper_sleep_for_target(target_sleeper_t * tst) {
	struct timespec t0;
	struct timespec t1;
	clock_gettime(tst->clk_id, &t0);
	t1 = tst->target;
	struct timespec tdiff;
	tdiff.tv_sec = t0.tv_sec - tst->frame.tv_sec;
	if (t0.tv_nsec < tst->frame.tv_nsec) {
		t0.tv_nsec += 1000000000;
		tdiff.tv_sec--;
		assert(t0.tv_sec >= 0);
		assert(t0.tv_nsec >= 0);
	}
	tdiff.tv_nsec = t0.tv_nsec - tst->frame.tv_nsec;
	assert(tdiff.tv_sec >= 0);
	assert(tdiff.tv_nsec >= 0);
	if (tdiff.tv_sec < t1.tv_sec || (tdiff.tv_sec == t1.tv_sec && tdiff.tv_nsec < t1.tv_nsec)) {
		struct timespec trem;
		trem.tv_sec = t1.tv_sec - tdiff.tv_sec;
		if (t1.tv_nsec < tdiff.tv_nsec) {
			t1.tv_nsec += 1000000000;
			trem.tv_sec--;
			assert(t1.tv_sec > 0);
			assert(t1.tv_nsec > 0);
		}
		trem.tv_nsec = t1.tv_nsec - tdiff.tv_nsec;
		assert(trem.tv_sec >= 0);
		assert(trem.tv_nsec >= 0);
		nanosleep(&trem, NULL);
	}
	clock_gettime(tst->clk_id, &tst->frame);
}
void target_sleeper_set_target_FPS(target_sleeper_t * tst, double fps) {
	fps = 1/fps;
	double ffps = floor(fps);
	tst->target.tv_sec = ffps;
	fps -= ffps;
	tst->target.tv_nsec = fps * 1000000000;
}