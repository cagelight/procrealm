#include "util.h"

#include <math.h>
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

//============================== VAS =============================
//	Thread safe function for quick string formatting.
//	Allows nesting to an extent.

#define VAS_BUFLEN 1024 //Should be a size large enough for "general use"
#define VAS_BUFNUM 8 //Corresponds to maximum nesting depth

thread_local char vbufs [VAS_BUFNUM][VAS_BUFLEN];
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

//===============================  ===============================

void timekeeper_init(timekeeper_t * tst) {
	memset(tst, 0, sizeof(timekeeper_t));
	tst->clk_id = CLOCK_MONOTONIC;
	clock_gettime(tst->clk_id, &tst->frame);
	tst->timescale = 1.0;
}
void timekeeper_sleep_for_target(timekeeper_t * tst) {
	struct timespec mark;
	struct timespec t0;
	struct timespec t1;
	struct timespec tdiff;
	clock_gettime(tst->clk_id, &mark);
	t0 = mark;
	t1 = tst->target;
	tdiff.tv_sec = t0.tv_sec - tst->frame.tv_sec;
	if (t0.tv_nsec < tst->frame.tv_nsec) {
		t0.tv_nsec += NANO;
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
			t1.tv_nsec += NANO;
			trem.tv_sec--;
			assert(t1.tv_sec > 0);
			assert(t1.tv_nsec > 0);
		}
		trem.tv_nsec = t1.tv_nsec - tdiff.tv_nsec;
		assert(trem.tv_sec >= 0);
		assert(trem.tv_nsec >= 0);
		nanosleep(&trem, NULL);
		clock_gettime(tst->clk_id, &mark);
		t0 = mark;
		tdiff.tv_sec = t0.tv_sec - tst->frame.tv_sec;
		if (t0.tv_nsec < tst->frame.tv_nsec) {
			t0.tv_nsec += NANO;
			tdiff.tv_sec--;
			assert(t0.tv_sec >= 0);
			assert(t0.tv_nsec >= 0);
		}
		tdiff.tv_nsec = t0.tv_nsec - tst->frame.tv_nsec;
	}
	tst->watch.impulse = tdiff.tv_sec;
	tst->watch.impulse += tdiff.tv_nsec / (double)NANO;
	tst->watch.impulse *= tst->timescale;
	tst->watch.sec_part += tst->watch.impulse;
	while (tst->watch.sec_part >= 1.0) {
		tst->watch.sec_total += 1.0;
		tst->watch.sec_part -= 1.0;
	}
	tst->frame = mark;
}
void timekeeper_set_target_FPS(timekeeper_t * tst, double fps) {
	fps = 1/fps;
	double ffps = floor(fps);
	tst->target.tv_sec = ffps;
	fps -= ffps;
	tst->target.tv_nsec = fps * NANO;
}

void timekeeper_cacheclock(timekeeper_t * tk, timekeeper_cacheclock_t * cc) {
	memset(cc, 0, sizeof(timekeeper_cacheclock_t));
	
	unsigned int sec = tk->watch.sec_total;
	while (sec >= 86400) {
		sec -= 86400;
		cc->cache_day++;
	}
	while (sec >= 3600) {
		sec -= 3600;
		cc->cache_hour++;
	}
	while (sec >= 60) {
		sec -= 60;
		cc->cache_minute++;
	}
	cc->cache_second = sec;
	
	unsigned int nano = tk->watch.sec_part * NANO;
	while (nano > 1000000) {
		nano -= 1000000;
		cc->cache_millisecond++;
	}
	while (nano > 1000) {
		nano -= 1000;
		cc->cache_microsecond++;
	}
	cc->cache_nanosecond = nano;
}
