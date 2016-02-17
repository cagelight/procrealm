#ifndef INP_H
#define INP_H

#include "com.h"

extern pthread_t inp_thrd;
extern com_thrd_status_ta inp_thrd_status;
extern char inp_thrd_err [COM_THRD_ERRLEN];
#define INPSTAT atomic_load(&inp_thrd_status)
#define INPEXIT atomic_store(&inp_thrd_status, COM_THRD_STATUS_EXIT_NORMAL); pthread_exit(NULL);
#define INPEXER(err) strncpy(inp_thrd_err, err, COM_THRD_ERRLEN); atomic_store(&inp_thrd_status, COM_THRD_STATUS_EXIT_ERROR); pthread_exit(NULL);

int inp_init();

#endif