#ifndef RND_H
#define RND_H

#include "com.h"

extern pthread_t rnd_thrd;
extern com_thrd_status_ta rnd_thrd_status;
extern char rnd_thrd_err [COM_THRD_ERRLEN];
#define RNDSTAT atomic_load(&rnd_thrd_status)
#define RNDEXIT atomic_store(&rnd_thrd_status, COM_THRD_STATUS_EXIT_NORMAL); pthread_exit(NULL);
#define RNDEXER(err) strncpy(rnd_thrd_err, err, COM_THRD_ERRLEN); atomic_store(&rnd_thrd_status, COM_THRD_STATUS_EXIT_ERROR); pthread_exit(NULL);

int rnd_init();

#endif