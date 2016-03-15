#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#define thread_local _Thread_local
#define atomic _Atomic

char * vas(char const * fmt, ...);

#define MILLI 1000
#define KILO  1000
#define MICRO 1000000
#define MEGA  1000000
#define NANO  1000000000
#define GIGA  1000000000

typedef struct timekeeper_cacheclock_s {
	uint_fast16_t cache_day;
	uint_fast8_t cache_hour;
	uint_fast8_t cache_minute;
	uint_fast8_t cache_second;
	uint_fast16_t cache_millisecond;
	uint_fast16_t cache_microsecond;
	uint_fast16_t cache_nanosecond;
} timekeeper_cacheclock_t;

typedef struct timekeeper_stopwatch_s {
	unsigned int sec_total;
	double sec_part;
	double impulse;
} timekeeper_stopwatch_t;

typedef struct timekeeper_s {
	timekeeper_stopwatch_t watch;
	clockid_t clk_id;
	double timescale;
	struct timespec target;
	struct timespec frame;
} timekeeper_t;

void timekeeper_init(timekeeper_t *);
void timekeeper_sleep_for_target(timekeeper_t *);
void timekeeper_set_target_FPS(timekeeper_t *, double);
void timekeeper_cacheclock(timekeeper_t *, timekeeper_cacheclock_t *);

#endif


